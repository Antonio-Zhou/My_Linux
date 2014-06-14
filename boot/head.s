/*
 * 被编译连接在system模块的最前部分,主要进行硬件设备的探测设置和内存管理页面的初始化设置工作
 * */

/*
 *  linux/boot/head.s
 *
 *  (C) 1991  Linus Torvalds
 */

/*
 *  head.s contains the 32-bit startup code.
 *
 * NOTE!!! Startup happens at absolute address 0x00000000, which is also where
 * the page directory will exist. The startup code will be overwritten by
 * the page directory.
 */
.text
.globl _idt,_gdt,_pg_dir,_tmp_floppy_area
_pg_dir:							/*页目录将会被放在这里*/
startup_32:
/*
 * 在32位运行模式下,'$0x10'是全局段描述符表中的偏移值,更准确的是一个描述符表项的选择符
 * 指向了setup.s中的内核数据段描述符
 * */
	movl $0x10,%eax
	mov %ax,%ds			/*设置各个数据段寄存器*/
	mov %ax,%es
	mov %ax,%fs
	mov %ax,%gs
	lss _stack_start,%esp		/*_stack_start---> ss:esp，设置系统堆栈*/
	call setup_idt			/*调用设置中断描述符表子程序*/
	call setup_gdt			/*调用设置全局描述符表子程序*/
	movl $0x10,%eax		# reload all the segment registers
	mov %ax,%ds		# after changing gdt. CS was already
	mov %ax,%es		# reloaded in 'setup_gdt'
	mov %ax,%fs
	mov %ax,%gs
	/*esp  指针指向user_stack数组的顶端*/
	lss _stack_start,%esp

	/*测试A20 地址是否已经开启*/
	xorl %eax,%eax
1:	incl %eax		# check that A20 really IS enabled
	movl %eax,0x000000	# loop forever if it isn't
	cmpl %eax,0x100000
/*
 * 相等就一直循环
 *	1b : 表示向后跳转到标号1去(43)
 * 	5f则表示向前跳转到标号5
 × */
	je 1b
/*
 * NOTE! 486 should set bit 16, to check for write-protect in supervisor
 * mode. Then it would be unnecessary with the "verify_area()"-calls.
 * 486 users probably want to set the NE (#5) bit also, so as to use
 * int 16 for math errors.
 */

 /*检查数学协处理器芯片是否存在*/
	movl %cr0,%eax		# check math chip
	andl $0x80000011,%eax	# Save PG,PE,ET
/* "orl $0x10020,%eax" here for 486 might be good */
	orl $2,%eax		# set MP
	movl %eax,%cr0
	call check_x87
	jmp after_page_tables

/*
 * We depend on ET to be correct. This checks for 287/387.
 */
check_x87:
	fninit			/*想协处理器发出初始化命令*/
	fstsw %ax		/*取协处理器状态字到ax中*/
	cmpb $0,%al		/*状态字应该为0,否则不存在*/
	je 1f			/* no coprocessor: have to set bits */
	movl %cr0,%eax		/*存在跳转到1处,否则改写为cr0*/
	xorl $6,%eax		/* reset MP, set EM */
	movl %eax,%cr0
	ret

/*
 * 存储边界对齐调整
 *	2-->把随后的代码或数据的偏移地址调整到地址值最后2比特为0的位置,即按4字节方式对齐内存
 *	为了提高32位CPU访问内存中代码或数据的速度和效率
 * */
.align 2
1:	.byte 0xDB,0xE4		/* fsetpm for 287, ignored by 387 */
	ret

/*
 *  setup_idt
 *
 *  sets up a idt with 256 entries pointing to
 *  ignore_int, interrupt gates. It then loads
 *  idt. Everything that wants to install itself
 *  in the idt-table may do so themselves. Interrupts
 *  are enabled elsewhere, when we can be relatively
 *  sure everything is ok. This routine will be over-
 *  written by the page tables.
 */

