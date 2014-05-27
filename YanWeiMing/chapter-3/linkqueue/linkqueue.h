/* c3-2.h 单链队列－－队列的链式存储结构 */
/*
 * 	base.h	
 *
 *	Created on:	2013-05-23
 *	    Author:	Eric Zhou
 */
typedef struct QNode
{
	QElemType data;
	struct QNode *next;
}QNode, *QueuePtr;

typedef struct
{
	QueuePtr front,rear; /* 队头、队尾指针 */
}LinkQueue;
