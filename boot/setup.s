!
!
!	/*用于读取机器的硬件配置参数,并把内核模块system移动到适当的内存位置处*/
!
!	setup.s		(C) 1991 Linus Torvalds
!
! setup.s is responsible for getting the system data from the BIOS,
! and putting them into the appropriate places in system memory.
! both setup.s and system has been loaded by the bootblock.
!
! This code asks the bios for memory/disk/other parameters, and
! puts them in a "safe" place: 0x90000-0x901FF, ie where the
! boot-block used to be. It is then up to the protected mode
! system to read them from there before the area is overwritten
! for buffer-blocks.
!

! NOTE! These had better be the same as in bootsect.s!

/*下面这些参数最好和bootsect.s中相同*/

INITSEG  = 0x9000	! we move boot here - out of the way
SYSSEG   = 0x1000	! system loaded at 0x10000 (65536).
SETUPSEG = 0x9020	! this is the current segment

.globl begtext, begdata, begbss, endtext, enddata, endbss
.text
begtext:
.data
begdata:
.bss
begbss:
.text

entry start
start:

! ok, the read went well so we get current cursor position and save it for
! posterity.

	/*利用BIOS中断0x10读取光标位置,保存*/
	mov	ax,#INITSEG	! this is done in bootsect already, but...
	mov	ds,ax
	mov	ah,#0x03	! read cursor pos
	xor	bh,bh
	int	0x10		! save it in known place, con_init fetches
	mov	[0],dx		! it from 0x90000.

! Get memory size (extended mem, kB)

	/*利用BIOS中断0x15读取扩展内存大小*/
	mov	ah,#0x88
	int	0x15
	mov	[2],ax		/*将数值存放在0x90002处1个字*/

! Get video-card data:

	/*利用BIOS中断0x10读取显示卡当前显示模式*/
	mov	ah,#0x0f
	int	0x10
	mov	[4],bx		! bh = display page			/*当前页0c90004*/
	mov	[6],ax		! al = video mode, ah = window width	/*0x90006-->显示模式,0x90007-->字符序列*/

! check for EGA/VGA and some config parameters

	/*利用BIOS中断0x10检查显示方式并取参数*/
	mov	ah,#0x12
	mov	bl,#0x10
	int	0x10
	mov	[8],ax
	mov	[10],bx		/*0x9000A = 安装的显示内存,0x9000B = 显示状态(彩色/单色)*/
	mov	[12],cx		/*0x9000C = 显示卡特性参数*/

! Get hd0 data

	/*取第一个硬盘的信息ds  =  段地址，si  =  段内偏移地址*/
	mov	ax,#0x0000
	mov	ds,ax
	lds	si,[4*0x41]		/*取中断向量0x41的值,也即hd0参数表的地址ds:si*/
	mov	ax,#INITSEG
	mov	es,ax
	mov	di,#0x0080		/*传输的目的地址:0x9000:0x0080 = es:si*/
	mov	cx,#0x10		/*传输16字节--表的长度*/
	rep
	movsb

! Get hd1 data

	mov	ax,#0x0000
	mov	ds,ax
	lds	si,[4*0x46]		/*取中断向量0x46的值,也即hd1参数表的地址ds:si*/
	mov	ax,#INITSEG
	mov	es,ax
	mov	di,#0x0090		/*传输的目的地址:0x9000:0x0090 = es:si*/
	mov	cx,#0x10
	rep
	movsb

! Check that there IS a hd1 :-)

	/*利用BIOS中断0x13读取盘类型,检查是否有第2个硬盘,没有就把第2个表清零*/
	mov	ax,#0x01500
	mov	dl,#0x81
	int	0x13
	jc	no_disk1
	cmp	ah,#3			/*是硬盘???*/
	je	is_disk1
no_disk1:
	mov	ax,#INITSEG		/*第2个硬盘不存在,则将第2个表清0*/
	mov	es,ax
	mov	di,#0x0090
	mov	cx,#0x10
	mov	ax,#0x00
	rep
	stosb
is_disk1:

! now we want to move to protected mode ...
	/*进入保护模式*/

	cli			! no interrupts allowed !	/*从此开始不允许中断*/

! first we move the system to it's rightful place

	/*把整个system模块从0x10000移动到0x00000位置*/
	mov	ax,#0x0000
	cld			! 'direction'=0, movs moves forward	/*方向标志位清0*/
do_move:
	mov	es,ax		! destination segment
	add	ax,#0x1000
	cmp	ax,#0x9000		/*最后一段移动完*/
	jz	end_move
	mov	ds,ax		! source segment
	sub	di,di
	sub	si,si
	mov 	cx,#0x8000		/*64kb字节*/
	rep
	movsw
	jmp	do_move

! then we load the segment descriptors

/*
 * 加载段描述符
 * 会遇到32  位保护模式的操作
 * 在进入保护模式中运行之前，我们要首先设置好要使用的段描述符表
 * 这里是全局描述符表和中段描述符表
 * */

end_move:
	mov	ax,#SETUPSEG	! right, forgot this at first. didn't work :-)
	mov	ds,ax		/*ds指向本程序的段*/
	lidt	idt_48		! load idt with 0,0				/*加载IDT寄存器*/
	lgdt	gdt_48		! load gdt with whatever appropriate		/*加载GDT寄存器*/

