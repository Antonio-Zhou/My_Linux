/*
 * 	cirlinklist_ops.c
 *
 *	Created on:	2013-05-15
 *	    Author:	Eric Zhou
 */

/* 
 * 设立尾指针的单循环链表操作1：
 * 操作结果：构造一个空的线性表L 
 */
Status InitList_CL(LinkList *L)
{
	*L = (LinkList)malloc(sizeof(struct LNode)); /* 产生头结点,并使L指向此头结点 */

	/* 存储分配失败 */
	if (!*L)
		exit(OVERFLOW);
	(*L)->next = *L; /* 指针域指向头结点 */

	return OK;
}

/* 
 * 设立尾指针的单循环链表操作2：
 * 操作结果：销毁线性表L  改变L 
 */
Status DestroyList_CL(LinkList *L)
{
	LinkList q;
	LinkList p = (*L)->next; /* p指向头结点 */

	/* 没到表尾 */
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
 * 设立尾指针的单循环链表操作3：
 * 初始条件：线性表L已存在。
 * 操作结果：将L重置为空表 
 * */
Status ClearList_CL(LinkList *L)
{
	LinkList p, q;
	*L = (*L)->next; /* L指向头结点 */
	p = (*L)->next; /* p指向第一个结点 */

	/* 没到表尾 */
	while (p != *L) {
		q = p->next;
		free(p);
		p = q;
	}
	(*L)->next = *L; /* 头结点指针域指向自身 */

	return OK;
}

/* 
 * 设立尾指针的单循环链表操作4：
 * 初始条件：线性表L已存在。
 * 操作结果：若L为空表，则返回TRUE，否则返回FALSE 
 */
Status ListEmpty_CL(LinkList L)
{
	/* 空 */
	if (L->next == L)
		return TRUE;
	else
		return FALSE;
}

/* 
 * 设立尾指针的单循环链表操作5：
 * 初始条件：L已存在。
 * 操作结果：返回L中数据元素个数 
 */
int ListLength_CL(LinkList L)
{
	int i = 0;
	LinkList p = L->next; /* p指向头结点 */

	/* 没到表尾 */
	while (p != L) {
		i++;
		p = p->next;
	}

	return i;
}

/* 
 * 设立尾指针的单循环链表操作6：
 * 当第i个元素存在时,其值赋给e并返回OK,否则返回ERROR 
 */ 
Status GetElem_CL(LinkList L, int i, ElemType *e)
{
	int j = 1; /* 初始化,j为计数器 */
	LinkList p = L->next->next; /* p指向第一个结点 */

	/* 第i个元素不存在 */
	if ((i <= 0) || (i > ListLength_CL(L)))
		return ERROR;
	/* 顺指针向后查找,直到p指向第i个元素 */
	while (j < i) {
		p = p->next;
		j++;
	}
	*e = p->data; /* 取第i个元素 */

	return OK;
}

/* 
 * 设立尾指针的单循环链表操作7：
 * 初始条件：线性表L已存在，compare()是数据元素判定函数 
 * 操作结果：返回L中第1个与e满足关系compare()的数据元素的位序。若这样的数据元素不存在，则返回值为0 
 */
int LocateElem_CL(LinkList L, ElemType e, Status(*compare)(ElemType, ElemType))
{
	int i = 0;
	LinkList p = L->next->next; /* p指向第一个结点 */
	while (p != L->next) {
		i++;
		/* 满足关系 */
		if (compare(p->data, e))
			return i;
		p = p->next;
	}
	return 0;
}

/* 
 * 设立尾指针的单循环链表操作8：
 * 初始条件：线性表L已存在 
 * 操作结果：若cur_e是L的数据元素，且不是第一个，则用pre_e返回它的前驱，否则操作失败，pre_e无定义 
 */
Status PriorElem_CL(LinkList L, ElemType cur_e, ElemType *pre_e)
{
	LinkList q;
	LinkList p = L->next->next; /* p指向第一个结点 */
	q = p->next;

	/* p没到表尾 */
	while (q != L->next) {
		if (q->data == cur_e) {
			*pre_e=p->data;
			return TRUE;
		}
		p = q;
		q = q->next;
	}

	return FALSE;
}

/* 
 * 设立尾指针的单循环链表操作9：
 * 初始条件：线性表L已存在
 * 操作结果：若cur_e是L的数据元素,且不是最后一个,则用next_e返回它的后继，否则操作失败，next_e无定义 
 */
Status NextElem_CL(LinkList L, ElemType cur_e, ElemType *next_e)
{
	LinkList p = L->next->next; /* p指向第一个结点 */

	/* p没到表尾 */
	while (p != L) {
		if (p->data == cur_e) {
			*next_e = p->next->data;
			return TRUE;
		}
		p = p->next;
	}

	return FALSE;
}

/* 
 * 设立尾指针的单循环链表操作10：
 * 在L的第i个位置之前插入元素e, 改变L
 */
Status ListInsert_CL(LinkList *L, int i, ElemType e)
{
	LinkList p = (*L)->next; /* p指向头结点 */
	LinkList s;
	int j = 0;

	/* 无法在第i个元素之前插入 */
	if ((i <= 0) || (i > ListLength_CL(*L) + 1))
		return ERROR;
	/* 寻找第i-1个结点 */
	while (j < i - 1) {
		p = p->next;
		j++;
	}
	s = (LinkList)malloc(sizeof(struct LNode)); /* 生成新结点 */
	s->data = e; /* 插入L中 */
	s->next = p->next;
	p->next = s;
	/* 改变尾结点 */
	if (p == *L)
		*L = s;

	return OK;
}

/* 
 * 设立尾指针的单循环链表操作11：
 * 删除L的第i个元素,并由e返回其值  改变L 
 */
Status ListDelete_CL(LinkList *L, int i, ElemType *e)
{
	LinkList p = (*L)->next; /* p指向头结点 */
	LinkList q;
	int j = 0;

	/* 第i个元素不存在 */
	if ((i <= 0) || (i > ListLength_CL(*L)))
		return ERROR;
	/* 寻找第i-1个结点 */
	while (j < i-1) {
		p = p->next;
		j++;
	}
	q = p->next; /* q指向待删除结点 */
	p->next = q->next;
	*e = q->data;
	/* 删除的是表尾元素 */
	if (*L == q)
		*L = p;
	free(q); /* 释放待删除结点 */

	return OK;
}

/* 
 * 设立尾指针的单循环链表操作12：
 * 初始条件:L已存在。
 * 操作结果:依次对L的每个数据元素调用函数vi()。一旦vi()失败,则操作失败 
 */
Status ListTraverse_CL(LinkList L, void(*vi)(ElemType))
{
	LinkList p = L->next->next;

	while (p != L->next) {
		vi(p->data);
		p = p->next;
	}
	printf("\n");

	return OK;
}
