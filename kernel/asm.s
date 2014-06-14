/*
 * 用于处理系统硬件异常引起的中断,
 * 在中断处理过程中,将调用trap.c中相应的C语言处理函数
 * */

/*
 *  linux/kernel/asm.s
 *
 *  (C) 1991  Linus Torvalds
 */

/*
 * asm.s contains the low-level code for most hardware faults.
 * page_exception is handled by the mm, so that isn't here. This
 * file also handles (hopefully) fpu-exceptions due to TS-bit, as
 * the fpu must be properly saved/resored. This hasn't been tested.
 */

/*
 * 这里主要是对Intel保留中断int0--int16的处理,(int17--int31留在以后再用)
 * 一下是全局函数名的声明,原型在traps.c中
 * */

.globl _divide_error,_debug,_nmi,_int3,_overflow,_bounds,_invalid_op
.globl _double_fault,_coprocessor_segment_overrun
.globl _invalid_TSS,_segment_not_present,_stack_segment
.globl _general_protection,_coprocessor_error,_irq13,_reserved

/*
 * 处理无出错号的情况:
 * int0-->处理被零除出错的情况.类型:错误,错误号:无.在执行DIV或IDIV时,若除数是0,CPU 就会产生这个异常.
 * 当EAX(或AX,AL)容纳不了一个合法除操作的结果时也会产生这个异常.
 * _do_divide_error实际上是C函数do_divide_error()编译后生成模块中对应的名称
 * */
_divide_error:
	pushl $_do_divide_error		/*首先将要调用的函数地址入栈*/
no_error_code:				/*无出错处理入口处*/
	xchgl %eax,(%esp)		/*_do_divide_error的地址-->eax  eax被交换入栈*/
	pushl %ebx
	pushl %ecx
	pushl %edx
	pushl %edi
	pushl %esi
	pushl %ebp
	push %ds			/*16位段寄存器入栈后也要占用4个字节*/
	push %es
	push %fs
	pushl $0			# "error code"		/*将数值0作为出错码入栈*/
	lea 44(%esp),%edx		/*取堆栈中原调用返回地址处堆栈指针位置,并压入堆栈*/
	pushl %edx
	movl $0x10,%edx			/*初始化段寄存器ds,es和fs,加载内核数据段选择符*/
	mov %dx,%ds
	mov %dx,%es
	mov %dx,%fs
	/*
	 * "*":调用操作数指定地址的函数-->间接调用
	 * 调用引起本次异常的C处理函数
	 * */
	call *%eax
	/*
	 * 将堆栈指针加8,相当于执行两次pop操作,弹出最后入堆栈的两个C函数参数
	 * 让堆栈指针重新指向寄存器fs入栈处
	 * */
	addl $8,%esp
	pop %fs
	pop %es
	pop %ds
	popl %ebp
	popl %esi
	popl %edi
	popl %edx
	popl %ecx
	popl %ebx
	popl %eax
	iret

/*
 * int1-->debug调试中断入口点,处理过程同上
 * 类型:错误/陷阱,错误号:无
 * 产生中断的条件:
 * 1.当EFLAGS中TF标志置位时而引发的中断;
 * 2.当发现硬件断点,或者开启了指令跟踪陷阱或任务交换陷阱;
 * 3.或者调试寄存器访问无效CPU就会产生该中断
 * */
_debug:
	pushl $_do_int3		# _do_debug    /*C函数指针入栈*/
	jmp no_error_code

/*
 * int2-->非屏蔽中断调用入口点
 * 类型:陷阱,无错误号
 * 这里仅有的被赋予固定中断向量的硬件中断每当接收到一个nmi信号,CPU 内部就会产生中断向量2,并执行标准中断应答周期,因此很节省时间
 * NMI通常保留为极为重要的硬件事件使用,
 * 当CPU收到一个NMI信号并开始执行其中断处理过程时,随后所有硬件中断都将被忽略.
 * */
_nmi:
	pushl $_do_nmi
	jmp no_error_code

/*
 * int3-->断点指令引起中断的入口点
 * 类型:陷阱,无错误号
 * 由int 3指令引发的中断,与硬件中断无关,该指令通常由调试器插入被调试程序的代码中
 * 处理过程同_debug
 * */
_int3:
/*
 * 采用JMP指令来调用C函数把我们要执行的下一步指令压入栈中
 * 然后直接用JMP命令跳转,完成后会执行ret命令把压入的指令弹出
 * */
	pushl $_do_int3
	jmp no_error_code

/*
 * int4-->溢出出错处理中断入口点
 * 类型:陷阱,无错误号
 * EFLAGS中OF标志置位时CPU执行INTO指令就会引发该中断
 * 通常用于编译器跟踪算术计算溢出
 * */
