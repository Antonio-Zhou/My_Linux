/*
 *  arch/s390/kernel/traps.c
 *
 *  S390 version
 *    Copyright (C) 1999,2000 IBM Deutschland Entwicklung GmbH, IBM Corporation
 *    Author(s): Martin Schwidefsky (schwidefsky@de.ibm.com),
 *               Denis Joseph Barrow (djbarrow@de.ibm.com,barrow_dj@yahoo.com),
 *
 *  Derived from "arch/i386/kernel/traps.c"
 *    Copyright (C) 1991, 1992 Linus Torvalds
 */

/*
 * 'Traps.c' handles hardware traps and faults after we have saved some
 * state in 'asm.s'.
 */
#include <linux/config.h>
#include <linux/sched.h>
#include <linux/kernel.h>
#include <linux/string.h>
#include <linux/errno.h>
#include <linux/ptrace.h>
#include <linux/timer.h>
#include <linux/mm.h>
#include <linux/smp.h>
#include <linux/smp_lock.h>
#include <linux/init.h>
#include <linux/delay.h>

#include <asm/system.h>
#include <asm/uaccess.h>
#include <asm/io.h>
#include <asm/spinlock.h>
#include <asm/atomic.h>
#include <asm/mathemu.h>
#if CONFIG_REMOTE_DEBUG
#include <asm/gdb-stub.h>
#endif

#include "cpcmd.h"

/* Called from entry.S only */
extern void handle_per_exception(struct pt_regs *regs);

typedef void pgm_check_handler_t(struct pt_regs *, long);
pgm_check_handler_t *pgm_check_table[128];

extern pgm_check_handler_t do_page_fault;
extern pgm_check_handler_t do_pseudo_page_fault;

static inline void console_verbose(void)
{
        extern int console_loglevel;
        console_loglevel = 15;
}

spinlock_t die_lock;

void die(const char * str, struct pt_regs * regs, long err)
{
        console_verbose();
        spin_lock_irq(&die_lock);
        printk("%s: %04lx\n", str, err & 0xffff);
        show_regs(regs);
        spin_unlock_irq(&die_lock);
        do_exit(SIGSEGV);
}

#define DO_ERROR(signr, str, name) \
asmlinkage void name(struct pt_regs * regs, long interruption_code) \
{ \
	do_trap(interruption_code, signr, str, regs); \
}

static void inline do_trap(long interruption_code, int signr, char *str,
			   struct pt_regs *regs)
{
	if (regs->psw.mask & PSW_PROBLEM_STATE) {
		struct task_struct *tsk = current;
		tsk->tss.trap_no = interruption_code;
		force_sig(signr, tsk);
#ifdef CONFIG_PROCESS_DEBUG
                printk("User process fault: interruption code 0x%lX\n",
		       interruption_code);
                show_regs(regs);
#endif
	} else {
		unsigned long fixup = search_exception_table(regs->psw.addr);
		if (fixup)
			regs->psw.addr = fixup;
		else
			die(str, regs, interruption_code);
	}
}

int do_debugger_trap(struct pt_regs *regs,int signal)
{
	if(regs->psw.mask&PSW_PROBLEM_STATE)
	{
		if(current->ptrace & PT_PTRACED)
			force_sig(signal,current);
		else
			return(TRUE);
	}
	else
	{
#if CONFIG_REMOTE_DEBUG
		if(gdb_stub_initialised)
		{
			gdb_stub_handle_exception((gdb_pt_regs *)regs,signal);
			return(FALSE);
		}
#endif
		return(TRUE);
	}
	return(FALSE);
}

DO_ERROR(SIGSEGV, "Unknown program exception", default_trap_handler)
DO_ERROR(SIGILL,  "privileged operation", privileged_op)
DO_ERROR(SIGILL,  "execute exception", execute_exception)
DO_ERROR(SIGSEGV, "addressing exception", addressing_exception)
DO_ERROR(SIGFPE,  "fixpoint divide exception", divide_exception)
DO_ERROR(SIGILL,  "translation exception", translation_exception)
DO_ERROR(SIGILL,  "special operand exception", special_op_exception)
DO_ERROR(SIGILL,  "operand exception", operand_exception)

