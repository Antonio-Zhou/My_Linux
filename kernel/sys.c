/*
 * 包括很多系统调用函数，其中有些还没有实现
 * */

/*
 *  linux/kernel/sys.c
 *
 *  (C) 1991  Linus Torvalds
 */

/**/
#include <errno.h>

#include <linux/sched.h>
#include <linux/tty.h>
#include <linux/kernel.h>
#include <asm/segment.h>
#include <sys/times.h>
#include <sys/utsname.h>	/*系统名称结构头文件*/

/*
 * 返回日期和时间-->以下返回值是ENOSYS的系统调用函数表示在本版中没有实现的
 * */
int sys_ftime()
{
	return -ENOSYS;
}

/**/
int sys_break()
{
	return -ENOSYS;
}

/*当前进程对子进程的调试*/
int sys_ptrace()
{
	return -ENOSYS;
}

/*改变并打印终端行设置*/
int sys_stty()
{
	return -ENOSYS;
}

/*取终端行设置信息*/
int sys_gtty()
{
	return -ENOSYS;
}

/*修改文件名*/
int sys_rename()
{
	return -ENOSYS;
}

/**/
int sys_prof()
{
	return -ENOSYS;
}

/*
 * 功能: 设置当前任务的实际以及/或者有效组ID(gid)
 * 参数: int rgid-->实际组gid
 * 	 int egid-->有效组gid
 * 返回值: 成功返回0
 * */
int sys_setregid(int rgid, int egid)
{
	/*
	 * 如果任务没有超级用户特权,那么只能互换实际组ID和有效组ID
	 * 如果任务具有超级用户特权,就能任意设置有效的和实际的组ID,sgid=egid
	 * */
	if (rgid>0) {
		if ((current->gid == rgid) || 
		/*检测是否是超级用户*/
		    suser())
			current->gid = rgid;
		else
			return(-EPERM);
	}
	if (egid>0) {
		if ((current->gid == egid) ||
		    (current->egid == egid) ||
		    (current->sgid == egid) ||
		    suser())
			current->egid = egid;
		else
			return(-EPERM);
	}
	return 0;
}

/*
 * 功能: 设置进程组号(gid)
 * 参数: int gid-->指定的gid
 * 返回值: sys_setregid的返回值
 * */
int sys_setgid(int gid)
{
	/*
	 * 如果任务没有超级用户特权,可以使用setgid()将其egid设置成sgid或rgid
	 * 如果任务有超级用户特权,则rgid和egid都被设置成参数指定的gid
	 * */
	return(sys_setregid(gid, gid));
}

/*打开或关闭进程计帐功能*/
int sys_acct()
{
	return -ENOSYS;
}

/*映射任意物理内存到进程的虚拟地址空间*/
int sys_phys()
{
	return -ENOSYS;
}

int sys_lock()
{
	return -ENOSYS;
}

int sys_mpx()
{
	return -ENOSYS;
}

int sys_ulimit()
{
	return -ENOSYS;
}

/*
 * 功能: 返回1970年1月1日00:00:00开始计时的时间值(秒)
 * 参数: long * tloc-->指向存储返回值的用户空间
 * 返回值: 时间值
 * */
int sys_time(long * tloc)
{
	int i;

	i = CURRENT_TIME;
	if (tloc) {
		verify_area(tloc,4);			/*验证内存容量是否足够(4字节)*/
		put_fs_long(i,(unsigned long *)tloc);	/*存放至用户数据段tloc处*/
	}
	return i;
}

/*
 * Unprivileged users may change the real user id to the effective uid
 * or vice versa.
 */

/*
 * 功能: 设置任务的实际以及/或者有效的用户ID
 * 参数: int ruid-->实际用户ID
 * 	 int euid-->有效用户ID
 * 返回值: 成功返回0
 * */
int sys_setreuid(int ruid, int euid)
{

	/*
	 * 如果任务没有超级用户特权,那么只能互换实际uid和有效uid
	 * 如果任务具有超级用户特权,就能任意设置有效的和实际的用户ID,suid=euid
	 * */
	int old_ruid = current->uid;
	
	if (ruid>0) {
		if ((current->euid==ruid) ||
                    (old_ruid == ruid) ||
		    suser())
			current->uid = ruid;
		else
			return(-EPERM);
	}
	if (euid>0) {
		if ((old_ruid == euid) ||
                    (current->euid == euid) ||
		    suser())
			current->euid = euid;
		else {
			current->uid = old_ruid;
			return(-EPERM);
		}
	}
	return 0;
}

/*
 * 功能: 设置任务用户ID(uid)
 * 参数: int uid-->指定的uid
 * 返回值: sys_setreuid的返回值
*/
int sys_setuid(int uid)
{
	/*
	 * 如果任务没有超级用户特权,可以使用setuid()将其euid设置成suid或ruid
	 * 如果任务有超级用户特权,则rgid,euid和sgid都被设置成参数指定的gid
	 * */
	return(sys_setreuid(uid, uid));
}

