/*
 * 主要实现对硬盘数据块进行读/写的底层驱动函数
 * 主要是do_hd_request()函数
 * */

/*
 *  linux/kernel/hd.c
 *
 *  (C) 1991  Linus Torvalds
 */

/*
 * This is the low-level hd interrupt support. It traverses the
 * request-list, using interrupts to jump between functions. As
 * all the functions are called within interrupts, we may not
 * sleep. Special care is recommended.
 * 
 *  modified by Drew Eckhardt to check nr of hd's from the CMOS.
 */

#include <linux/config.h>
#include <linux/sched.h>
#include <linux/fs.h>
#include <linux/kernel.h>
#include <linux/hdreg.h>
#include <asm/system.h>
#include <asm/io.h>
#include <asm/segment.h>

#define MAJOR_NR 3
#include "blk.h"

#define CMOS_READ(addr) ({ \
outb_p(0x80|addr,0x70); \
inb_p(0x71); \
})

/* Max read/write errors/sector */
#define MAX_ERRORS	7
#define MAX_HD		2

static void recal_intr(void);

static int recalibrate = 1;
static int reset = 1;

/*
 *  This struct defines the HD's and their types.
 */
struct hd_i_struct {
	int head,sect,cyl,wpcom,lzone,ctl;
	};
#ifdef HD_TYPE
struct hd_i_struct hd_info[] = { HD_TYPE };
#define NR_HD ((sizeof (hd_info))/(sizeof (struct hd_i_struct)))
#else
struct hd_i_struct hd_info[] = { {0,0,0,0,0,0},{0,0,0,0,0,0} };
static int NR_HD = 0;
#endif

static struct hd_struct {
	long start_sect;
	long nr_sects;
} hd[5*MAX_HD]={{0,0},};

#define port_read(port,buf,nr) \
__asm__("cld;rep;insw"::"d" (port),"D" (buf),"c" (nr):"cx","di")

#define port_write(port,buf,nr) \
__asm__("cld;rep;outsw"::"d" (port),"S" (buf),"c" (nr):"cx","si")

extern void hd_interrupt(void);
extern void rd_load(void);

/* This may be used only once, enforced by 'static int callable' */

/*
 * 功能: main.c里的init()中setup的对应函数: 系统设置函数
 * 	 读取CMOS和硬盘参数表信息,用于设置硬盘分区结构hd并尝试加载RAM虚拟盘和根文件系统
 * 参数: void * BIOS-->由初始化程序init/main.c中init子程序设置为指向硬盘参数表结构的指针
 * 	该硬盘参数表结构包含2个硬盘参数表的内容(32字节),是从内存0x90080处复制而来
 * 返回值:
 * */
