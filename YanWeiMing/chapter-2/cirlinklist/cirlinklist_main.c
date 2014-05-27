/*
 * 	cirlinklist_main.c
 *
 *	Created on:	2013-05-15
 *	    Author:	Eric Zhou
 */

#include "base.h"
typedef int ElemType;
#include "cirlinklist.h"
#include "cirlinklist_ops.c"

/* 
 * 数据元素判定函数(判定相等) 
 */
Status compare(ElemType c1, ElemType c2)
{
	if(c1 == c2)
		return TRUE;
	else
		return FALSE;
}

/* 
 * ListTraverse()调用的函数(类型一致) 
 */
void visit(ElemType c)
{
	printf("%d ", c);
}

int main(void)
{
	LinkList L;
	ElemType e;
	int j;
	Status i;

	i = InitList_CL(&L); /* 初始化单循环链表L */
	printf("初始化单循环链表L i=%d (1:初始化成功)\n", i);

	i = ListEmpty_CL(L);
	printf("L是否空 i=%d(1:空 0:否)\n", i);
	ListInsert_CL(&L, 1, 3); /* 在L中依次插入3,5 */
	ListInsert_CL(&L, 2, 5);
	i = GetElem_CL(L, 1, &e);
	j = ListLength_CL(L);
	printf("L中数据元素个数=%d,第1个元素的值为%d。\n", j, e);
	printf("L中的数据元素依次为：");
	ListTraverse_CL(L, visit);

	PriorElem_CL(L, 5, &e); /* 求元素5的前驱 */
	printf("5前面的元素的值为%d。\n", e);

	NextElem_CL(L, 3, &e); /* 求元素3的后继 */
	printf("3后面的元素的值为%d。\n", e);

	printf("L是否空 %d(1:空 0:否)\n", ListEmpty_CL(L));
	j = LocateElem_CL(L, 5, compare);
	if (j)
		printf("L的第%d个元素为5。\n", j);
	else
		printf("不存在值为5的元素\n");

	i = ListDelete_CL(&L, 2, &e);
	printf("删除L的第2个元素：\n");
	if (i) {
		printf("删除的元素值为%d,现在L中的数据元素依次为：", e);
		ListTraverse_CL(L, visit);
	} else
		printf("删除不成功！\n");

	printf("清空L：%d(1: 成功)\n", ClearList_CL(&L));
	printf("清空L后，L是否空：%d(1:空 0:否)\n", ListEmpty_CL(L));
	printf("销毁L：%d(1: 成功)\n", DestroyList_CL(&L));

	return 0;
}