/*对所有256个描述符进行了默认描述设置*/
setup_idt:
	lea ignore_int,%edx		/*将ignore_int  的有效地址(偏移值)值-->edx寄存器*/
	movl $0x00080000,%eax		/*将选择符0x0008置入eax的高16位中*/
	movw %dx,%ax		/* selector = 0x0008 = cs */
					/*偏移值的低16位置入eax的低16位.此时eax含有门描述符低4字节值*/
	movw $0x8E00,%dx	/* interrupt gate - dpl=0, present */
					/*edx含有门描述符高4字节的值*/

	lea _idt,%edi			/*_idt是中段描述符表的地址*/
	mov $256,%ecx
rp_sidt:
	movl %eax,(%edi)		/*将哑中断门描述存入表中*/
	movl %edx,4(%edi)·		/*eax内容放到edi + 4所指的位置*/
	addl $8,%edi			/*edi指向表的下一项*/
	dec %ecx
	jne rp_sidt
	lidt idt_descr			/*加载中断描述符表寄存器值*/
	ret

/*
 *  setup_gdt
 *
 *  This routines sets up a new gdt and loads it.
 *  Only two entries are currently built, the same
 *  ones that were built in init.s. The routine
 *  is VERY complicated at two whole lines, so this
 *  rather long comment is certainly needed :-).
 *  This routine will beoverwritten by the page tables.
 */
setup_gdt:
	lgdt gdt_descr			/*加载全局描述符表寄存器值*/
	ret

/*
 * I put the kernel page tables right after the page directory,
 * using 4 of them to span 16 Mb of physical memory. People with
 * more than 16MB will have to expand this.
 */
.org 0x1000		/*从偏移地址0x1000处开始是第一个页表(偏移0开始处将存放页表目录)*/
pg0:

.org 0x2000
pg1:

.org 0x3000
pg2:

.org 0x4000
pg3:

.org 0x5000		/*定义下面的内存数据块从偏移0x5000处开始*/
/*
 * tmp_floppy_area is used by the floppy-driver when DMA cannot
 * reach to a buffer-block. It needs to be aligned, so that it isn't
 * on a 64kB border.
 */
_tmp_floppy_area:
	.fill 1024,1,0		/*共保留1024项,每项1字节,填充数据0*/

/*用于跳转到init/main.c中的main()函数做准备工作*/
after_page_tables:
	pushl $0		# These are the parameters to main :-)	/*envp*/
	pushl $0		/*argv*/
	pushl $0		/*argc*/
	pushl $L6		# return address for main, if it decides to.
	/*压入了main() 函数代码的地址 _main是编译程序内部对main的表示方法*/
	pushl $_main

	jmp setup_paging
/*main 函数真的退出时,会返回到这里.死循环*/
L6:
	jmp L6			# main should never return here, but
				# just in case, we know what happens.

/*哑中断向量默认指向无中断处理过程*/

/* This is the default interrupt "handler" :-) */
int_msg:
	.asciz "Unknown interrupt\n\r"			/*定义字符串--未知中断(回车换行)*/
.align 2						/*按四字节方式对齐内存地址*/
ignore_int:
	pushl %eax
	pushl %ecx
	pushl %edx
	push %ds		/*ds,es,fs,gs虽是16位寄存器,但是入栈后还是会以32位形式存在要4个字节的堆栈空间*/
	push %es
	push %fs
	movl $0x10,%eax		/*置段选择符(使ds,es,fs指向gdt表中的数据段)*/
	mov %ax,%ds
	mov %ax,%es
	mov %ax,%fs
	pushl $int_msg		/*把调用printk函数的参数指针(地址)入栈,若不加'$',则表示int_msg符号处的长字*/
	call _printk
	popl %eax
	pop %fs
	pop %es
	pop %ds
	popl %edx
	popl %ecx
	popl %eax
	iret			/*中断返回,把中断调用时压入栈的CPU标志寄存器(32位)值也弹出*/


