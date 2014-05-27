/*
 * 	dulinklist_ops.c
 *
 *	Created on:	2013-05-14
 *	    Author:	Eric Zhou
 */

/* 
 * 双向循环线性表操作1：
 * 产生空的双向循环链表L 
 */
Status InitList(DuLinkList *L)
{
	*L = (DuLinkList)malloc(sizeof(DuLNode));
	if (*L) {
		(*L)->next = (*L)->prior =*L;
		return OK;
	} else
		return OVERFLOW;
}

/* 
 * 双向循环线性表操作2：
 * 操作结果：销毁双向循环链表L 
 */
Status DestroyList(DuLinkList *L)
{
	DuLinkList q;
	DuLinkList p = (*L)->next; /* p指向第一个结点 */

	/* p没到表头 */
	while (p != *L) {
		q = p->next;
		free(p);
		p = q;
	}
	free(*L);
	*L = NULL;

	return OK;
}

/* 
 * 双向循环线性表操作3：
 * 初始条件：L已存在。
 * 操作结果：将L重置为空表 
 */
Status ClearList(DuLinkList L)
{
	DuLinkList q;
	DuLinkList p = L->next; /* p指向第一个结点 */

	/* p没到表头 */
	while (p != L) {
		q = p->next;
		free(p);
		p = q;
	}
	L->next = L->prior = L; /* 头结点的两个指针域均指向自身 */

	return OK;
}

/* 
 * 双向循环线性表操作4：
 * 初始条件：L已存在。
 * 初始条件：线性表L已存在。
 * 操作结果：若L为空表，则返回TRUE，否则返回FALSE 
 */
Status ListEmpty(DuLinkList L)
{
	if((L->next == L) && (L->prior == L))
		return TRUE;
	else
		return FALSE;
}

/* 
 * 双向循环线性表操作5：
 * 初始条件：L已存在。
 * 操作结果：返回L中数据元素个数 
 */
int ListLength(DuLinkList L)
{
	int i = 0;
	DuLinkList p = L->next; /* p指向第一个结点 */

	/* p没到表头 */
	while (p != L) {
		i++;
		p = p->next;
	}

	return i;
}

/* 
 * 双向循环线性表操作6：
 * 当第i个元素存在时,其值赋给e并返回OK,否则返回ERROR 
 */
Status GetElem(DuLinkList L, int i, ElemType *e)
{
	int j = 1; /* j为计数器 */
	DuLinkList p = L->next; /* p指向第一个结点 */

	/* 顺指针向后查找,直到p指向第i个元素或p指向头结点 */
	while ((p != L) && (j < i)) {
		p = p->next;
		j++;
	}
	/* 第i个元素不存在 */
	if ((p == L) || (j > i))
		return ERROR;
	*e = p->data; /* 取第i个元素 */

	return OK;
}

/* 
 * 双向循环线性表操作7：
 * 初始条件：L已存在，compare()是数据元素判定函数 
 * 操作结果：返回L中第1个与e满足关系compare()的数据元素的位序。若这样的数据元素不存在，则返回值为0 
 */
int LocateElem(DuLinkList L, ElemType e, Status(*compare)(ElemType, ElemType))
{
	int i = 0;
	DuLinkList p = L->next; /* p指向第1个元素 */

	while (p != L) {
		i++;
		if (compare(p->data, e)) /* 找到这样的数据元素 */
			return i;
		p = p->next;
	}

	return 0;
}

/* 
 * 双向循环线性表操作8：
 * 操作结果：若cur_e是L的数据元素，且不是第一个，则用pre_e返回它的前驱，否则操作失败，pre_e无定义 
 */
Status PriorElem(DuLinkList L, ElemType cur_e, ElemType *pre_e)
{
	DuLinkList p = L->next->next; /* p指向第2个元素 */

	/* p没到表头 */
	while (p != L) {
		if (p->data == cur_e) {
			*pre_e = p->prior->data;
			return TRUE;
		}
		p = p->next;
	}

	return FALSE;
}

/* 
 * 双向循环线性表操作9：
 * 操作结果：若cur_e是L的数据元素，且不是最后一个，则用next_e返回它的后继，否则操作失败，next_e无定义 
 */
Status NextElem(DuLinkList L,ElemType cur_e,ElemType *next_e)
{
	DuLinkList p = L->next->next; /* p指向第2个元素 */

	/* p没到表头 */
	while (p != L) {
		if (p->prior->data == cur_e) {
			*next_e = p->data;
			return TRUE;
		}
		p = p->next;
	}

	return FALSE;
}

/* 
 * 双向循环线性表操作10：
 * 在双向链表L中返回第i个元素的位置指针(算法2.18、2.19要调用的函数) 
 */
DuLinkList GetElemP(DuLinkList L, int i)
{
	int j;
	DuLinkList p = L;

	for (j = 1; j <= i; j++)
		p = p->next;

	return p;
}

/* 
 * 双向循环线性表操作11：
 * 在带头结点的双链循环线性表L中第i个位置之前插入元素e，i的合法值为1≤i≤表长+1 
 */
Status ListInsert(DuLinkList L, int i, ElemType e)
{
	DuLinkList p, s;

	/* i值不合法 */
	if ((i < 1) || (i > ListLength(L) + 1))
		return ERROR;
	p = GetElemP(L, i - 1); /* 在L中确定第i-1个元素的位置指针p */
	/* p=NULL,即第i-1个元素不存在 */
	if (!p)
		return ERROR;
	s = (DuLinkList)malloc(sizeof(DuLNode));
	if (!s)
		return OVERFLOW;
	s->data = e; /* 在第i-1个元素之后插入 */
	s->prior = p;
	s->next = p->next;
	p->next->prior = s;
	p->next = s;

	return OK;
}

/* 
 * 双向循环线性表操作12：
 * 删除带头结点的双链循环线性表L的第i个元素,i的合法值为1≤i≤表长+1 
 */
Status ListDelete(DuLinkList L, int i, ElemType *e)
{
	DuLinkList p;

	/* i值不合法 */
	if ((i < 1) || (i > ListLength(L)))
		return ERROR;
	p = GetElemP(L, i);  /* 在L中确定第i个元素的位置指针p */
	/* p=NULL,即第i个元素不存在 */
	if (!p)
		return ERROR;
	*e = p->data;
	p->prior->next = p->next;
	p->next->prior = p->prior;
	free(p);

	return OK;
}

/* 
 * 双向循环线性表操作13：
 * 由双链循环线性表L的头结点出发,正序对每个数据元素调用函数visit() 
 */
void ListTraverse(DuLinkList L, void(*visit)(ElemType))
{
	DuLinkList p = L->next; /* p指向头结点 */
	while (p != L) {
		visit(p->data);
		p = p->next;
	}
	printf("\n");
}

/* 
 * 双向循环线性表操作14：
 * 由双链循环线性表L的头结点出发,逆序对每个数据元素调用函数visit()。
 * */
void ListTraverseBack(DuLinkList L, void(*visit)(ElemType))
{
	DuLinkList p = L->prior; /* p指向尾结点 */
	while (p != L) {
		visit(p->data);
		p = p->prior;
	}
	printf("\n");
}