asmlinkage void illegal_op(struct pt_regs * regs, long interruption_code)
{
        __u8 opcode[6];
	__u16 *location;
	int do_sig = 0;
	int problem_state=(regs->psw.mask & PSW_PROBLEM_STATE);

        lock_kernel();
	location = (__u16 *)(regs->psw.addr-S390_lowcore.pgm_ilc);
	if(problem_state)
		get_user(*((__u16 *) opcode), location);
	else
		*((__u16 *)opcode)=*((__u16 *)location);
	if(*((__u16 *)opcode)==S390_BREAKPOINT_U16)
        {
		if(do_debugger_trap(regs,SIGTRAP))
			do_sig=1;
	}
#ifdef CONFIG_IEEEFPU_EMULATION
        else if (problem_state )
	{
		if (opcode[0] == 0xb3) {
			get_user(*((__u16 *) (opcode+2)), location+1);
			do_sig = math_emu_b3(opcode, regs);
                } else if (opcode[0] == 0xed) {
			get_user(*((__u32 *) (opcode+2)),
				 (__u32 *)(location+1));
			do_sig = math_emu_ed(opcode, regs);
		} else if (*((__u16 *) opcode) == 0xb299) {
			get_user(*((__u16 *) (opcode+2)), location+1);
			do_sig = math_emu_srnm(opcode, regs);
		} else if (*((__u16 *) opcode) == 0xb29c) {
			get_user(*((__u16 *) (opcode+2)), location+1);
			do_sig = math_emu_stfpc(opcode, regs);
		} else if (*((__u16 *) opcode) == 0xb29d) {
			get_user(*((__u16 *) (opcode+2)), location+1);
			do_sig = math_emu_lfpc(opcode, regs);
		} else
			do_sig = 1;
        }
#endif 
	else
		do_sig = 1;
	if (do_sig)
		do_trap(interruption_code, SIGILL, "illegal operation", regs);
        unlock_kernel();
}



#ifdef CONFIG_IEEEFPU_EMULATION
asmlinkage void specification_exception(struct pt_regs * regs, long interruption_code)
{
        __u8 opcode[6];
	__u16 *location;
	int do_sig = 0;

        lock_kernel();
        if (regs->psw.mask & 0x00010000L) {
		location = (__u16 *)(regs->psw.addr-S390_lowcore.pgm_ilc);
		get_user(*((__u16 *) opcode), location);
		switch (opcode[0]) {
		case 0x28: /* LDR Rx,Ry   */
			do_sig=math_emu_ldr(opcode);
			break;
		case 0x38: /* LER Rx,Ry   */
			do_sig=math_emu_ler(opcode);
			break;
		case 0x60: /* STD R,D(X,B) */
			get_user(*((__u16 *) (opcode+2)), location+1);
			do_sig=math_emu_std(opcode, regs);
			break;
		case 0x68: /* LD R,D(X,B) */
			get_user(*((__u16 *) (opcode+2)), location+1);
			do_sig=math_emu_ld(opcode, regs);
			break;
		case 0x70: /* STE R,D(X,B) */
			get_user(*((__u16 *) (opcode+2)), location+1);
			do_sig=math_emu_ste(opcode, regs);
			break;
		case 0x78: /* LE R,D(X,B) */
			get_user(*((__u16 *) (opcode+2)), location+1);
			do_sig=math_emu_le(opcode, regs);
			break;
		default:
			do_sig = 1;
			break;
                }
        } else
		do_sig = 1;
        if (do_sig)
                do_trap(interruption_code, SIGILL, "specification exception", regs);
        unlock_kernel();
}
#else
DO_ERROR(SIGILL, "specification exception", specification_exception)
#endif

