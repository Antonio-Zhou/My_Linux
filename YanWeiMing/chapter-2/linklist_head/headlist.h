/*
 * 	base.h
 *
 *	Created on:	2013-05-15
 *	    Author:	Eric Zhou
 */

/* 结点类型 */
typedef struct LNode {
	ElemType data;
	struct LNode *next;
}LNode, *Link, *Position;

/* 链表类型 */
typedef struct LinkList {
	Link head, tail; /* 分别指向线性链表中的头结点和最后一个结点 */
	int len; /* 指示线性链表中数据元素的个数 */
}LinkList;