int sys_setup(void * BIOS)
{
	static int callable = 1;	/*限制本函数只能被调用一次的标志*/
	int i,drive;
	unsigned char cmos_disks;
	struct partition *p;
	struct buffer_head * bh;

	if (!callable)			/*这只callable标志位*/
	{
		return -1;
	}
	callable = 0;
/*如果没有定义HD_DYPE读取内存0X90080处开始的硬盘参数表*/
#ifndef HD_TYPE
	for (drive=0 ; drive<2 ; drive++) {
		hd_info[drive].cyl = *(unsigned short *) BIOS;		/*柱面数*/
		hd_info[drive].head = *(unsigned char *) (2+BIOS);	/*磁头数*/
		hd_info[drive].wpcom = *(unsigned short *) (5+BIOS);	/*写前预补偿柱面号*/
		hd_info[drive].ctl = *(unsigned char *) (8+BIOS);	/*控制字节*/
		hd_info[drive].lzone = *(unsigned short *) (12+BIOS);	/*磁头着陆区柱面号*/
		hd_info[drive].sect = *(unsigned char *) (14+BIOS);	/*每磁道扇区数*/
		BIOS += 16;						/*指向下一个参数表*/
	}

	/*判断第2个硬盘柱面数是否是0就可以知道是否有第2个硬盘*/
	if (hd_info[1].cyl)
		NR_HD=2;
	else
		NR_HD=1;
#endif

	/*
	 * 硬盘信息数组已经设置好,并确定了系统的硬盘数NR_HD,现在设置硬盘分区结构数组hd[]
	 * 项0: 第一个硬盘的整体参数
	 * 项5: 第二个硬盘的整体参数
	 * 项1-4: 第一个硬盘的4个分区参数
	 * 和项6-9: 第二个硬盘的4个分区参数
	 * */
	for (i=0 ; i<NR_HD ; i++) {
		hd[i*5].start_sect = 0;					/*硬盘其实扇区号*/
		hd[i*5].nr_sects = hd_info[i].head*
				hd_info[i].sect*hd_info[i].cyl;		/*硬盘总扇区数*/
	}

	/*
		We querry CMOS about hard disks : it could be that 
		we have a SCSI/ESDI/etc controller that is BIOS
		compatable with ST-506, and thus showing up in our
		BIOS table, but not register compatable, and therefore
		not present in CMOS.

		Furthurmore, we will assume that our ST-506 drives
		<if any> are the primary drives in the system, and 
		the ones reflected as drive 1 or 2.

		The first drive is stored in the high nibble of CMOS
		byte 0x12, the second in the low nibble.  This will be
		either a 4 bit drive type or 0xf indicating use byte 0x19 
		for an 8 bit type, drive 1, 0x1a for drive 2 in CMOS.

		Needless to say, a non-zero value means we have 
		an AT controller hard disk for that drive.

		
	*/

	/*
	 * 检测硬盘到底是不是AT 控制器兼容
	 * 从CMOS偏移地址0x12  处读出硬盘类型字节
	 * 低半字节(存放着第2个硬盘类型值)不为0-->表示系统有两硬盘
	 * 0x12处读出的值为0-->系统中没有AT兼容盘
	 * */
	if ((cmos_disks = CMOS_READ(0x12)) & 0xf0)
	{
		if (cmos_disks & 0x0f)
		{
			NR_HD = 2;
		}
		else
		{
			NR_HD = 1;
		}
	}
	else
	{
		NR_HD = 0;
	}

	/*
	 * NR_HD = 0: 两个硬盘都不是AT控制器兼容的,两个硬盘数据结构全清零
	 * NR_HD = 1: 将第二个硬盘参数清空
	 * */
	for (i = NR_HD ; i < 2 ; i++) {
		hd[i*5].start_sect = 0;
		hd[i*5].nr_sects = 0;
	}

	/*
	 * 以上真正确定了NR_HD
	 * 下面读取每个硬盘上第一个扇区中的分区表信息,用来设置分区结构数组hd[]中硬盘各分区信息
	 * */
	for (drive=0 ; drive<NR_HD ; drive++) {
		if (!(bh = bread(0x300 + drive*5,0))) {					/*0x300,0x305是设备号*/
			printk("Unable to read partition table of drive %d\n\r",
				drive);
			panic("");
		}
		if (bh->b_data[510] != 0x55 || (unsigned char)
		    bh->b_data[511] != 0xAA) {						/*判断硬盘标志0xAA55*/
			printk("Bad partition table on drive %d\n\r",drive);
			panic("");
		}
		p = 0x1BE + (void *)bh->b_data;						/*分区表位于第1扇区0x1BE处*/
		for (i=1;i<5;i++,p++) {
			hd[i+5*drive].start_sect = p->start_sect;
			hd[i+5*drive].nr_sects = p->nr_sects;
		}
		brelse(bh);								/*释放为存放硬盘数据块而申请的缓冲区*/
	}

	/*
	 * 前面完成设置硬盘分区结构数组hd[]
	 * 接下来
	 * 1.尝试在系统内存虚拟盘中加装启动盘中包含的根文件系统映像,如果有,则尝试把该映像加载并存放到虚拟盘中
	 * 2.把此时的根文件系统设备号ROOT_DEV修改成虚拟盘的设备号
	 * 3.安装根文件系统
	 * */
	if (NR_HD)
		printk("Partition table%s ok.\n\r",(NR_HD>1)?"s":"");
	rd_load();									/*尝试创建并加装虚拟盘*/
	mount_root();									/*安装根文件系统*/
	return (0);
}

static int controller_ready(void)
{
	int retries=10000;

	while (--retries && (inb_p(HD_STATUS)&0xc0)!=0x40);
	return (retries);
}

static int win_result(void)
{
	int i=inb_p(HD_STATUS);

	if ((i & (BUSY_STAT | READY_STAT | WRERR_STAT | SEEK_STAT | ERR_STAT))
		== (READY_STAT | SEEK_STAT))
		return(0); /* ok */
	if (i&1) i=inb(HD_ERROR);
	return (1);
}

static void hd_out(unsigned int drive,unsigned int nsect,unsigned int sect,
		unsigned int head,unsigned int cyl,unsigned int cmd,
		void (*intr_addr)(void))
{
	register int port asm("dx");

	if (drive>1 || head>15)
		panic("Trying to write bad sector");
	if (!controller_ready())
		panic("HD controller not ready");
	do_hd = intr_addr;
	outb_p(hd_info[drive].ctl,HD_CMD);
	port=HD_DATA;
	outb_p(hd_info[drive].wpcom>>2,++port);
	outb_p(nsect,++port);
	outb_p(sect,++port);
	outb_p(cyl,++port);
	outb_p(cyl>>8,++port);
	outb_p(0xA0|(drive<<4)|head,++port);
	outb(cmd,++port);
}