_overflow:
	pushl $_do_overflow
	jmp no_error_code

/*
 * int5-->边界检查出错中断入口点
 * 类型:错误,无错误号
 * 3个操作数,如果第一个不在另外两个之间,就产生异常
 * */
_bounds:
	pushl $_do_bounds
	jmp no_error_code

/*
 * int6-->无效操作指令出错中断入口点
 * 类型:错误,无出错号
 * CPU执行机构检测到一个无效的操作码而引起的中断
 * */
_invalid_op:
	pushl $_do_invalid_op
	jmp no_error_code

/*
 * int9-->协处理器超出出错中断入口点
 * 类型:放弃,无错误号
 * 该异常基本等同于协处理器出错保护
 * 因为在浮点指令操作数太大时,我们就有这个机会加载或保存超出数据段的浮点值
 * */
_coprocessor_segment_overrun:
	pushl $_do_coprocessor_segment_overrun
	jmp no_error_code

/*int15-->其他Intel 保留中断的入口点*/
_reserved:
	pushl $_do_reserved
	jmp no_error_code

/*
 * int--(=0x20+13) Linux设置的数学协处理器硬件中断
 * 当协处理器执行完一个操作时就会发出IRQ13中断信号,以通知CPU操作完成
 * */
_irq13:
	pushl %eax
	xorb %al,%al
	outb %al,$0xF0
	movb $0x20,%al
	outb %al,$0x20		/*向8259主中断控制芯片发送EOI(中断结束)信号*/
	jmp 1f			/*两个跳转:延迟*/
1:	jmp 1f
1:	outb %al,$0xA0		/*向8259从中断控制芯片发送EOI(中断结束)信号*/
	popl %eax
	jmp _coprocessor_error

/*以下中断有错误号*/

/*
 * int8 -->双出错故障
 * 类型:放弃,有错误码
 * 通常当CPU在调用前一个异常处理程序而又检测到一个新的异常时,这两个异常会被串行的进行处理,
 * 但也会碰到很少的情况,CPU不能进行这样的串行处理操作,此时会引起该中断
 * */
_double_fault:
	pushl $_do_double_fault		/*C函数地址入栈*/
error_code:
	xchgl %eax,4(%esp)		# error code <-> %eax		/*eax原来的值被保存在堆栈上*/
	xchgl %ebx,(%esp)		# &function <-> %ebx		/*ebx原来的值被保存在堆栈上*/
	pushl %ecx
	pushl %edx
	pushl %edi
	pushl %esi
	pushl %ebp
	push %ds
	push %es
	push %fs
	pushl %eax			# error code			/*出错号入栈*/
	lea 44(%esp),%eax		# offset			/*程序返回地址处堆栈指针位置值入栈*/
	pushl %eax
	movl $0x10,%eax			/*置内核数据段选择符*/
	mov %ax,%ds
	mov %ax,%es
	mov %ax,%fs
	call *%ebx			/*间接调用,调用相应的C函数,其参数已入栈*/
	addl $8,%esp			/*丢弃入栈的2个用作C函数的参数*/
	pop %fs
	pop %es
	pop %ds
	popl %ebp
	popl %esi
	popl %edi
	popl %edx
	popl %ecx
	popl %ebx
	popl %eax
	iret

/*
 * int10-->无效的任务状态段(TSS)
 * 类型:错误,有出错码
 * CPU企图切换到一个进程,而该进程的TSS无效,根据TSS 中哪一部分引起了异常,
 * 当由于TSS长度超过104字节时,这个异常在当前任务中产生,因而切换被终止,其他问题则会导致在切换后的新任务中产生本异常
 * */
_invalid_TSS:
	pushl $_do_invalid_TSS
	jmp error_code

/*
 * int11-->段不存在
 * 类型:错误,有出错码
 * 被引用的段不在内存中,段描述符中标志着段不在内存中
 * */
_segment_not_present:
	pushl $_do_segment_not_present
	jmp error_code

/*
 * int12-->堆栈段错误
 * 类型:错误,有出错码
 * 指令操作试图超出堆栈段范围,或者堆栈段不在内存中,这是异常11和13 的特例,
 * 有些操作系统可以利用这个异常来确定什么时候应该为程序分配更多的栈空间
 * */
_stack_segment:
	pushl $_do_stack_segment
	jmp error_code

/*
 * int13-->一般保护性出错
 * 类型:  错误,有出错码
 * 表明是不属于人和我其他类的错误,若一个异常产生时没有对应的处理向量(0-16),通常就会归到这类
 * */
_general_protection:
	pushl $_do_general_protection
	jmp error_code

/*int7-->设备不存在*/
/*int14-->页错误*/
/*int16-->协处理器错误*/
/*时钟中断int 0x20*/
/*系统调用int 0x80*/
