/*
 * 用于异步串行通信芯片UATR进行初始化,并设置两个通信端口的中断向量
 * 还包括tty用于串口输出的rs_write()
 * */

/*
 *  linux/kernel/serial.c
 *
 *  (C) 1991  Linus Torvalds
 */

/*
 *	serial.c
 *
 * This module implements the rs232 io functions
 *	void rs_write(struct tty_struct * queue);
 *	void rs_init(void);
 * and all interrupts pertaining to serial IO.
 */

#include <linux/tty.h>
#include <linux/sched.h>
#include <asm/system.h>
#include <asm/io.h>

#define WAKEUP_CHARS (TTY_BUF_SIZE/4)

extern void rs1_interrupt(void);
extern void rs2_interrupt(void);

static void init(int port)
{
	outb_p(0x80,port+3);	/* set DLAB of line control reg */
	outb_p(0x30,port);	/* LS of divisor (48 -> 2400 bps */
	outb_p(0x00,port+1);	/* MS of divisor */
	outb_p(0x03,port+3);	/* reset DLAB */
	outb_p(0x0b,port+4);	/* set DTR,RTS, OUT_2 */
	outb_p(0x0d,port+1);	/* enable all intrs but writes */
	(void)inb(port);	/* read data port to reset things (?) */
}


/*
 * 功能: 初始化串行中断程序和串行接口
 * 参数: 无
 * 返回值: 无
 * */
void rs_init(void)
{
	/*设置两个串行口的中断门描述符.rs1_interrupt是串口1的中断处理过程指针*/
	set_intr_gate(0x24,rs1_interrupt);		/*设置串口1的中断门向量(IRQ4 信号)*/
	set_intr_gate(0x23,rs2_interrupt);		/*设置串口2的中断门向量(IRQ3 信号)*/
	init(tty_table[1].read_q.data);			/*初始化串行口1(.data是基口地址)*/
	init(tty_table[2].read_q.data);			/*初始化串行口2*/
	outb(inb_p(0x21)&0xE7,0x21);			/*允许主8259A响应IRQ3,IRQ4中断请求疑问,这里的port为什么是0x21  ---- OK*/

	/*
	 * outb(inb_p(0x21)&0xE7,0x21);解析:
	 * inb_p(0x21):返回读取0x21的字节,即8259A芯片当前屏蔽字节,
	 * 然后和0xE7相与,将位3和位4置0,即响应相对应的中断请求.这里就是IRQ3,IRQ4
	 * */
}

/*
 * This routine gets called when tty_write has put something into
 * the write_queue. It must check wheter the queue is empty, and
 * set the interrupt register accordingly
 *
 *	void _rs_write(struct tty_struct * tty);
 */
void rs_write(struct tty_struct * tty)
{
	cli();
	if (!EMPTY(tty->write_q))
		outb(inb_p(tty->write_q.data+1)|0x02,tty->write_q.data+1);
	sti();
}
