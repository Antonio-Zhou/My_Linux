/*
 * 实现Linux系统调用(int 0x80)的接口处理过程
 * 实际处理过程包含在各系统调用相应的c语言处理函数中,这些处理函数分布在整个Linux内核代码中
 * */

/*
 *  linux/kernel/system_call.s
 *
 *  (C) 1991  Linus Torvalds
 */

/*处理系统调用中断int  0x80  的过程*/

/*
 *  system_call.s  contains the system-call low-level handling routines.
 * This also contains the timer-interrupt handler, as some of the code is
 * the same. The hd- and flopppy-interrupts are also here.
 *
 * NOTE: This code handles signal-recognition, which happens every time
 * after a timer-interrupt and after each system call. Ordinary interrupts
 * don't handle signal-recognition, as that would clutter them up totally
 * unnecessarily.
 *
 * Stack layout in 'ret_from_system_call':
 *
 *	 0(%esp) - %eax
 *	 4(%esp) - %ebx
 *	 8(%esp) - %ecx
 *	 C(%esp) - %edx
 *	10(%esp) - %fs
 *	14(%esp) - %es
 *	18(%esp) - %ds
 *	1C(%esp) - %eip
 *	20(%esp) - %cs
 *	24(%esp) - %eflags
 *	28(%esp) - %oldesp
 *	2C(%esp) - %oldss
 */

SIG_CHLD	= 17			/*定义SIG_CHLD信号,子进程结束或终止*/

EAX		= 0x00			/*堆栈中各个寄存器的偏移位置*/
EBX		= 0x04
ECX		= 0x08
EDX		= 0x0C
FS		= 0x10
ES		= 0x14
DS		= 0x18
EIP		= 0x1C
CS		= 0x20
EFLAGS		= 0x24
OLDESP		= 0x28			/*当特权级变化时栈会切换,用户栈指针被保存在内核态栈中*/
OLDSS		= 0x2C

/*下面是任务结构(task-struct)中变量的偏移值*/
state	= 0		# these are offsets into the task-struct.	/*进程状态码*/
counter	= 4				/*任务运行时间计数(递减)，运行时间片*/
priority = 8				/*运行优先数,任务开始运行时counter=priority,越大则运行时间越长*/
signal	= 12				/*信号位图,每个比特位代表一种信号,信号值=位偏移量+1*/
sigaction = 16		# MUST be 16 (=len of sigaction)		/*sigaction结构长度必须是16字节*/
blocked = (33*16)			/*受阻塞信号位图的偏移量*/

# offsets within sigaction
/*sigaction结构中为偏移量*/
sa_handler = 0				/*信号处理过程的句柄(描述符)*/
sa_mask = 4				/*信号屏蔽码*/
sa_flags = 8				/*信号集*/
sa_restorer = 12			/*恢复函数指针*/

nr_system_calls = 72			/*0.11版内核中系统调用的总数*/

/*
 * Ok, I get parallel printer interrupts while using the floppy for some
 * strange reason. Urgel. Now I just ignore them.
 */

/*定义入口点*/
.globl _system_call,_sys_fork,_timer_interrupt,_sys_execve
.globl _hd_interrupt,_floppy_interrupt,_parallel_interrupt
.globl _device_not_available, _coprocessor_error

/*错误的系统调用号*/
.align 2				/*内存4字节对齐*/
bad_sys_call:
	movl $-1,%eax			/*eax中置-1,退出中断*/
	iret

/*重新执行调度程序入口,当调度程序schedule()返回时,就从ret_from_sys_call处继续执行*/
.align 2
reschedule:
	pushl $ret_from_sys_call	/*将ret_from_sys_call的地址入栈*/
	jmp _schedule

/*int 0x80 -->linux系统调用入口点(调用中断int 0x80,eax中是调用号)*/
.align 2
_system_call:
	cmpl $nr_system_calls-1,%eax	/*调用号如果超出范围就在eax中置-1并退出*/
	ja bad_sys_call
	push %ds			/*保存原寄存器的内容*/
	push %es
	push %fs

/*
 * 一个系统调用最多有3个参数,也可以不带参数,下面入栈的ebx,ecx,edx存放着系统调用相应C函数的调用参数
 * 这几个寄存器入栈的顺序由GNU GCC规定的ebx中可存放第一个参数,ecx中放第二个,edx中放第三个
 * */
	pushl %edx
	pushl %ecx		# push %ebx,%ecx,%edx as parameters
	pushl %ebx		# to the system call
	movl $0x10,%edx		# set up ds,es to kernel space
	mov %dx,%ds		/*ds,es指向内核数据段(全局描述符表中数据段描述符)*/
	mov %dx,%es

/*
 * fs指向局部数据段(局部描述符表中数据段描述符),即指向执行本次系统调用的用户程序的数据段
 * 注意:在linux 0.11中内核给任务分配的代码和数据内存段是重叠的,它们的段基址和段限长相同
 * */
	movl $0x17,%edx		# fs points to local data space
	mov %dx,%fs

/*
 * 调用地址=[call _sys_call_table + %eax*4]
 * call _sys_call_table[]指针数组中设置了所有72个系统调用C处理函数的地址
 * 根据sys_call_table[] 函数指针表调用执行相应的系统调用处理程序间接调用指定功能的C函数
 * */
	call _sys_call_table(,%eax,4)
	pushl %eax			/*把系统调用返回值入栈*/

/*
 * 查看当前任务的状态
 * 执行调度程序的情况:
 * 1.不是就绪(state != 0);
 * 2.是就绪状态,但时间片已经用完(counter == 0)
 * */
	movl _current,%eax		/*取当前任务(进程)数据结构地址-->eax*/
	cmpl $0,state(%eax)		# state
	jne reschedule
	cmpl $0,counter(%eax)		# counter
	je reschedule

/*
 * 执行从系统调用C函数返回后,对信号进行识别处理
 * 其他中断服务程序退出时也将跳转到这里进行处理后才退出中断过程
 * */
ret_from_sys_call:

	/*判断当前任务是否是初始任务task0,是就不必进行信号量方面的处理,直接返回*/
	movl _current,%eax		# task[0] cannot have signals
	cmpl _task,%eax
	je 3f				/*向前(forward)跳转到标号3处退出中断处理*/

/*
 * 通过对原调用程序代码选择符的检查老判断调用程序是否是用户任务:
 * 不是就直接退出中断,因为任务在内核态执行时不可抢占;否则对任务进行信号量的识别处理.
 * 比较选择符是否为用户代码段的选择符0x000f来判断是否为用户任务:
 * 不是则说明是某个中断服务程序r,退出中断程序
 * 如果原堆栈段选择符不是0x17(原堆栈段不在用户段中),也说明本次系统调用不是用户任务,也退出
 * */
	cmpw $0x0f,CS(%esp)		# was old code segment supervisor ?
	jne 3f
	cmpw $0x17,OLDSS(%esp)		# was stack segment = 0x17 ?
	jne 3f

/*
 * 处理当前任务中的信号:
 *	1.取当前任务中的信号位图(32位,每位代表1种信号);
 * 	2.用任务结构中的信号阻塞码,阻塞不允许的信息位,取得数值最小的信号值;
 *	3.把原信号位图中该信号对应的位复位(置0);
 *	4.将该信号值作为参数之一调用_do_signal().
 * */
	movl signal(%eax),%ebx		/*取信号位图--->ebx, singal():这是什么??*/
	movl blocked(%eax),%ecx		/*取阻塞信号位图--->ecx*/
	notl %ecx			/*每位取反??*/
	andl %ebx,%ecx			/*获得许可的信号位图*/
	/*
	 * 从低位开始扫描位图,看是否有1的位
	 * 屏蔽位图中1-->屏蔽.0-->通过
	 * 信号位图中1-->有信号
	 */
	bsfl %ecx,%ecx
	je 3f				/*没有信号就向前跳转*/
	btrl %ecx,%ebx			/*复位该信号(ebx中含有原signal原图)*/
	movl %ebx,signal(%eax)		/*重新保存signal位图信息current--->signal*/
	incl %ecx			/*将信号调整为从1开始的数*/
	pushl %ecx			/*信号值入栈作为调用do_signal 的参数之一*/
	call _do_signal			/*调用信号处理程序*/
	popl %eax
3:	popl %eax
	popl %ebx
	popl %ecx
	popl %edx
	pop %fs
	pop %es
	pop %ds
	iret