! that was painless, now we enable A20

	call	empty_8042		/*测试8042状态寄存器,等待输入缓冲器空,只有当输入缓冲器为空时才可以对其执行写命令*/
	mov	al,#0xD1		! command write		/*0xD1-->要写数据到*/
	out	#0x64,al		/*8042的P2端口,位1用于A20线的选通,数据要写到0x60口*/
	call	empty_8042		/*等待输入缓冲器空,看命令是否被接收*/
	mov	al,#0xDF		! A20 on		/*选通A20地址线的参数*/
	out	#0x60,al
	call	empty_8042		/*若此时输入缓冲器为空,则表示A20地址线已经选通*/

! well, that went ok, I hope. Now we have to reprogram the interrupts :-(
! we put them right after the intel-reserved hardware interrupts, at
! int 0x20-0x2F. There they won't mess up anything. Sadly IBM really
! messed this up with the original PC, and they haven't been able to
! rectify it afterwards. Thus the bios puts interrupts at 0x08-0x0f,
! which is used for the internal hardware interrupts as well. We just
! have to reprogram the 8259's, and it isn't fun.

	mov	al,#0x11		! initialization sequence
	out	#0x20,al		! send it to 8259A-1		/*主芯片*/
	.word	0x00eb,0x00eb		! jmp $+2, jmp $+2		/*'$':当前指令的地址,起延时作用*/
	out	#0xA0,al		! and to 8259A-2		/*从芯片*/
	.word	0x00eb,0x00eb

	/*Linux 系统硬件中断号被设置成从0x20  开始，*/
	mov	al,#0x20		! start of hardware int's (0x20)
	out	#0x21,al		/*送主芯片ICW2命令字,设置起始中断号*/
	.word	0x00eb,0x00eb
	mov	al,#0x28		! start of hardware int's 2 (0x28)
	out	#0xA1,al		/*送从芯片ICW2命令字,从芯片的起始中断号*/
	.word	0x00eb,0x00eb
	mov	al,#0x04		! 8259-1 is master
	out	#0x21,al		/*送主芯片CW3命令字,主芯片的IR2连接从芯片的INT*/
	.word	0x00eb,0x00eb
	mov	al,#0x02		! 8259-2 is slave
	out	#0xA1,al		/*送从芯片ICW3命令字,从芯片的INT连接到主芯片的IR2引脚*/
	.word	0x00eb,0x00eb
	mov	al,#0x01		! 8086 mode for both
	out	#0x21,al		/*送主芯片CW4命令字,8086模式*/
	.word	0x00eb,0x00eb
	out	#0xA1,al		/*送从芯片CW4命令字*/
	.word	0x00eb,0x00eb
	mov	al,#0xFF		! mask off all interrupts for now
	out	#0x21,al		/*屏蔽主芯片所有中断请求*/
	.word	0x00eb,0x00eb
	out	#0xA1,al		/*屏蔽从芯片所有中断请求*/

! well, that certainly wasn't fun :-(. Hopefully it works, and we don't
! need no steenking BIOS anyway (except for the initial loading :-).
! The BIOS-routine wants lots of unnecessary data, and it's less
! "interesting" anyway. This is how REAL programmers do it.
!
! Well, now's the time to actually move into protected mode. To make
! things as simple as possible, we do no register set-up or anything,
! we let the gnu-compiled 32-bit programs do that. We just jump to
! absolute address 0x00000, in 32-bit protected mode.

	mov	ax,#0x0001	! protected mode (PE) bit		/*保护模式比特位(PE)*/
	lmsw	ax		! This is it!				/加载机器状态字*/
/*
 * 段间跳转
 * 	用于刷新CPU当前指令队列会跳到head.s程序开始处继续执行下去
 * 	8:段选择符,指定所需要的描述符项
 * 	0:代码段内的偏移值
 * */
	jmpi	0,8		! jmp offset 0 of segment 8 (cs)

! This routine checks that the keyboard command queue is empty
! No timeout is used - if this hangs there is something wrong with
! the machine, and we probably couldn't proceed anyway.

/*只有在输入缓冲器为空时(测试位1 = 0),才可以对其写入命令*/
empty_8042:
	.word	0x00eb,0x00eb
	in	al,#0x64	! 8042 status port		/*读AT键盘控制器状态寄存器*/
	test	al,#2		! is input buffer full?		/*测试位1,输入缓冲器是否满*/
	jnz	empty_8042	! yes - loop
	ret


/*全局描述符表开始处*/
gdt:
	.word	0,0,0,0		! dummy		/*第一个描述符,不用*/

/*在GDT表中,这里的偏移量是0x08.内核代码段选择符的值*/
	.word	0x07FF		! 8Mb - limit=2047 (2048*4096=8Mb)
	.word	0x0000		! base address=0
	.word	0x9A00		! code read/exec
	.word	0x00C0		! granularity=4096, 386

/*在GDT表中,这里的偏移量是0x10.内核数据段选择符的值*/
	.word	0x07FF		! 8Mb - limit=2047 (2048*4096=8Mb)
	.word	0x0000		! base address=0
	.word	0x9200		! data read/write
	.word	0x00C0		! granularity=4096, 386


/*
 * 加载中断描述符表
 * CPU要求在进入保护模式之前要设置IDT表,这里就先设置一个长度为0的空表
 * */
idt_48:
	.word	0			! idt limit=0
	.word	0,0			! idt base=0L

/*加载全局描述符*/
gdt_48:
	.word	0x800		! gdt limit=2048, 256 GDT entries
	.word	512+gdt,0x9	! gdt base = 0X9xxxx
	
.text
endtext:
.data
enddata:
.bss
endbss:
