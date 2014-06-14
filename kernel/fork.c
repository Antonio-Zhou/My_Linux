/*
 * 程序给出了sys_fork()系统调用中使用了两个c函数:
 * find_empty_process()和copy_process()
 * */

/*
 *  linux/kernel/fork.c
 *
 *  (C) 1991  Linus Torvalds
 */

/*
 *  'fork.c' contains the help-routines for the 'fork' system call
 * (see also system_call.s), and some misc functions ('verify_area').
 * Fork is rather simple, once you get the hang of it, but the memory
 * management can be a bitch. See 'mm/mm.c': 'copy_page_tables()'
 */
#include <errno.h>		/*错误号头文件-->包括系统中各种错误号*/

#include <linux/sched.h>	/*调度程序头文件-->定义了任务结构task_struct,任务0的数据*/
#include <linux/kernel.h>	/*内核头文件*/
#include <asm/segment.h>	/*段操作头文件*/
#include <asm/system.h>		/*系统头文件*/

/*写页面验证,若页面不可写,则复制页面(mm/memory.c)*/
extern void write_verify(unsigned long address);

/*最新进程号,其值会由get_empty_process()生成*/
long last_pid=0;

/*
 * 功能: 进程空间区域写前验证
 * 参数: void * addr-->进程空间地址
 * 	 int size-->空间长度
 * 返回值: 无
 * */
void verify_area(void * addr,int size)
{
	unsigned long start;

	/*
	 * 将start调整为其所在页的左边界开始位置，同时调整验证区域大小
	 * 整个地址的高20位表示页地址，而低12 位表示页内偏移值
	 * */
	start = (unsigned long) addr;
	/*
	 * 用于获得指定起始位置addr(start)在所在页面中的偏移地址
	 * 再加上原来的size,就扩展成以addr所在页面起始位置开始的范围
	 * */
	size += start & 0xfff;
	start &= 0xfffff000;		/*调整页面边界值-->页*/
	/*
	 * 再调整start:加上进程数据段在线性地址空间中的起始基地址,变成系统整个线性空间中的地址
	 * ldt[2]是什么东东????   数据段和堆栈段
	 * */
	start += get_base(current->ldt[2]);
	/*循环写页面验证，若不可写，就复制页面*/
	while (size>0) {
		size -= 4096;
		write_verify(start);
		start += 4096;
	}
}

/*
 * 功能: 复制内存页表,为新任务在线性地址空间中设置代码段和数据段基址,限长,并复制页表,
 * 	 由于写时复制,因此这里仅为新进程设置自己的页目录表项和页表项,没有实际为新进程分配物理内存页面
 * 参数: int nr-->新任务号
 * 	 struct task_struct * p-->新任务数据结构指针
 * 返回值: 0-->成功,错误号-->失败
 * 总的思路:
 * 	1.获取当前进程局部描述符表中代码段描述符和数据段描述符中的段限长;
 * 	2.设置创建中的新进程在线性地址空间中的基地址;
 * 	3.设置新进程的页目录表项和页表项(复制当前进程),此时不分配物理空间.
 * */
int copy_mem(int nr,struct task_struct * p)
{
	unsigned long old_data_base,new_data_base,data_limit;		/*数据段基址和段限长*/
	unsigned long old_code_base,new_code_base,code_limit;

	code_limit=get_limit(0x0f);			/*任务代码段*/
	data_limit=get_limit(0x17);			/*任务数据段*/
	old_code_base = get_base(current->ldt[1]);	/*线性地址空间中的基地址*/
	old_data_base = get_base(current->ldt[2]);
	if (old_data_base != old_code_base)		/*还不支持代码段和数据段的分立*/
		panic("We don't support separate I&D");
	if (data_limit < code_limit)
		panic("Bad data_limit");
	/*新进程在线性地址空间中基地址-->64MB*其任务号,并用该值设置新进程局部描述符表中的基地址*/
	new_data_base = new_code_base = nr * 0x4000000;
	p->start_code = new_code_base;
	set_base(p->ldt[1],new_code_base);
	set_base(p->ldt[2],new_data_base);
	/*设置页目录表项和页表项*/
	if (copy_page_tables(old_data_base,new_data_base,data_limit)) {
		free_page_tables(new_data_base,data_limit);
		return -ENOMEM;				/*若出错,则释放刚申请的页表项*/
	}
	return 0;
}

/*
 *  Ok, this is the main fork-routine. It copies the system process
 * information (task[nr]) and sets up the necessary registers. It
 * also copies the data segment in it's entirety.
 */

/*
 * 功能: 复制系统进程信息(task[n])，并设置必要的寄存器
 * 参数: 是进入系统调用那个中断处理(system_call.s)开始,直到调用本系统调用处理过程和调用本函数前时逐步压入栈的各寄存器的值
 * 	 long ss,long esp-->CPU执行中断指令压入的用户栈地址
 * 	 long eflags-->CPU执行中断指令压入的标志寄存器
 * 	 long cs,long eip-->CPU执行中断指令压入的返回地址
 * 	 long ds,long es,long fs,long edx,long ecx,long ebx-->刚进入system_call.s时压入栈的段寄存器
 * 	 long none-->调用sys_call_table中sys_fork函数时压入栈放入返回地址
 * 	 long gs,long esi,long edi,long ebp,int nr(eax)-->调用copy_process()之前压入栈各值
 *	 	 nr是调用find_empty_process()分配的任务数组项号
 * 返回值: 新进程的pid
 * */
