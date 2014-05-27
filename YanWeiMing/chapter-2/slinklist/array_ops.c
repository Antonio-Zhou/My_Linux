/*
 * 	array_ops.c	
 *
 *	Created on:	2013-05-12
 *	    Author:	Eric Zhou
 */

/* 
 * 静态链表操作1：
 * 构造一个空的链表，表头为L的最后一个单元L[MAXSIZE-1]，
 * 其余单元链成一个备用链表，表头为L的第一个单元L[0]，“0”表示空指针 
 * */
void InitList(SLinkList L)
{
	int i;
	L[MAXSIZE-1].cur = 0; /* L的最后一个单元为空链表的表头 */
	/* 将其余单元链接成以L[0]为表头的备用链表 */
	for (i = 0; i < MAXSIZE - 2; i++)
		L[i].cur = i + 1;
	L[MAXSIZE-2].cur = 0;
}

/* 
 * 静态链表操作2：
 * 初始条件：线性表L已存在。操作结果：将L重置为空表 
 */
Status ClearList(SLinkList L)
{
	int i,j,k;

	i = L[MAXSIZE-1].cur; /* 链表第一个结点的位置 */
	L[MAXSIZE-1].cur = 0; /* 链表空 */
	k = L[0].cur; /* 备用链表第一个结点的位置 */
	L[0].cur = i; /* 把链表的结点连到备用链表的表头 */
	/* 没到链表尾 */
	while (i) {
		j = i;
		i = L[i].cur; /* 指向下一个元素 */
	}
	L[j].cur = k; /* 备用链表的第一个结点接到链表的尾部 */

	return OK;
}

/* 
 * 静态链表操作3：
 * 若L是空表，返回TRUE；否则返回FALSE 
 */
Status ListEmpty(SLinkList L)
{
	/* 空表 */
	if (L[MAXSIZE-1].cur == 0)
		return TRUE;
	else
		return FALSE;
}

/* 
 * 静态链表操作4：
 * 返回L中数据元素个数 
 */
int ListLength(SLinkList L)
{
	int j = 0;
	int i = L[MAXSIZE-1].cur; /* i指向第一个元素 */

	/* 没到静态链表尾 */
	while (i) {
		i = L[i].cur; /* 指向下一个元素 */
		j++;
	}

	return j;
}

/* 
 * 静态链表操作5：
 * 用e返回L中第i个元素的值 
 */
Status GetElem(SLinkList L, int i, ElemType *e)
{
	int l, k=MAXSIZE-1; /* k指向表头序号 */

	if ((i < 1) || (i > ListLength(L)))
		return ERROR;
	/* 移动到第i个元素处 */
	for (l = 1; l <= i; l++)
		k = L[k].cur;
	*e = L[k].data;

	return OK;
}

/* 
 * 静态链表操作6：
 * 在静态单链线性表L中查找第1个值为e的元素。
 * 若找到，则返回它在L中的位序，否则返回0。（与其它LocateElem()的定义不同) 
 */
int LocateElem(SLinkList L, ElemType e)
{
	int i = L[MAXSIZE-1].cur; /* i指示表中第一个结点 */

	/* 在表中顺链查找(e不能是字符串) */
	while (i && L[i].data != e)
		i = L[i].cur;

	return i;
}

/* 
 * 静态链表操作7：
 * 初始条件：线性表L已存在
 * 操作结果：若cur_e是L的数据元素，且不是第一个，则用pre_e返回它的前驱，否则操作失败，pre_e无定义 
 */
Status PriorElem(SLinkList L, ElemType cur_e, ElemType *pre_e)
{
	int j;
	int i = L[MAXSIZE-1].cur; /* i指示链表第一个结点的位置 */

	/* 向后移动结点 */
	do {
		j = i;
		i = L[i].cur;
	}while(i && cur != L[i].data);
	/* 找到该元素 */
	if (i) {
		*pre_e = L[j].data;
		return OK;
	}

	return ERROR;
}

/* 
 * 静态链表操作8：
 * 初始条件：线性表L已存在 
 * 操作结果：若cur_e是L的数据元素，且不是最后一个，则用next_e返回它的后继， 否则操作失败，next_e无定义 
 */
Status NextElem(SLinkList L, ElemType cur_e, ElemType *next_e)
{
	int j;
	int i = LocateElem(L, cur_e); /* 在L中查找第一个值为cur_e的元素的位置 */

	/* L中存在元素cur_e */
	if (i) {
		j = L[i].cur; /* cur_e的后继的位置 */
		/* cur_e有后继 */
		if (j) {
			*next_e = L[j].data;
			return OK; /* cur_e元素有后继 */
		}
	}

	return ERROR; /* L不存在cur_e元素,cur_e元素无后继 */
}

/* 
 * 静态链表操作9：
 * 在L中第i个元素之前插入新的数据元素e 
 */
Status ListInsert(SLinkList L, int i, ElemType e)
{
	int l, j, k=MAXSIZE-1; /* k指向表头 */

	if ((i < 1) || (i > ListLength(L) + 1))
		return ERROR;
	j = Malloc(L); /* 申请新单元 */
	/* 申请成功 */
	if (j) {
		L[j].data = e; /* 赋值给新单元 */
		/* 移动i-1个元素 */
		for (l = 1; l < i; l++)
			k = L[k].cur;
		L[j].cur = L[k].cur;
		L[k].cur = j;
		return OK;
	}

	return ERROR;
}

/* 
 * 静态链表操作10：
 * 删除在L中第i个数据元素e，并返回其值 
 */
Status ListDelete(SLinkList L, int i, ElemType *e)
{
	int j; 
	int k=MAXSIZE-1; /* k指向表头 */

	if ((i < 1) || (i > ListLength(L)))
		return ERROR;
	/* 移动i-1个元素 */
	for (j = 1; j < i; j++)
		k = L[k].cur;
	j = L[k].cur;
	L[k].cur = L[j].cur;
	*e = L[j].data;
	Free(L, j);

	return OK;
}

/* 
 * 静态链表操作11：
 * 初始条件:线性表L已存在 
 * 操作结果:依次对L的每个数据元素调用函数vi()。一旦vi()失败,则操作失败 
 */
Status ListTraverse(SLinkList L, void(*vi)(ElemType))
{
	int i = L[MAXSIZE-1].cur; /* 指向第一个元素 */
	/* 没到静态链表尾 */
	while (i) {
		vi(L[i].data); /* 调用vi() */
		i = L[i].cur; /* 指向下一个元素 */
	}
	printf("\n");

	return OK;
}
