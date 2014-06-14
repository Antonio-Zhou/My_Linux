/*
 * 包含了有关信号处理的4个系统调用
 * 以及一个在对应的中断处理程序中处理信号的函数do_signal()
 * */

/*
 *  linux/kernel/signal.c
 *
 *  (C) 1991  Linus Torvalds
 */

/*
 * 调度程序头文件,定义了任务结构task_struct,第一个初始化任务的数据.
 * 还有一些以宏形式定义的有关描述符参数设置和获取的嵌入式汇编函数程序
 * */
#include <linux/sched.h>
#include <linux/kernel.h>		/*内核头文件,含有一些内核常用函数的原型定义*/
#include <asm/segment.h>		/*段操作头文件,定义了有关段寄存器操作的嵌入式汇编函数*/

#include <signal.h>			/*信号头文件,定义信号符号常量,信号结构以及信号操作函数原型*/

/*
 * 下面函数名前的关键字volatile用于告诉编译器gcc该函数不会返回.
 * 这样可以让gcc产生更好一些的代码,更重要的是使用这个关键字可以避免产生某些(未初始化变量的)假警告信号.
 * 等同于现在gcc的函数属性说明:
 * 	void do_exit(int error_code)  _attribute_ ((noreturn));
 * */
volatile void do_exit(int error_code);

/*
 * 功能: 获取当前任务信号屏蔽位图(屏蔽码或阻塞码)
 * 参数: 无
 * 返回值: 得到的当前任务的屏蔽位图(在task_struct中)
 * */
int sys_sgetmask()
{
	return current->blocked;
}

/*
 * 功能: 设置新的信号屏蔽位图。SIGKILL不能被屏蔽
 * 参数: int newmask-->新屏蔽码
 * 返回值: 原来的屏蔽位图(为什么是原来的??)
 * */
int sys_ssetmask(int newmask)
{
	int old=current->blocked;

	current->blocked = newmask & ~(1<<(SIGKILL-1));	/*很精妙*/
	return old;
}

/*
 * 功能: 复制sigaction数据到fs数据段to处,即从内核空间复制到用户(任务数据段中)
 * 参数: char * from-->要复制的sigaction数据
 * 	 char * to-->复制到的fs数据段地址
 * 返回值: 无
 * */
static inline void save_old(char * from,char * to)
{
	int i;

	/*首先验证to处的内存空间是否足够大,然后把一个sigaction结构信息复制到fs段(用户)空间*/
	verify_area(to, sizeof(struct sigaction));
	for (i=0 ; i< sizeof(struct sigaction) ; i++) {
		put_fs_byte(*from,to);
		from++;
		to++;
	}
}

/*
 * 功能: 复制sigaction数据从fs数据段from位置复制到to处,即从用户数据空间复制到内核数据段中
 * 参数: char * from-->原地址
 * 	 char * to-->目的地址
 * 返回值: 无
 * */
static inline void get_new(char * from,char * to)
{
	int i;

	for (i=0 ; i< sizeof(struct sigaction) ; i++)
		*(to++) = get_fs_byte(from++);
}

/*
 * 功能: signal()系统调用,类似于sigaction().为指定的信号安装新的信号句柄(信号处理程序)
 * 参数: int signum-->指定的信号
 * 	 long handler-->指定的句柄
 * 	 long restorer-->恢复函数指针(该函数由Libc库提供).
 * 	 	用于在信号处理程序结束后恢复系统调用返回时几个寄存器的原有值以及系统调用的返回值,
 * 	 	就好像系统调用没有执行过信号处理程序而直接返回给用户一样
 * 返回值: 原信号句柄
 * */
int sys_signal(int signum, long handler, long restorer)
{
	struct sigaction tmp;

	/*验证信号值在(1-32)内,并且不是信号SIGKILL(和SIGSTOP).这两个信号不能同时被进程捕获*/
	if (signum<1 || signum>32 || signum==SIGKILL)
		return -1;
	/*根据参数组建sigaction结构内容*/
	tmp.sa_handler = (void (*)(int)) handler;	/*指定的信号处理句柄(函数)*/
	tmp.sa_mask = 0;				/*处理信号时的信号屏蔽码*/
	/*标志组合,这里设定该信号处理句柄只使用一次后就恢复默认值,并允许信号在自己的处理句柄中收到*/
	tmp.sa_flags = SA_ONESHOT | SA_NOMASK;
	tmp.sa_restorer = (void (*)(void)) restorer;	/*保存恢复处理函数指针*/
	/*取该信号原来的处理句柄,并设置该信号的sigaction结构*/
	handler = (long) current->sigaction[signum-1].sa_handler;
	current->sigaction[signum-1] = tmp;
	return handler;
}