int copy_process(int nr,long ebp,long edi,long esi,long gs,long none,
		long ebx,long ecx,long edx,
		long fs,long es,long ds,
		long eip,long cs,long eflags,long esp,long ss)
{
	struct task_struct *p;
	int i;
	struct file *f;

	/*为新进程分配内存*/
	p = (struct task_struct *) get_free_page();
	if (!p)
		return -EAGAIN;
	/*将新任务指针放入任务指针数组*/
	task[nr] = p;
	*p = *current;	/* NOTE! this doesn't copy the supervisor stack */	/*只复制了进程结构*/
	/*对新进程的进程结构内容进行修改*/
	p->state = TASK_UNINTERRUPTIBLE;	/*设为不可中断等待状态,防止内核调度执行*/
	p->pid = last_pid;			/*新进程号,由find_empty_process()得到*/
	p->father = current->pid;		/*设置父进程号*/
	p->counter = p->priority;		/*运行时间片值*/
	p->signal = 0;				/*信号位图置0*/
	p->alarm = 0;				/*报警定时值(滴答数)*/
	p->leader = 0;		/* process leadership doesn't inherit */
	p->utime = p->stime = 0;		/*用户态和核心态运行时间*/
	p->cutime = p->cstime = 0;		/*子进程用户态和核心态运行时间*/
	p->start_time = jiffies;		/*子进程开始运行时间(当前时间滴答数)*/
	/*任务状态段TSS数据的修改*/
	p->tss.back_link = 0;
	p->tss.esp0 = PAGE_SIZE + (long) p;	/*任务的内核态堆栈被设置在位于其任务数据结构所在页面的顶端*/
	p->tss.ss0 = 0x10;			/*内核态栈的段选择符(与内核数据段相同)*/
	p->tss.eip = eip;			/*指令代码指针*/
	p->tss.eflags = eflags;			/*标志寄存器*/
	p->tss.eax = 0;				/*当fork()返回时新进程会返回0的原因所在*/
	p->tss.ecx = ecx;
	p->tss.edx = edx;
	p->tss.ebx = ebx;
	p->tss.esp = esp;
	p->tss.ebp = ebp;
	p->tss.esi = esi;
	p->tss.edi = edi;
	p->tss.es = es & 0xffff;		/*段寄存器仅16位有效*/
	p->tss.cs = cs & 0xffff;
	p->tss.ss = ss & 0xffff;
	p->tss.ds = ds & 0xffff;
	p->tss.fs = fs & 0xffff;
	p->tss.gs = gs & 0xffff;
	p->tss.ldt = _LDT(nr);			/*任务局部表描述符的选择符*/
	p->tss.trace_bitmap = 0x80000000;	/*高16位有效*/
	/*
	 * 协处理器,若使用过则保存其上下文
	 * clts: 用于清除控制寄存器CR0中的任务已交换(TS)标志
	 * fnsave: 把协处理器的所有状态保存到目的操作数制动的内存区域中
	 * */
	if (last_task_used_math == current)
		__asm__("clts ; fnsave %0"::"m" (p->tss.i387));
	/*
	 * 复制进程页表
	 * 在线性地址空间中设置新任务代码段和数据段描述符中的基地址和限长,
	 * 并复制页表返回值不为0-->出错
	 * */
	if (copy_mem(nr,p)) {
		task[nr] = NULL;
		free_page((long) p);
		return -EAGAIN;
	}
	/*
	 * 若父进程中有文件时打开的,则将对应文件的打开次数加1
	 * 父子进程会共享打开的文件
	 * */
	for (i=0; i<NR_OPEN;i++)
		if (f=p->filp[i])
			f->f_count++;
	if (current->pwd)
		current->pwd->i_count++;
	if (current->root)
		current->root->i_count++;
	if (current->executable)
		current->executable->i_count++;
	/*
	 * 在GDT表中设置新任务的TSS段和LDT段描述符项
	 * gdt+(nr<<1)+FIRST_TSS_ENTRY-->是任务nr的TSS描述符在全局表中的地址
	 * (nr<<1)-->每个任务占2项
	 * */
	set_tss_desc(gdt+(nr<<1)+FIRST_TSS_ENTRY,&(p->tss));
	set_ldt_desc(gdt+(nr<<1)+FIRST_LDT_ENTRY,&(p->ldt));
	/*设置成就绪态,在任务切换时,任务寄存器tr由CPU自动加载*/
	p->state = TASK_RUNNING;	/* do this last, just in case */
	return last_pid;
}

/*
 * 功能: 为新进程取得不重复的进程号last_pid
 * 参数: 无
 * 返回值: 新进程在任务数组中的任务号(数组项)
 * */
int find_empty_process(void)
{
	int i;

	repeat:
		if ((++last_pid)<0) last_pid=1;
		for(i=0 ; i<NR_TASKS ; i++)
			if (task[i] && task[i]->pid == last_pid) goto repeat;
	for(i=1 ; i<NR_TASKS ; i++)		/*任务0 被排除在外*/
		if (!task[i])
			return i;
	return -EAGAIN;				/*64项全部被占用*/
}
