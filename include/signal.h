/*
 * 信号头文件,定义信号符号常量,信号结构及信号操作原型
 * */

#ifndef _SIGNAL_H
#define _SIGNAL_H

#include <sys/types.h>

typedef int sig_atomic_t;
typedef unsigned int sigset_t;		/* 32 bits */

#define _NSIG             32
#define NSIG		_NSIG

#define SIGHUP		 1		/*Hang Up-->挂断控制终端或进程*/
#define SIGINT		 2		/*Interrupt-->来自键盘的中断*/
#define SIGQUIT		 3		/*Quit-->来自键盘的退出*/
#define SIGILL		 4		/*Illeagle-->非法指令*/
#define SIGTRAP		 5		/*Trap-->跟踪断点*/
#define SIGABRT		 6		/*Abort-->异常结束*/
#define SIGIOT		 6		/*IO  Trap-->同上*/
#define SIGUNUSED	 7		/*Unsed-->没有使用*/
#define SIGFPE		 8		/*FPE-->协处理器出错*/
#define SIGKILL		 9		/*Kill-->强迫进程终止*/
#define SIGUSR1		10		/*User1-->用户信号1,进程可使用*/
#define SIGSEGV		11		/*Segment Violation-->无效内存引用*/
#define SIGUSR2		12		/*User2-->用户信号2,进程可使用*/
#define SIGPIPE		13		/*Pipe-->管道写出错*/
#define SIGALRM		14		/*Alarm-->定时定时器调整*/
#define SIGTERM		15		/*Terminate-->进程终止*/
#define SIGSTKFLT	16		/*Stack Fault-->栈出错(协处理器)*/
#define SIGCHLD		17		/*Child-->子进程停止或被终止*/
#define SIGCONT		18		/*Continue-->恢复进程继续执行*/
#define SIGSTOP		19		/*Stop-->停止进程执行*/
#define SIGTSTP		20		/*TTY Stop-->tty发出停止进程,可忽略*/
#define SIGTTIN		21		/*TTY In-->后台进程请求输入*/
#define SIGTTOU		22		/*TTY Out-->后台进程请求输出*/

/* Ok, I haven't implemented sigactions, but trying to keep headers POSIX */
#define SA_NOCLDSTOP	1
#define SA_NOMASK	0x40000000
#define SA_ONESHOT	0x80000000

#define SIG_BLOCK          0	/* for blocking signals */
#define SIG_UNBLOCK        1	/* for unblocking signals */
#define SIG_SETMASK        2	/* for setting the signal mask */

#define SIG_DFL		((void (*)(int))0)	/* default signal handling */
#define SIG_IGN		((void (*)(int))1)	/* ignore signal */

struct sigaction {
	void (*sa_handler)(int);		/*信号处理句柄*/
	sigset_t sa_mask;			/*信号的屏蔽码,可以阻塞指定的信号集*/
	int sa_flags;				/*信号选项标志*/
	void (*sa_restorer)(void);		/*信号恢复函数指针(系统内部使用)*/
};

void (*signal(int _sig, void (*_func)(int)))(int);
int raise(int sig);
int kill(pid_t pid, int sig);
int sigaddset(sigset_t *mask, int signo);
int sigdelset(sigset_t *mask, int signo);
int sigemptyset(sigset_t *mask);
int sigfillset(sigset_t *mask);
int sigismember(sigset_t *mask, int signo); /* 1 - is, 0 - not, -1 error */
int sigpending(sigset_t *set);
int sigprocmask(int how, sigset_t *set, sigset_t *oldset);
int sigsuspend(sigset_t *sigmask);
int sigaction(int sig, struct sigaction *act, struct sigaction *oldact);

#endif /* _SIGNAL_H */
