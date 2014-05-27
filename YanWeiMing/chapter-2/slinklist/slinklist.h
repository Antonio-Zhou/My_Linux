/*
 *      slinklist.h
 *
 *      Created on:     2013-05-10
 *          Author:     Eric Zhou
 */

#define MAXSIZE 100 /* 链表的最大长度 */
typedef struct
{
	ElemType data;
	int cur;
}component,SLinkList[MAXSIZE];