/*
 * 功能: 设置系统开机时间
 * 参数: long * tptr-->1970年1月1日00:00:00开始计时的时间值(秒)
 * 返回值: 0
 * */
int sys_stime(long * tptr)
{
	/*调用者必须有超级用户权限,HZ==100-->内核系统运行频率*/
	if (!suser())
		return -EPERM;
	 /*jiffies/HZ--->统已经运行的时间秒值*/
	startup_time = get_fs_long((unsigned long *)tptr) - jiffies/HZ;
	return 0;
}

/*
 * 功能: 获取当前任务运行时间统计值
 * 参数: struct tms * tbuf-->tms结构(包括进程用户运行时间,内核时间,子进程用户运行时间,子进程系统运行时间)
 * 返回值: 系统运行到当前的滴答数
 * */
int sys_times(struct tms * tbuf)
{
	if (tbuf) {
		verify_area(tbuf,sizeof *tbuf);
		put_fs_long(current->utime,(unsigned long *)&tbuf->tms_utime);
		put_fs_long(current->stime,(unsigned long *)&tbuf->tms_stime);
		put_fs_long(current->cutime,(unsigned long *)&tbuf->tms_cutime);
		put_fs_long(current->cstime,(unsigned long *)&tbuf->tms_cstime);
	}
	return jiffies;
}

/*
 * 功能: 设置数据段末尾为end_data_seg指定的值
 * 参数: unsigned long end_data_seg-->指定要设定的值
 * 返回值: 数据段的新结尾值
 * 	不由用户直接调用,而是由libc 库函数进行包装,而且返回值也不一样
 * */
int sys_brk(unsigned long end_data_seg)
{
	/*条件是: end_data_seg数值合理,系统确实有足够的内存,而且没有超越其最大数据段大小时*/
	if (end_data_seg >= current->end_code &&
	    end_data_seg < current->start_stack - 16384)
		current->brk = end_data_seg;
	return current->brk;
}

/*
 * This needs some heave checking ...
 * I just haven't get the stomach for it. I also don't fully
 * understand sessions/pgrp etc. Let somebody who does explain it.
 */

/*
 * 功能: 设定进程pid的进程组号pgid
 * 参数: int pid-->要设定的进程号pid
 * 	 int pgid-->进程组号pgid
 * 返回值: 0
 * */
int sys_setpgid(int pid, int pgid)
{
	int i;

	if (!pid)
		pid = current->pid;
	if (!pgid)
		pgid = current->pid;
	/*
	 * 将进程从一个进程组移到另一个进程组,则这两个进程组必须属于同一个会话
	 * pgid指定了要加入的现有进程组ID
	 * */
	for (i=0 ; i<NR_TASKS ; i++)
		if (task[i] && task[i]->pid==pid) {
			if (task[i]->leader)				/*该进程是会话首领*/
				return -EPERM;
			if (task[i]->session != current->session)	/*两进程不在同一会话组*/
				return -EPERM;
			task[i]->pgrp = pgid;
			return 0;
		}
	return -ESRCH;
}

/*
 * 功能: 获取当前进程的进程组号(同getpid(0))
 * 参数: 无
 * 返回值: 当前进程的进程组号
 * */
int sys_getpgrp(void)
{
	return current->pgrp;
}

/*
 * 功能: 创建一个会话(session)-->设置其leader=1,并且设置其会话号==其组号==其进程号
 * 参数: 无
 * 返回值: 当前进程的pgid
 * */
int sys_setsid(void)
{
	/*已经是会话首领,并且不是超级用户*/
	if (current->leader && !suser())
		return -EPERM;
	current->leader = 1;
	current->session = current->pgrp = current->pid;
	/*表示当前进程没有控制终端*/
	current->tty = -1;
	return current->pgrp;
}

/*
 * 功能: 获取系统名称等信息
 * 参数: struct utsname * name-->utsname结构包括5个字段
 * 返回值: 0
 * */
int sys_uname(struct utsname * name)
{
	static struct utsname thisname = {
		"linux .0","nodename","release ","version ","machine "
	};
	int i;

	/*判断参数的有效性*/
	if (!name) return -ERROR;
	/*验证缓冲区大小是否超限,内核会自动扩展*/
	verify_area(name,sizeof *name);
	/*复制到用户缓冲区中*/
	for(i=0;i<sizeof *name;i++)
		put_fs_byte(((char *) &thisname)[i],i+(char *) name);
	return 0;
}

/*
 * 功能: 设置当前进程创建文件属性屏蔽码
 * 参数: int mask -->用户输入的屏蔽码
 * 返回值: 原来的屏蔽码
*/
int sys_umask(int mask)
{
	int old = current->umask;

	current->umask = mask & 0777;
	return (old);
}