/*
 * int16-->处理器错误中断
 * 类型: 错误,无错误码
 * 这是一个外部的基于硬件的异常,当协处理器检测到自己发生错误时,就会通过ERROR引脚通知CPU
 * 下面是处理协处理器发出的出错信号,并跳转去执行C函数math_error(),返回后跳转到标号ret_from_sys_call处继续执行
 * */
.align 2
_coprocessor_error:				/*为什么这里压栈的顺序与asm.s中不同*/
	push %ds
	push %es
	push %fs
	pushl %edx
	pushl %ecx
	pushl %ebx
	pushl %eax
	movl $0x10,%eax				/*内核数据段*/
	mov %ax,%ds
	mov %ax,%es
	movl $0x17,%eax				/*fs置为指向局部数据段(出错程序的数据段)*/
	mov %ax,%fs
	pushl $ret_from_sys_call		/*把下面调用返回的地址入栈*/
	jmp _math_error

/*
 * int7 -->设备不存在或协处理器不存在
 * 类型: 错误,无错误码
 * 如果控制寄存器CR0中EM(模拟)标志置位,则当CPU执行一个协处理器指令时就会引发该中断
 * 这样CPU就可以有机会让这个中断处理程序模拟协处理器指令CR0的交换标志TS是在CPU执行任务转换时设置的
 * TS可以用来确定什么时候协处理器中的内容与CPU正在执行的任务不匹配了.当CPU在运行一个协处理器转义指令时发现TS置位时就会引发该中断
 * 此时就可以保存前一个任务的协处理器内容,并恢复新任务的协处理器执行状态
 * */
.align 2
_device_not_available:
	push %ds
	push %es
	push %fs
	pushl %edx
	pushl %ecx
	pushl %ebx
	pushl %eax
	movl $0x10,%eax			/*内核数据段*/
	mov %ax,%ds
	mov %ax,%es
	movl $0x17,%eax			/*fs置为指向局部数据段(出错程序的数据段)*/
	mov %ax,%fs

/*
 * 清CR0中任务已交换标志TS,并取CR0值。
 * 若EM没有置位,说明不是EM引起的中断,则恢复任务协处理器状态,执行c函数math_state_restore()，
 * 并在返回时去执行ret_from_sys_call处的代码
 * */
	pushl $ret_from_sys_call
	clts				# clear TS so that we can use math	/*清除任务标志*/
	movl %cr0,%eax
	testl $0x4,%eax			# EM (math emulation bit)		/*检测CR0的位2*/
	je _math_state_restore			/*EM没有置位*/
	/*EM置位,执行数学仿真程序math_emulate()*/
	pushl %ebp
	pushl %esi
	pushl %edi
	call _math_emulate
	popl %edi
	popl %esi
	popl %ebp
	ret

/*
 * int32-->(intx20)时钟中断处理程序
 * 中断频率设置为100Hz,这里jiffies每10毫秒加1
 * 这段代码将jiffies增1,发送结束中断指令给8259控制器,利用当前特权级作为参数调用do_timer(long CPL),当做调用返回时转去检测并处理信号
 * */
	
.align 2
_timer_interrupt:
	push %ds		# save ds,es and put kernel data space
	push %es		# into them. %fs is used by _system_call
	push %fs
	pushl %edx		# we save %eax,%ecx,%edx as gcc doesn't
	pushl %ecx		# save those across function calls. %ebx
	pushl %ebx		# is saved as we use that in ret_sys_call
	pushl %eax
	movl $0x10,%eax
	mov %ax,%ds
	mov %ax,%es
	movl $0x17,%eax
	mov %ax,%fs
	incl _jiffies
	/*由于初始化中断控制芯片没有采用自动EOI,所以这里要发指令结束该硬件中断*/
	movb $0x20,%al		# EOI to interrupt controller #1		/*OCW2位5置1,即EOI置1*/
	outb %al,$0x20		/*操作命令字OCW2送0x20端口*/
	/*从堆栈中取出执行系统调用代码的选择符(CS段寄存器的值)中的当前特权级别并压入堆栈,作为do_timer的参数*/
	movl CS(%esp),%eax	/*还不是很明白*/
	andl $3,%eax		# %eax is CPL (0 or 3, 0=supervisor)
	pushl %eax
	call _do_timer		# 'do_timer(long CPL)' does everything from
	addl $4,%esp		# task switching to accounting ...
	jmp ret_from_sys_call

