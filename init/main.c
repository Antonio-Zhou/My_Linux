/*
 * 执行内核所有的初始化操作,
 * 然后移到用户模式创建新进程,并在控制台设备上运行shell 程序
 * */

/*
 *  linux/init/main.c
 *
 *  (C) 1991  Linus Torvalds
 */

/*定义宏__LIBRARY__ 是为了包括定义在unistd.h中的内嵌汇编代码等信息*/
#define __LIBRARY__
#include <unistd.h>		/*标准符号常量与类型文件*/
#include <time.h>		/*时间类型头文件*/

/*
 * we need this inline - forking from kernel space will result
 * in NO COPY ON WRITE (!!!), until an execve is executed. This
 * is no problem, but for the stack. This is handled by not letting
 * main() use the stack at all after fork(). Thus, no function
 * calls - which means inline code for fork too, as otherwise we
 * would use the stack upon exit from 'fork()'.
 *
 * Actually only pause and fork are needed inline, so that there
 * won't be any messing with the stack from main(), but we define
 * some others too.
 */

/*int fork()系统调用:生成子进程*/
static inline _syscall0(int,fork)
/*int pause()系统调用:暂停进程的执行,直到收到一个信号*/
static inline _syscall0(int,pause)
/*int setup(void *BIOS)系统调用,仅使用于linux初始化(仅在这个程序被调用)*/
static inline _syscall1(int,setup,void *,BIOS)
/*int  sync()系统调用:更新文件系统*/
static inline _syscall0(int,sync)

#include <linux/tty.h>			/*tty头文件,定义了有关tty_io,串行通信方面的参数,常数*/
#include <linux/sched.h>		/*
					 * 调度程序头文件,定义了任务结构task_struct,第一个初始任务的数据
					 * 还有一些以宏形式定义的有关描述符参数设置和获取的嵌入式汇编函数程序
					 * */
#include <linux/head.h>			/*head头文件,定义了段描述符的简单结构,和几个选择符常量*/
#include <asm/system.h>			/*系统头文件,以宏形式定义了很多有关设置或修改描述符/中断门等嵌入式汇编子程序*/
#include <asm/io.h>			/*io头文件,以宏的嵌入式汇编程序形式定义对io 端口操作的函数*/

#include <stddef.h>			/*标准定义头文件,定义了NULL,offset(TYPE,MEMBER)*/
#include <stdarg.h>			/*标准参数头文件,以宏的形式定义变量参数列表*/
#include <unistd.h>
#include <fcntl.h>			/*文件控制头文件,用于文件及其描述符的操作控制常数符号的定义*/
#include <sys/types.h>			/*类型头文件,定义了基本的系统数据类型*/

#include <linux/fs.h>			/*文件系统头文件,定义文件表结构*/

static char printbuf[1024];			/*静态字符串数组,用作内核显示信息的缓存*/

extern int vsprintf();				/*送格式化输出到一字符串中*/
extern void init(void);				/*初始化*/
extern void blk_dev_init(void);			/*块设备初始化子程序*/
extern void chr_dev_init(void);			/*字符设备初始化*/
extern void hd_init(void);			/*硬盘初始化*/
extern void floppy_init(void);			/*软驱初始化*/
extern void mem_init(long start, long end);	/*内存管理初始化*/
extern long rd_init(long mem_start, int length);/*虚拟盘初始化*/
extern long kernel_mktime(struct tm * tm);	/*计算系统开机启动时间(秒)*/
extern long startup_time;			/*内核启动时间(秒)*/
/*
 * This is set up by the setup-routine at boot-time
 */

/*将下面指定的线性地址强行转换成给定类型数据类型的指针,并获取指针所指的内容*/
#define EXT_MEM_K (*(unsigned short *)0x90002)			/*1MB以后的扩展内存大小(KB)*/
#define DRIVE_INFO (*(struct drive_info *)0x90080)		/*硬盘参数的32字节内容*/
#define ORIG_ROOT_DEV (*(unsigned short *)0x901FC)		/*根文件系统所在设备号*/

/*
 * Yeah, yeah, it's ugly, but I cannot find how to do this correctly
 * and this seems to work. I anybody has more info on the real-time
 * clock I'd be interested. Most of this was trial and error, and some
 * bios-listing reading. Urghh.
 */

/*读取CMOS时钟信息的宏定义*/
/*这段宏读取CMOS实时时钟信息.outb_p和inb_p在include/asm/io.h中定义的端口输入输出宏*/
#define CMOS_READ(addr) ({ \
outb_p(0x80|addr,0x70); \
inb_p(0x71); \
})

