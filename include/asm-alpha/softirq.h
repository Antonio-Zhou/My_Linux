#ifndef _ALPHA_SOFTIRQ_H
#define _ALPHA_SOFTIRQ_H

#include <linux/config.h>
#include <linux/stddef.h>
#include <asm/atomic.h>
#include <asm/hardirq.h>

#ifndef CONFIG_SMP
extern int __local_bh_count;
#define local_bh_count(cpu)	((void)(cpu), __local_bh_count)
#else
#define local_bh_count(cpu)	(cpu_data[cpu].bh_count)
#endif

extern inline void cpu_bh_disable(int cpu)
{
	local_bh_count(cpu)++;
	mb();
}

extern inline void cpu_bh_enable(int cpu)
{
	mb();
	local_bh_count(cpu)--;
}

#define local_bh_enable()	cpu_bh_enable(smp_processor_id())
#define local_bh_disable()	cpu_bh_disable(smp_processor_id())

#define in_softirq() (local_bh_count(smp_processor_id()) != 0)

#endif /* _ALPHA_SOFTIRQ_H */