/*
 * _sys_execve()系统调用
 * 取中断调用程序的代码指针作为参数调用C函数do_execve()
 * */
.align 2
_sys_execve:
	lea EIP(%esp),%eax		/*eax指向堆栈中保存用户程序eip 指针处(EIP+%esp)*/
	pushl %eax
	call _do_execve
	addl $4,%esp			/*丢弃调用时压入栈的eip*/
	ret

/*_sys_fork函数调用,用于创建子进程*/
.align 2
_sys_fork:
	call _find_empty_process	/*调用find_empty_process(),在eax中返回进程号pid,若返回负数则退出*/
	testl %eax,%eax
	js 1f
	push %gs
	pushl %esi
	pushl %edi
	pushl %ebp
	pushl %eax			/*参数nr*/
/*
 * 上面压入栈的是copy_process()函数的最左面的5个参数,随后的其余参数实际上上直接对应堆栈上以后的内容
 * 这些内容是系统调用中断处理过程开始,直到调用本系统调用处理过程时逐步入栈的各寄存器的值
 * */
	call _copy_process
	addl $20,%esp
1:	ret

/*
 * int46-->(int 0x2E)硬盘中断处理程序,响应硬件中断IRQ14
 * 当请求的硬盘操作完成或出错会发出此中断信号
 * */
_hd_interrupt:
	pushl %eax
	pushl %ecx
	pushl %edx
	push %ds
	push %es
	push %fs
	movl $0x10,%eax
	mov %ax,%ds
	mov %ax,%es
	movl $0x17,%eax
	mov %ax,%fs
	movb $0x20,%al
	outb %al,$0xA0		# EOI to interrupt controller #1	/*送从8259A*/
	jmp 1f			# give port chance to breathe		/*jmp起延时作用*/
1:	jmp 1f
/*
 * do_hd被定义成为一个函数指针,将被赋值read_intr或write_intr函数地址
 * 放到edx寄存器后就将do_hd指针变量置为NULL。然后测试得到的函数指针
 * 若指针为空,则赋予该指针指向C函数unexpected_hd_interrupt(),处理未知硬盘中断
 * */
1:	xorl %edx,%edx
	xchgl _do_hd,%edx
	testl %edx,%edx				/*测试函数指针是不是NULL*/
	jne 1f					/*若空,使函数指针指向unexpected_hd_interrupt()*/
	movl $_unexpected_hd_interrupt,%edx
1:	outb %al,$0x20
	call *%edx		# "interesting" way of handling intr.	/*调用do_hd指向的C函数*/
	pop %fs
	pop %es
	pop %ds
	popl %edx
	popl %ecx
	popl %eax
	iret

/*
 * int38-->(int 0x26) 软盘驱动器中断处理程序,响应硬件中断请求IRQ6
 * 其处理过程与上面对硬盘的处理基本一样
 * */
_floppy_interrupt:
	pushl %eax
	pushl %ecx
	pushl %edx
	push %ds
	push %es
	push %fs
	movl $0x10,%eax
	mov %ax,%ds
	mov %ax,%es
	movl $0x17,%eax
	mov %ax,%fs
	movb $0x20,%al
	outb %al,$0x20		# EOI to interrupt controller #1	/*有一点还是不明白,为什么是在这里发送EOI命令*/

/*
 * do_floppy被定义成为一个函数指针,将被赋值实际处理C函数指针放到edx寄存器后就将do_floppy指针变量置为NULL
 * 然后测试得到的函数指针:
 * 	若指针为空,则赋予该指针指向C函数unexpected_floppy_interrupt()
 * */
	xorl %eax,%eax
	xchgl _do_floppy,%eax
	testl %eax,%eax				/*测试是否为空*/
	jne 1f					/*若空,则使指针指向unexpected_floppy_interrupt()*/
	movl $_unexpected_floppy_interrupt,%eax
1:	call *%eax		# "interesting" way of handling intr.
	pop %fs
	pop %es
	pop %ds
	popl %edx
	popl %ecx
	popl %eax
	iret

/*
 * int39-->(int 0x27)并行口中断处理程序,对应硬件中断请求信号IRQ7
 * 未实现
 * */
_parallel_interrupt:
	pushl %eax
	movb $0x20,%al
	outb %al,$0x20
	popl %eax
	iret