/*一个世纪字节BCD码的实际二进制数值*/
#define BCD_TO_BIN(val) ((val)=((val)&15) + ((val)>>4)*10)

/*获取当前芯片中保存的当前时间和日期信息*/
static void time_init(void)
{
	struct tm time;

/*CMOS的访问速度很慢,为了减少误差,在读取下面循环中所有数值之后,若此时CMOS中秒值发生了变化,就重新读取所有值将误差控制在1秒之内*/
	do {
		time.tm_sec = CMOS_READ(0);		/*为什么读出来的是BCD 码*/
		time.tm_min = CMOS_READ(2);
		time.tm_hour = CMOS_READ(4);
		time.tm_mday = CMOS_READ(7);
		time.tm_mon = CMOS_READ(8);
		time.tm_year = CMOS_READ(9);
	} while (time.tm_sec != CMOS_READ(0));
	BCD_TO_BIN(time.tm_sec);
	BCD_TO_BIN(time.tm_min);
	BCD_TO_BIN(time.tm_hour);
	BCD_TO_BIN(time.tm_mday);
	BCD_TO_BIN(time.tm_mon);
	BCD_TO_BIN(time.tm_year);
	time.tm_mon--;
	startup_time = kernel_mktime(&time);		/*计算开机时间*/
}

static long memory_end = 0;				/*机器具有的物理内存容量(字节数)*/
static long buffer_memory_end = 0;			/*高速缓存区末端地址*/
static long main_memory_start = 0;			/*主内存(用于分页)开始的位置*/

struct drive_info { char dummy[32]; } drive_info;	/*用于存放硬盘参数信息*/


/*内核初始化主程序,初始化结束后将以任务0(idle任务即空闲任务)的身份运行*/
void main(void)		/* This really IS void, no error here. */
{			/* The startup routine assumes (well, ...) this */
/*
 * Interrupts are still disabled. Do necessary setups, then
 * enable them
 */
	ROOT_DEV = ORIG_ROOT_DEV;			/*保存根设备号*/
	drive_info = DRIVE_INFO;			/*复制0x90080处的硬盘参数表*/
	memory_end = (1<<20) + (EXT_MEM_K<<10);		/*内存大小 = 1Mb + 扩展内存(k) * 1024字节*/
	memory_end &= 0xfffff000;			/*忽略不到4kb(1页)的内存数*/
	if (memory_end > 16*1024*1024)
	{
		memory_end = 16*1024*1024;
	}
	if (memory_end > 12*1024*1024) 
	{
		buffer_memory_end = 4*1024*1024;
	}
	else if (memory_end > 6*1024*1024)
	{
		buffer_memory_end = 2*1024*1024;
	}
	else
	{
		buffer_memory_end = 1*1024*1024;
	}
	main_memory_start = buffer_memory_end;

/*如果在Makefile文件中定义了内存虚拟盘符号RAMDISK,则初始化虚拟盘,主内存将减少*/
#ifdef RAMDISK
	main_memory_start += rd_init(main_memory_start, RAMDISK*1024);
#endif

/*下面是内核进行所有方面的初始化工作*/
	mem_init(main_memory_start,memory_end);		/*主内存区初始化*/
	trap_init();					/*陷阱门(硬件中断向量)初始化xxxxxxxxxxx*/
	blk_dev_init();					/*块设备初始化*/
	chr_dev_init();					/*字符设备初始化*/
	tty_init();					/*tty初始化*/
	time_init();					/*设置开机启动时间*/
	sched_init();					/*调度程序初始化*/
	buffer_init(buffer_memory_end);			/*缓冲区管理初始化*/
	hd_init();					/*硬盘初始化*/
	floppy_init();					/*软驱初始化*/
	sti();						/*所有初始化做完之后,开启中断*/

/*
 * 在此之前,系统一直使用head.s中定义的user_stack堆栈
 * 通过在堆栈中设置参数,利用中断返回指令启动任务0执行
 * */
	move_to_user_mode();				/*移到用户模式下执行*/
	if (!fork()) {		/* we count on this going ok */
		/*
		 * init()将在任务1中执行,
		 * 并使用任务1的堆栈main()"切换"成为任务0后继续使用上述内核程序自己的堆栈作为自己的用户态堆栈
		 * */
		init();					/*在新建的子进程(任务1)中执行*/
	}

/*下面的代码开始以任务0的身份执行*/

/*
 *   NOTE!!   For any other task 'pause()' would mean we have to get a
 * signal to awaken, but task0 is the sole exception (see 'schedule()')
 * as task 0 gets activated at every idle moment (when no other tasks
 * can run). For task0 'pause()' just means we go check if some other
 * task can run, and if not we return here.
 */
	/*
	 * pause()系统调用会把任务0转换成可中断等待状态,再执行调度函数
	 * 但是调度函数只要发现系统中没有其他任务可以运行时就会切换到任务0,而不依赖于任务0的状态
	 * */
	for(;;) pause();
}

