/**/

/*
 * linux/kernel/math/math_emulate.c
 *
 * (C) 1991 Linus Torvalds
 */

/*
 * This directory should contain the math-emulation code.
 * Currently only results in a signal.
 */

#include <signal.h>

#include <linux/sched.h>
#include <linux/kernel.h>
#include <asm/segment.h>

/*
 * 协处理器仿真函数
 * 参数esp和ss的位置应该互换,因为在堆栈中,ss比esp先入栈
 * */
void math_emulate(long edi, long esi, long ebp, long sys_call_ret,
	long eax,long ebx,long ecx,long edx,
	unsigned short fs,unsigned short es,unsigned short ds,
	unsigned long eip,unsigned short cs,unsigned long eflags,
	unsigned short ss, unsigned long esp)
{
	unsigned char first, second;

/* 0x0007 means user code space */
	/*
	 * cs为什么存放着段选择符显示
	 * cs != 0x000F-->cs一定是内核代码选择符-->出错,并显示此时的cs:eip,并"内核中需要数学仿真",然后进入死机状态
	 * */
	if (cs != 0x000F) {
		printk("math_emulate: %04x:%08x\n\r",cs,eip);
		panic("Math emulation needed in kernel");
	}
	/*
	 * 取用户进程进入中断时当前代码指针cs:eip处的两个字节机器码first和second,是这个指令引发了本中断.
	 * 然后显示进程的cs和eip这两个字节,并给进程设置浮点异常信号SIGFPE
	 * */
	first = get_fs_byte((char *)((*&eip)++));
	second = get_fs_byte((char *)((*&eip)++));
	printk("%04x:%08x %02x %02x\n\r",cs,eip-2,first,second);
	current->signal |= 1<<(SIGFPE-1);
}

void math_error(void)
{
	__asm__("fnclex");
	if (last_task_used_math)
		last_task_used_math->signal |= 1<<(SIGFPE-1);
}
