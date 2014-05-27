/*
 * 	cirlinklist.h	
 *
 *	Created on:	2013-05-14
 *	    Author:	Eric Zhou
 */

struct LNode
{
	ElemType data;
	struct LNode *next;
}; 

/* 另一种定义LinkList的方法 */
typedef struct LNode *LinkList;