static int printf(const char *fmt, ...)
{
	va_list args;
	int i;

	va_start(args, fmt);
	write(1,printbuf,i=vsprintf(printbuf, fmt, args));
	va_end(args);
	return i;
}

/*读取并执行/etc/rc文件时所使用的命令行参数和环境参数*/
static char * argv_rc[] = { "/bin/sh", NULL };				/*调用执行程序时参数的字符串数组*/
static char * envp_rc[] = { "HOME=/", NULL };				/*调用执行程序时的环境字符串数组*/

/*
 * 运行登录shell时所使用的命令行参数和环境参数
 * '-':传递给shell程序sh的一个标志.
 * 通过识别该标志,sh程序会作为登录shell执行,其执行过程和在shell提示符下执行sh不一样
 * */
static char * argv[] = { "-/bin/sh",NULL };
static char * envp[] = { "HOME=/usr/root", NULL };

void init(void)
{
	int pid,i;

	/*setup()是系统调用,用于读取硬盘参数包括分区表信息并加载虚拟盘和安装根文件系统设备*/
	setup((void *) &drive_info);
	(void) open("/dev/tty0",O_RDWR,0);		/*对应终端控制器*/
	(void) dup(0);					/*复制句柄,产生句柄一号-->stdout(标准输出设备)*/
	(void) dup(0);					/*复制句柄,产生句柄二号-->stdin(标准输入设备)*/
	/*打印缓冲区块数和总字节数,每块1024字节,以及内存区空闲内存字节数*/
	printf("%d buffers = %d bytes buffer space\n\r",NR_BUFFERS,
		NR_BUFFERS*BLOCK_SIZE);
	printf("Free mem: %d bytes\n\r",memory_end-main_memory_start);

	/*fork()创建子进程(任务2)*/
	if (!(pid=fork())) {
		close(0);				/*关闭句柄0(stdin)*/
		if (open("/etc/rc",O_RDONLY,0))
			_exit(1);			/*以只读方式打开文件/etc/rc,若失败,则退出*/
		execve("/bin/sh",argv_rc,envp_rc);	/*将进程自身替换成/bin/sh程序(shell程序)*/
		_exit(2);
	}

	/*下面是父进程执行的语句*/
	if (pid>0)
		/*
		 * 等待子进程停止或终止返回值是子进程的进程号
		 * &i:存放返回状态信息的位置
		 * */
		while (pid != wait(&i))

			/* nothing */;

	/*前面的创建的子进程的执行已经停止或终止*/
	while (1) {
		if ((pid=fork())<0) {
			printf("Fork failed in init\r\n");
			continue;
		}
		if (!pid) {					/*新的子进程*/
			close(0);close(1);close(2);		/*关闭所有以前遗留下来的句柄(stdin,stdout,stderr)*/
			setsid();				/*创建一个新的会话期*/
			(void) open("/dev/tty0",O_RDWR,0);	/*重新打开/dev/tty0作为stdin,并复制成stdout和stderr*/
			(void) dup(0);
			(void) dup(0);
			_exit(execve("/bin/sh",argv,envp));	/*参数与前面的不一样!!!*/
		}
		while (1)
			/*
			 * wait()的另外一个功能是处理孤儿进程,
			 * 若父进程先于子进程而结束,那么init进程将充当子进程的父进程,并由init进程负责释放一个已终止进程的任务数据结构等资源!!
			 * */
			if (pid == wait(&i))
				break;
		printf("\n\rchild %d died with code %04x\n\r",pid,i);
		sync();						/*同步操作,刷新缓冲区*/
	}
	/*
	 * _exit()和exit()都用于正常终止一个函数,区别:
	 * _exit()直接是一个sys_exit()系统调用;
	 * exit()通常是普通函数库中的一个函数,它会先执行一些清除操作,例如调用执行各终止处理程序,关闭所有标准io等,然后调用sys_exit()
	 * */
	_exit(0);	/* NOTE! _exit, not exit() */
}
