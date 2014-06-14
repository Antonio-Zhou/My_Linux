/*
 * 包含一个显示内核错误信息并停机的函数painc()
 * */

/*
 *  linux/kernel/panic.c
 *
 *  (C) 1991  Linus Torvalds
 */

/*
 * This function is used through-out the kernel (includeinh mm and fs)
 * to indicate a major problem.
 */
#include <linux/kernel.h>
#include <linux/sched.h>

void sys_sync(void);	/* it's really int */

/*
 * 功能: 显示内核中出现重大错误信息,并运行文件系统同步函数,然后进去死循环(死机)
 * 参数: 要显示的错误信息
 * 返回值: 无
 * volatile用于告诉编译器gcc该函数不会返回,可以让gcc产生更好的一些的代码
 * */
volatile void panic(const char * s)
{
	printk("Kernel panic: %s\n\r",s);
	/*果当前任务是任务0的话,还说明是交换任务出错,并且还没有运行文件系统同步函数*/
	if (current == task[0])
		printk("In swapper task - not syncing\n\r");
	else
		sys_sync();
	for(;;);
}