/*
 * Setup_paging
 *
 * This routine sets up paging by setting the page bit
 * in cr0. The page tables are set up, identity-mapping
 * the first 16MB. The pager assumes that no illegal
 * addresses are produced (ie >4Mb on a 4Mb machine).
 *
 * NOTE! Although all physical memory should be identity
 * mapped by this routine, only the kernel page functions
 * use the >1Mb addresses directly. All "normal" functions
 * use just the lower 1Mb, or the local data space, which
 * will be mapped to some other place - mm keeps track of
 * that.
 *
 * For those with more memory than 16 Mb - tough luck. I've
 * not got it, why should you :-) The source is here. Change
 * it. (Seriously - it shouldn't be too difficult. Mostly
 * change some constants etc. I left it at 16Mb, as my machine
 * even cannot be extended past that (ok, but it was cheap :-)
 * I've tried to show which constants to change by having
 * some kind of marker at them (search for "16Mb"), but I
 * won't guarantee that's all :-( )
 */
.align 2
setup_paging:				/*首先对5页内存(1页目录+  4页页表)清零*/
	movl $1024*5,%ecx		/* 5 pages - pg_dir+4 page tables */
	xorl %eax,%eax
	xorl %edi,%edi			/* pg_dir is at 0x000 */
	cld;rep;stosl			/*eax 内容存到es:edi  所指的位置处，且edi   增4*/

/*
 * 设置页目录表中的项
 * 	$pg0+7:0x00001007是页目录表中的第一项
 * */
	movl $pg0+7,_pg_dir		/* set present bit/user r/w */
	movl $pg1+7,_pg_dir+4		/*  --------- " " --------- */
	movl $pg2+7,_pg_dir+8		/*  --------- " " --------- */
	movl $pg3+7,_pg_dir+12		/*  --------- " " --------- */

/*
 * 填写4个页表中所有项的内容,每项内容是当前所映射的物理内存地址+该页的标志(这里均为7)
 * 使用的方法是从最后一个页表的最后一项开始倒退顺序填写
 * */
	movl $pg3+4092,%edi		/*edi --> 最后一页的最后一项*/
	movl $0xfff007,%eax		/*  16Mb - 4096 + 7 (r/w user,p) */
	std				/*方向位置位,edi递减(4字节)*/
1:	stosl			/* fill pages backwards - more efficient :-) */
	subl $0x1000,%eax		/*每填好一项,物理地址减0x1000*/
	jge 1b				/*若小于0就说明已经填写好了*/

/*设置页目录表基址寄存器cr3的值,指向页目录表.cr3中保存的是页目录表的物理地址*/
	xorl %eax,%eax		/* pg_dir is at 0x0000 */
	movl %eax,%cr3		/* cr3 - page directory start */

/*设置启动使用分页处理(cr0的PG标志,位31)*/
	movl %cr0,%eax
	orl $0x80000000,%eax	/*添上PG标志*/
	movl %eax,%cr0		/* set paging (PG) bit */
/*ret指令,会弹出main()的地址,并把控制权交回到init/main.c*/
	ret			/* this also flushes prefetch-queue */

/*
 * 在改变分页处理标志后要求使用转移指令刷新预取指令队列,这里用的是ret
 * 返回该指令的另一个作用是将压入栈的main程序的地址弹出,并跳转到/init/main.c程序中去运行,本程序到此就真正结束了
 * */

.align 2
.word 0		/*空出2字节,与上面的长字节对齐*/

/*加载中断描述符寄存器*/
idt_descr:
	.word 256*8-1		# idt contains 256 entries
	.long _idt
.align 2
.word 0

/*加载全局描述符寄存器*/
gdt_descr:
	.word 256*8-1		# so does gdt (not that that's any
	.long _gdt		# magic number, but it works for me :^)

	.align 3
_idt:	.fill 256,8,0		# idt is uninitialized


/*全局表*/
_gdt:	.quad 0x0000000000000000	/* NULL descriptor */
	.quad 0x00c09a0000000fff	/* 16Mb */					/*0x08  内核代码段最大长度16MB*/
	.quad 0x00c0920000000fff	/* 16Mb */					/*0x10  内核数据段最大长度16MB*/
	.quad 0x0000000000000000	/* TEMPORARY - don't use */
	.fill 252,8,0			/* space for LDT's and TSS's etc */		/*预留空间*/
