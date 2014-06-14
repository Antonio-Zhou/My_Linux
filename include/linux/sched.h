/*
 * 调度程序头文件,定义了任务结构task_struct，初始任务0的数据
 * 还有一些有关描述符参数设置和获取的嵌入式汇编函数宏语句
 * */

#ifndef _SCHED_H
#define _SCHED_H

#define NR_TASKS 64
#define HZ 100

#define FIRST_TASK task[0]
#define LAST_TASK task[NR_TASKS-1]

#include <linux/head.h>
#include <linux/fs.h>
#include <linux/mm.h>
#include <signal.h>

#if (NR_OPEN > 32)
#error "Currently the close-on-exec-flags are in one word, max 32 files/proc"
#endif

#define TASK_RUNNING		0
#define TASK_INTERRUPTIBLE	1
#define TASK_UNINTERRUPTIBLE	2
#define TASK_ZOMBIE		3
#define TASK_STOPPED		4

#ifndef NULL
#define NULL ((void *) 0)
#endif

extern int copy_page_tables(unsigned long from, unsigned long to, long size);
extern int free_page_tables(unsigned long from, unsigned long size);

extern void sched_init(void);
extern void schedule(void);
extern void trap_init(void);
extern void panic(const char * str);
extern int tty_write(unsigned minor,char * buf,int count);

typedef int (*fn_ptr)();

struct i387_struct {
	long	cwd;
	long	swd;
	long	twd;
	long	fip;
	long	fcs;
	long	foo;
	long	fos;
	long	st_space[20];	/* 8*10 bytes for each FP-reg = 80 bytes */
};

struct tss_struct {
	long	back_link;	/* 16 high bits zero */
	long	esp0;
	long	ss0;		/* 16 high bits zero */
	long	esp1;
	long	ss1;		/* 16 high bits zero */
	long	esp2;
	long	ss2;		/* 16 high bits zero */
	long	cr3;
	long	eip;
	long	eflags;
	long	eax,ecx,edx,ebx;
	long	esp;
	long	ebp;
	long	esi;
	long	edi;
	long	es;		/* 16 high bits zero */
	long	cs;		/* 16 high bits zero */
	long	ss;		/* 16 high bits zero */
	long	ds;		/* 16 high bits zero */
	long	fs;		/* 16 high bits zero */
	long	gs;		/* 16 high bits zero */
	long	ldt;		/* 16 high bits zero */
	long	trace_bitmap;	/* bits: trace 0, bitmap 16-31 */
	struct i387_struct i387;
};

/*进程控制块PCB,保存着用于控制和管理进程的所有信息*/
struct task_struct {
/* these are hardcoded - don't touch */
	long state;	/* -1 unrunnable, 0 runnable, >0 stopped */	/*任务运行状态*/
	long counter;		/*任务运行时间计数(递减)(滴答数),运行时间片*/
	long priority;		/*运行优先数.任务开始时,counter=priority 越大运行时间越长*/
	long signal;		/*信号,是位图,每个比特位代表一种信号,信号值= 位偏移值+ 1*/
	struct sigaction sigaction[32];		/*信号执行属性结构,对应信号将要执行的操作和标志信息*/
	long blocked;	/* bitmap of masked signals */		/*进程信号屏蔽码(对应信号位图)*/
/* various fields */
	int exit_code;		/*任务停止执行后的退出码,其父进程会来取*/
	/*
	 * start_code:代码段地址;
	 * end_code:代码长度(字节数);
	 * end_data:代码长度+数据长度(字节数);
	 * brk:总长度(字节数);
	 * start_stack:堆栈段地址
	 * */
	unsigned long start_code,end_code,end_data,brk,start_stack;
	/*
	 * pid:进程标识号;
	 * father:父进程号;
	 * pgrp:进程组号;
	 * session:会话号;
	 * leader:会话首领
	 * */
	long pid,father,pgrp,session,leader;
	/*
	 * uid:用户标识号(用户id);
	 * euid:有效用户id;
	 * suid:保存的用户id
	 * */
	unsigned short uid,euid,suid;
	/*
	 * uid:组标识号(组id);
	 * euid:有效组id;
	 * suid:保存的组id
	 * */
	unsigned short gid,egid,sgid;
	/*报警定时值(滴答数)*/
	long alarm;
	/*
	 * utime:用户态运行时间(滴答数);
	 * stime:系统态运行时间(滴答数);
	 * cutime:子进程用户态运行时间;
	 * cstime:子进程系统态运行时间;
	 * start_time:进程开始运行时刻
	 * `*/
	long utime,stime,cutime,cstime,start_time;
	unsigned short used_math;		/*标志:是否用了协处理器*/
/* file system info */
	int tty;		/* -1 if no tty, so it must be signed */	/*进程使用tty终端的子设备号,-1-->没有使用*/
	unsigned short umask;			/*文件创建属性屏蔽位*/
	struct m_inode * pwd;			/*当前工作目录i节点结构指针*/
	struct m_inode * root;			/*根目录i节点结构指针*/
	struct m_inode * executable;		/*执行文件i节点结构指针*/
	unsigned long close_on_exec;		/*执行时关闭句柄位图标志(include/fcntl.h)*/
	struct file * filp[NR_OPEN];		/*文件结构指针表,最多32项,表项号即是文件描述符的值*/
/* ldt for this task 0 - zero 1 - cs 2 - ds&ss */
	struct desc_struct ldt[3];		/*局部描述符表*/
/* tss for this task */
	struct tss_struct tss;			/*进程的任务状态段信息结构*/
};

/*
 *  INIT_TASK is used to set up the first task table, touch at
 * your own risk!. Base=0, limit=0x9ffff (=640kB)
 */

/*对应上面任务结构的第一个任务的信息*/
#define INIT_TASK \
/* state etc */	{ 0,15,15, \
/* signals */	0,{{},},0, \
/* ec,brk... */	0,0,0,0,0,0, \
/* pid etc.. */	0,-1,0,0,0, \
/* uid etc */	0,0,0,0,0,0, \
/* alarm */	0,0,0,0,0,0, \
/* math */	0, \
/* fs info */	-1,0022,NULL,NULL,NULL,0, \
/* filp */	{NULL,}, \
	{ \
		{0,0}, \
/* ldt */	{0x9f,0xc0fa00}, \
		{0x9f,0xc0f200}, \
	}, \
/*tss*/	{0,PAGE_SIZE+(long)&init_task,0x10,0,0,0,0,(long)&pg_dir,\
	 0,0,0,0,0,0,0,0, \
	 0,0,0x17,0x17,0x17,0x17,0x17,0x17, \
	 _LDT(0),0x80000000, \
		{} \
	}, \
}

extern struct task_struct *task[NR_TASKS];
extern struct task_struct *last_task_used_math;
extern struct task_struct *current;
extern long volatile jiffies;
extern long startup_time;

/*
 * jiffies: 系统滴答值10毫秒,定义了宏方便了对当前时间的访问
 * HZ = 100 内核系统时钟频率
 * */
#define CURRENT_TIME (startup_time+jiffies/HZ)

extern void add_timer(long jiffies, void (*fn)(void));
extern void sleep_on(struct task_struct ** p);
extern void interruptible_sleep_on(struct task_struct ** p);
extern void wake_up(struct task_struct ** p);

/*
 * Entry into gdt where to find first TSS. 0-nul, 1-cs, 2-ds, 3-syscall
 * 4-TSS0, 5-LDT0, 6-TSS1 etc ...
 */
#define FIRST_TSS_ENTRY 4
#define FIRST_LDT_ENTRY (FIRST_TSS_ENTRY+1)
#define _TSS(n) ((((unsigned long) n)<<4)+(FIRST_TSS_ENTRY<<3))
#define _LDT(n) ((((unsigned long) n)<<4)+(FIRST_LDT_ENTRY<<3))
#define ltr(n) __asm__("ltr %%ax"::"a" (_TSS(n)))
#define lldt(n) __asm__("lldt %%ax"::"a" (_LDT(n)))
#define str(n) \
__asm__("str %%ax\n\t" \
	"subl %2,%%eax\n\t" \
	"shrl $4,%%eax" \
	:"=a" (n) \
	:"a" (0),"i" (FIRST_TSS_ENTRY<<3))
/*
 *	switch_to(n) should switch tasks to task nr n, first
 * checking that n isn't the current task, in which case it does nothing.
 * This also clears the TS-flag if the task we switched to has used
 * tha math co-processor latest.
 */

/*
 * 跳转到一个任务的TSS  段选择符组成的地址处会造成CPU  进行任务切换操作
 * 参数: %0->指向 _tmp %1-->指向_tmp.b,用于存放新的TSS的选择符
 * 	 dx-->新任务n的TSS 段选择符 ecx-->新任务n的任务结构指针task[n]
 * */
#define switch_to(n) {\
struct {long a,b;} __tmp; \
__asm__("cmpl %%ecx,_current\n\t" \				/*n是当前任务吗?*/
	"je 1f\n\t" \						/*是则退出*/
	"movw %%dx,%1\n\t" \					/*将新任务TSS的16位选择符存入_tmp.b中*/
	"xchgl %%ecx,_current\n\t" \				/*current=task[n],ecx=被切换出的任务*/
	"ljmp %0\n\t" \						/*长跳转至*&_tmp,造成任务切换,回来之后才会继续执行下面的语句*/
	"cmpl %%ecx,_last_task_used_math\n\t" \			/*原任务上次使用过协处理器吗?*/
	"jne 1f\n\t" \						/*没有则跳转退出*/
	"clts\n" \						/*使用过,则清cr0中的任务切换*/
	"1:" \							/*标志TS*/
	::"m" (*&__tmp.a),"m" (*&__tmp.b), \
	"d" (_TSS(n)),"c" ((long) task[n])); \
}

#define PAGE_ALIGN(n) (((n)+0xfff)&0xfffff000)

#define _set_base(addr,base) \
__asm__("movw %%dx,%0\n\t" \
	"rorl $16,%%edx\n\t" \
	"movb %%dl,%1\n\t" \
	"movb %%dh,%2" \
	::"m" (*((addr)+2)), \
	  "m" (*((addr)+4)), \
	  "m" (*((addr)+7)), \
	  "d" (base) \
	:"dx")

#define _set_limit(addr,limit) \
__asm__("movw %%dx,%0\n\t" \
	"rorl $16,%%edx\n\t" \
	"movb %1,%%dh\n\t" \
	"andb $0xf0,%%dh\n\t" \
	"orb %%dh,%%dl\n\t" \
	"movb %%dl,%1" \
	::"m" (*(addr)), \
	  "m" (*((addr)+6)), \
	  "d" (limit) \
	:"dx")

#define set_base(ldt,base) _set_base( ((char *)&(ldt)) , base )
#define set_limit(ldt,limit) _set_limit( ((char *)&(ldt)) , (limit-1)>>12 )

#define _get_base(addr) ({\
unsigned long __base; \
__asm__("movb %3,%%dh\n\t" \
	"movb %2,%%dl\n\t" \
	"shll $16,%%edx\n\t" \
	"movw %1,%%dx" \
	:"=d" (__base) \
	:"m" (*((addr)+2)), \
	 "m" (*((addr)+4)), \
	 "m" (*((addr)+7))); \
__base;})

/*取局部描述符中ldt所指段描述符中的基地址*/
#define get_base(ldt) _get_base( ((char *)&(ldt)) )

#define get_limit(segment) ({ \
unsigned long __limit; \
__asm__("lsll %1,%0\n\tincl %0":"=r" (__limit):"r" (segment)); \
__limit;})

#endif