asmlinkage void data_exception(struct pt_regs * regs, long interruption_code)
{
        __u8 opcode[6];
	__u16 *location;
	int do_sig = 0;

        lock_kernel();
	location = (__u16 *)(regs->psw.addr-S390_lowcore.pgm_ilc);
	if(MACHINE_HAS_IEEE)
	{
		__asm__ volatile ("stfpc %0\n\t" 
				  : "=m" (current->tss.fp_regs.fpc));
		
	}
	/* Same code should work when we implement fpu emulation */
	/* provided we call data exception from the fpu emulator */
	if(current->tss.fp_regs.fpc&FPC_DXC_MASK)
	{
		current->tss.ieee_instruction_pointer=
			(addr_t)ADDR_BITS_REMOVE((addr_t)location);
		force_sig(SIGFPE, current);
	}
#ifdef CONFIG_IEEEFPU_EMULATION
        else if ((regs->psw.mask & 0x00010000L)) 
	{
		get_user(*((__u16 *) opcode), location);
		switch (opcode[0]) {
		case 0x28: /* LDR Rx,Ry   */
			do_sig=math_emu_ldr(opcode);
			break;
		case 0x38: /* LER Rx,Ry   */
			do_sig=math_emu_ler(opcode);
			break;
		case 0x60: /* STD R,D(X,B) */
			get_user(*((__u16 *) (opcode+2)), location+1);
			do_sig=math_emu_std(opcode, regs);
			break;
		case 0x68: /* LD R,D(X,B) */
			get_user(*((__u16 *) (opcode+2)), location+1);
			do_sig=math_emu_ld(opcode, regs);
			break;
		case 0x70: /* STE R,D(X,B) */
			get_user(*((__u16 *) (opcode+2)), location+1);
			do_sig=math_emu_ste(opcode, regs);
			break;
		case 0x78: /* LE R,D(X,B) */
			get_user(*((__u16 *) (opcode+2)), location+1);
			do_sig=math_emu_le(opcode, regs);
			break;
		case 0xb3:
			get_user(*((__u16 *) (opcode+2)), location+1);
			do_sig = math_emu_b3(opcode, regs);
			break;
                case 0xed:
			get_user(*((__u32 *) (opcode+2)),
				 (__u32 *)(location+1));
			do_sig = math_emu_ed(opcode, regs);
			break;
	        case 0xb2:
			if (opcode[1] == 0x99) {
				get_user(*((__u16 *) (opcode+2)), location+1);
				do_sig = math_emu_srnm(opcode, regs);
			} else if (opcode[1] == 0x9c) {
				get_user(*((__u16 *) (opcode+2)), location+1);
				do_sig = math_emu_stfpc(opcode, regs);
			} else if (opcode[1] == 0x9d) {
				get_user(*((__u16 *) (opcode+2)), location+1);
				do_sig = math_emu_lfpc(opcode, regs);
			} else
				do_sig = 1;
			break;
		default:
			do_sig = 1;
			break;
                }
        } 
#endif
	else
		do_sig = 1;
        if (do_sig)
                do_trap(interruption_code, SIGILL, "data exception", regs);
        unlock_kernel();
}



/* init is done in lowcore.S and head.S */

__initfunc(void trap_init(void))
{
        int i;

        for (i = 0; i < 128; i++)
          pgm_check_table[i] = &default_trap_handler;
        pgm_check_table[1] = &illegal_op;
        pgm_check_table[2] = &privileged_op;
        pgm_check_table[3] = &execute_exception;
        pgm_check_table[4] = &do_page_fault;
        pgm_check_table[5] = &addressing_exception;
        pgm_check_table[6] = &specification_exception;
        pgm_check_table[7] = &data_exception;
        pgm_check_table[9] = &divide_exception;
        pgm_check_table[0x10] = &do_page_fault;
        pgm_check_table[0x11] = &do_page_fault;
        pgm_check_table[0x12] = &translation_exception;
        pgm_check_table[0x13] = &special_op_exception;
	pgm_check_table[0x14] = &do_pseudo_page_fault;
        pgm_check_table[0x15] = &operand_exception;
        pgm_check_table[0x1C] = &privileged_op;
	if (MACHINE_IS_VM)
	        cpcmd("SET PAGEX ON", NULL, 0);
}


void handle_per_exception(struct pt_regs *regs)
{
	if(regs->psw.mask&PSW_PROBLEM_STATE)
	{
		per_struct *per_info=&current->tss.per_info;
		per_info->lowcore.words.perc_atmid=S390_lowcore.per_perc_atmid;
		per_info->lowcore.words.address=S390_lowcore.per_address;
		per_info->lowcore.words.access_id=S390_lowcore.per_access_id;
	}
	if(do_debugger_trap(regs,SIGTRAP))
	{
		/* I've seen this possibly a task structure being reused ? */
		printk("Spurious per exception detected\n");
		printk("switching off per tracing for this task.\n");
		show_regs(regs);
		/* Hopefully switching off per tracing will help us survive */
		regs->psw.mask &= ~PSW_PER_MASK;
	}
}

