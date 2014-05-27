/*
 * 	main.c	
 *
 *	Created on:	2013-05-10
 *	    Author:	Eric Zhou
 */

#include "base.h"
typedef int ElemType;
#include "sqlist.h"
#include "sqlist_ops.c"

/*
 * 数据元素判定函数(平方关系)
 */
Status comp(ElemType c1,ElemType c2)
{
	if (c1 == c2*c2)
		return TRUE;
	else
		return FALSE;
}

/*
 * ListTraverse()调用的函数(类型要一致)
 */
void visit(ElemType *c)
{
	printf("%d ", *c);
}

/*
 * ListTraverse()调用的另一函数(元素值加倍)
 */
void dbl(ElemType *c)
{
	*c *= 2;
}

int main(void)
{
	SqList L;
	ElemType e, e0;
	Status i;
	int j, k;

	i = InitList(&L);
	printf("初始化L后：L.elem=%u L.length=%d L.listsize=%d\n", (unsigned int)L.elem, L.length, L.listsize);
	for (j=1; j<=5; j++)
		i=ListInsert(&L,1,j);

	printf("在L的表头依次插入1～5后：*L.elem=");
	for (j = 1; j <= 5; j++)
		printf("%d ",*(L.elem + j - 1));
	printf("\n");
	printf("L.elem=%u L.length=%d L.listsize=%d\n", (unsigned int)L.elem, L.length, L.listsize);

	i = ListEmpty(L);
	printf("L是否空：i=%d(1:是 0:否)\n", i);
	i = ClearList(&L);
	printf("清空L后：L.elem=%u L.length=%d L.listsize=%d\n", (unsigned int)L.elem, L.length, L.listsize);
	i = ListEmpty(L);
	printf("L是否空：i=%d(1:是 0:否)\n", i);

	for (j = 1; j <= 10; j++)
		ListInsert(&L, j, j);
	printf("在L的表尾依次插入1～10后：*L.elem=");
	for (j = 1; j <= 10; j++)
		printf("%d ", *(L.elem + j - 1));
	printf("\n");
	printf("L.elem=%u L.length=%d L.listsize=%d\n", (unsigned int)L.elem, L.length, L.listsize);
	ListInsert(&L, 1, 0);
	printf("在L的表头插入0后：*L.elem=");
	for (j = 1; j <= ListLength(L); j++) /* ListLength(L)为元素个数 */
		printf("%d ", *(L.elem + j - 1));
	printf("\n");
	printf("L.elem=%u(有可能改变) L.length=%d(改变) L.listsize=%d(改变)\n", (unsigned int)L.elem, L.length, L.listsize);

	GetElem(L, 5, &e);
	printf("第5个元素的值为：%d\n", e);
	for (j = 3; j <= 4; j ++) {
		k = LocateElem(L, j, comp);
		if (k)
			printf("第%d个元素的值为%d的平方\n", k, j);
		else
			printf("没有值为%d的平方的元素\n", j);
	}

	/* 测试头两个数据 */
	for (j = 1; j <= 2; j++) {
		GetElem(L, j, &e0); /* 把第j个数据赋给e0 */
		i = PriorElem(L, e0, &e); /* 求e0的前驱 */
		if (i == INFEASIBLE)
			printf("元素%d无前驱\n",e0);
		else
			printf("元素%d的前驱为：%d\n",e0,e);
	}
	/* 最后两个数据 */
	for (j = ListLength(L) - 1; j <= ListLength(L); j++) { 
		GetElem(L, j, &e0); /* 把第j个数据赋给e0 */
		i = NextElem(L, e0, &e); /* 求e0的后继 */
		if (i == INFEASIBLE)
			printf("元素%d无后继\n", e0);
		else
			printf("元素%d的后继为：%d\n", e0, e);
	}

	k = ListLength(L); /* k为表长 */
	for (j = k + 1;j >= k; j--) {
		i = ListDelete(&L, j, &e); /* 删除第j个数据 */
		if (i == ERROR)
			printf("删除第%d个数据失败\n", j);
		else
			printf("删除的元素值为：%d\n", e);
	}
	printf("依次输出L的元素：");
	ListTraverse(L, visit); /* 依次对元素调用visit()，输出元素的值 */
	printf("L的元素值加倍后：");
	ListTraverse(L, dbl); /* 依次对元素调用dbl()，元素值乘2 */
	ListTraverse(L, visit);
	DestroyList(&L);
	printf("销毁L后：L.elem=%u L.length=%d L.listsize=%d\n", (unsigned int)L.elem, L.length, L.listsize);
}