/*
 * 功能: sigaction()的系统调用.改变进程收到一个信号时的操作
 * 参数: int signum-->除了SIGKILL外的任何信号
 * 	 const struct sigaction * action-->新的操作
 * 	 struct sigaction * oldaction-->原来的操作
 * 返回值: 0-->成功;1-->失败
 * */
int sys_sigaction(int signum, const struct sigaction * action,
	struct sigaction * oldaction)
{
	struct sigaction tmp;

	if (signum<1 || signum>32 || signum==SIGKILL)
		return -1;
	tmp = current->sigaction[signum-1];
	/*
	 * 不理解
	 * 把它写成current->sigaction[signum-1]不就很好理解了啊
	 * current->sigaction指向任务0,也是任务数组的第一个元素
	 * */
	get_new((char *) action,
		(char *) (signum-1+current->sigaction));
	/*oldaction指针不为空,就将原操作保存到oldaction*/
	if (oldaction)
		save_old((char *) &tmp,(char *) oldaction);
	if (current->sigaction[signum-1].sa_flags & SA_NOMASK)
		current->sigaction[signum-1].sa_mask = 0;	/*允许信号在自己的信号句柄中收到*/
	else
		/*屏蔽本信号,这里是将1右移(signum-1)位,总是看成将(signum-1)右移1位*/
		current->sigaction[signum-1].sa_mask |= (1<<(signum-1));

	return 0;
}

/*
 * 功能: 系统调用的中断处理程序中真正的信号预处理程序
 * 	 将信号处理句柄插入到用户程序堆栈中,并在本系统调用结束返回后立即执行信号句柄程序,然后继续执行用户的程序.这里还不能处理SIGSTOP等信号
 * 参数: 进入系统调用处理程序system_call.s开始直到调用本程序前压入栈的值
 * 压入顺序是参数的从右往左
 * 	1.CPU执行中断指令压入放入用户栈地址ss和esp,标志寄存器eflags和返回地址cs和eip
 * 	2.在刚进入system_call时压入栈中的寄存器ds,es,fs和edx,ecx,ebx
 * 	3.调用sys_call_table后压入栈的相应系统调用处理函数的返回值(eax)
 * 	4.压入栈中的当前处理信号值(signr)
 * 返回值: 无
 * */
void do_signal(long signr,long eax, long ebx, long ecx, long edx,
	long fs, long es, long ds,
	long eip, long cs, long eflags,
	unsigned long * esp, long ss)
{
	unsigned long sa_handler;
	long old_eip=eip;
	/*指定的信号*/
	struct sigaction * sa = current->sigaction + signr - 1;
	int longs;
	unsigned long * tmp_esp;

	sa_handler = (unsigned long) sa->sa_handler;
	/*对句柄类型的判断*/
	if (sa_handler==1)			/*SIG_IGN-->直接返回*/
		return;
	if (!sa_handler) {			/*SIG_DFL*/
		if (signr==SIGCHLD)		/*信号是SIGCHLD,直接返回*/
			return;
		else				/*终止进程执行*/
			do_exit(1<<(signr-1));	/*对1<<(signr-1)还是不理解*/
	}
	/*对调用句柄的设置*/
	if (sa->sa_flags & SA_ONESHOT)		/*信号句柄只使用一次*/
		sa->sa_handler = NULL;
	/*
	 * 在系统调用进入内核时,用户程序返回地址(eip,cs)被保存在内核态堆栈中,
	 * 修改内核态堆栈中用户调用系统调用时的代码指针eip为指向信号处理句柄,
	 * 同时也将sa_restorer,signr,进程屏蔽码,eax,ecx,edx作为参数以及原调用系统调用的程序,
	 * 返回指针及标志寄存器压入用户堆栈,因此会先执行用户的信号句柄程序,然后继续执行用户程序
	 * */
	*(&eip) = sa_handler;
	/*将原调用程序的用户堆栈指针向下扩展7(或8)个长字(存放调用信号句柄的参数)*/
	longs = (sa->sa_flags & SA_NOMASK)?7:8;		/*允许自己的处理句柄收到信号自己,将阻塞码也压入栈中*/
	*(&esp) -= longs;
	/*检查内存使用情况(超界时,分配新页)*/
	verify_area(esp,longs*4);
	/*栈中从下往上的顺序*/
	tmp_esp=esp;
	put_fs_long((long) sa->sa_restorer,tmp_esp++);
	put_fs_long(signr,tmp_esp++);
	if (!(sa->sa_flags & SA_NOMASK))
		put_fs_long(current->blocked,tmp_esp++);
	put_fs_long(eax,tmp_esp++);
	put_fs_long(ecx,tmp_esp++);
	put_fs_long(edx,tmp_esp++);
	put_fs_long(eflags,tmp_esp++);
	put_fs_long(old_eip,tmp_esp++);
	current->blocked |= sa->sa_mask;
}