static int drive_busy(void)
{
	unsigned int i;

	for (i = 0; i < 10000; i++)
		if (READY_STAT == (inb_p(HD_STATUS) & (BUSY_STAT|READY_STAT)))
			break;
	i = inb(HD_STATUS);
	i &= BUSY_STAT | READY_STAT | SEEK_STAT;
	if (i == READY_STAT | SEEK_STAT)
		return(0);
	printk("HD controller times out\n\r");
	return(1);
}

static void reset_controller(void)
{
	int	i;

	outb(4,HD_CMD);
	for(i = 0; i < 100; i++) nop();
	outb(hd_info[0].ctl & 0x0f ,HD_CMD);
	if (drive_busy())
		printk("HD-controller still busy\n\r");
	if ((i = inb(HD_ERROR)) != 1)
		printk("HD-controller reset failed: %02x\n\r",i);
}

static void reset_hd(int nr)
{
	reset_controller();
	hd_out(nr,hd_info[nr].sect,hd_info[nr].sect,hd_info[nr].head-1,
		hd_info[nr].cyl,WIN_SPECIFY,&recal_intr);
}

/*
 * 意外硬盘中断调用函数
 * 发生意外硬盘中断函数时,硬盘中断处理程序中调用默认C处理函数
 * 在被调用函数指针为空时调用该函数
 * */
void unexpected_hd_interrupt(void)
{
	printk("Unexpected HD interrupt\n\r");
}

static void bad_rw_intr(void)
{
	if (++CURRENT->errors >= MAX_ERRORS)
		end_request(0);
	if (CURRENT->errors > MAX_ERRORS/2)
		reset = 1;
}

static void read_intr(void)
{
	if (win_result()) {
		bad_rw_intr();
		do_hd_request();
		return;
	}
	port_read(HD_DATA,CURRENT->buffer,256);
	CURRENT->errors = 0;
	CURRENT->buffer += 512;
	CURRENT->sector++;
	if (--CURRENT->nr_sectors) {
		do_hd = &read_intr;
		return;
	}
	end_request(1);
	do_hd_request();
}

static void write_intr(void)
{
	if (win_result()) {
		bad_rw_intr();
		do_hd_request();
		return;
	}
	if (--CURRENT->nr_sectors) {
		CURRENT->sector++;
		CURRENT->buffer += 512;
		do_hd = &write_intr;
		port_write(HD_DATA,CURRENT->buffer,256);
		return;
	}
	end_request(1);
	do_hd_request();
}

static void recal_intr(void)
{
	if (win_result())
		bad_rw_intr();
	do_hd_request();
}

void do_hd_request(void)
{
	int i,r;
	unsigned int block,dev;
	unsigned int sec,head,cyl;
	unsigned int nsect;

	INIT_REQUEST;
	dev = MINOR(CURRENT->dev);
	block = CURRENT->sector;
	if (dev >= 5*NR_HD || block+2 > hd[dev].nr_sects) {
		end_request(0);
		goto repeat;
	}
	block += hd[dev].start_sect;
	dev /= 5;
	__asm__("divl %4":"=a" (block),"=d" (sec):"0" (block),"1" (0),
		"r" (hd_info[dev].sect));
	__asm__("divl %4":"=a" (cyl),"=d" (head):"0" (block),"1" (0),
		"r" (hd_info[dev].head));
	sec++;
	nsect = CURRENT->nr_sectors;
	if (reset) {
		reset = 0;
		recalibrate = 1;
		reset_hd(CURRENT_DEV);
		return;
	}
	if (recalibrate) {
		recalibrate = 0;
		hd_out(dev,hd_info[CURRENT_DEV].sect,0,0,0,
			WIN_RESTORE,&recal_intr);
		return;
	}	
	if (CURRENT->cmd == WRITE) {
		hd_out(dev,nsect,sec,head,cyl,WIN_WRITE,&write_intr);
		for(i=0 ; i<3000 && !(r=inb_p(HD_STATUS)&DRQ_STAT) ; i++)
			/* nothing */ ;
		if (!r) {
			bad_rw_intr();
			goto repeat;
		}
		port_write(HD_DATA,CURRENT->buffer,256);
	} else if (CURRENT->cmd == READ) {
		hd_out(dev,nsect,sec,head,cyl,WIN_READ,&read_intr);
	} else
		panic("unknown hd-command");
}

void hd_init(void)
{
	blk_dev[MAJOR_NR].request_fn = DEVICE_REQUEST;
	set_intr_gate(0x2E,&hd_interrupt);
	outb_p(inb_p(0x21)&0xfb,0x21);
	outb(inb_p(0xA1)&0xbf,0xA1);
}
