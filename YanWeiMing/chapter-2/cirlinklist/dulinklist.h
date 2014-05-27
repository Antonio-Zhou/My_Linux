/*
 * 	dulinklist.h
 *
 *	Created on:	2013-05-15
 *	    Author:	Eric Zhou
 */

typedef struct DuLNode {
	ElemType data;
	struct DuLNode *prior, *next;
}DuLNode, *DuLinkList;
