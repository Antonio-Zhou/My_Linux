/*
 *      linklist_ops.c
 *
 *      Created on:     2013-05-10
 *          Author:     Eric Zhou
 */

/* 
 * 单链表线性表操作1：
 * 操作结果：构造一个空的线性表L
 */
Status InitList(LinkList *L)
{
	*L = (LinkList)malloc(sizeof(struct LNode)); /* 产生头结点,并使L指向此头结点 */
	/* 存储分配失败 */
	if (!*L)
		exit(OVERFLOW);
	(*L)->next = NULL; /* 指针域为空 */

	return OK;
}

/* 
 * 单链表线性表操作2：
 * 初始条件：线性表L已存在。
 * 操作结果：销毁线性表L
 */
Status DestroyList(LinkList *L)
{
	LinkList q;

	while (*L) {
		q = (*L)->next;
		free(*L);
		*L = q;
	}

	return OK;
}

/* 
 * 单链表线性表操作3：
 * 初始条件：线性表L已存在。
 * 操作结果：将L重置为空表
 */
Status ClearList(LinkList L)
{
	LinkList p, q;
	p = L->next; /* p指向第一个结点 */

	/* 没到表尾 */
	while (p) {
	q = p->next;
	free(p);
	p = q;
	}
	L->next = NULL; /* 头结点指针域为空 */

	return OK;
}

/* 
 * 单链表线性表操作4：
 * 初始条件：线性表L已存在。
 * 操作结果：若L为空表，则返回TRUE，否则返回FALSE
 */
Status ListEmpty(LinkList L)
{
	/* 非空 */
	if (L->next)
		return FALSE;
	else
		return TRUE;
}

/* 
 * 单链表线性表操作5：
 * 初始条件：线性表L已存在。
 * 操作结果：返回L中数据元素个数
 */
int ListLength(LinkList L)
{
	int i = 0;
	LinkList p = L->next; /* p指向第一个结点 */

	/* 没到表尾 */
	while (p) {
		i++;
		p = p->next;
	}

	return i;
}

/* 
 * 单链表线性表操作6：
 * L为带头结点的单链表的头指针。当第i个元素存在时,其值赋给e并返回OK,否则返回ERROR 
 */
Status GetElem(LinkList L, int i, ElemType *e)
{
	int j = 1; /* j为计数器 */
	LinkList p = L->next; /* p指向第一个结点 */

	 /* 顺指针向后查找,直到p指向第i个元素或p为空 */
	while ((p && j) < i) {
		p = p->next;
		j++;
	}
	/* 第i个元素不存在 */
	if (!p || j > i)
		return ERROR;
	*e = p->data; /* 取第i个元素 */

	return OK;
}


/*
 * 单链表线性表操作7：
 * 初始条件: 线性表L已存在,compare()是数据元素判定函数(满足为1,否则为0)
 * 操作结果: 返回L中第1个与e满足关系compare()的数据元素的位序。若这样的数据元素不存在,则返回值为0 
 */
int LocateElem(LinkList L, ElemType e, Status(*compare)(ElemType, ElemType))
{
	int i = 0;
	LinkList p = L->next;

	while (p) {
		i++;
		/* 找到这样的数据元素 */
		if (compare(p->data, e))
			return i;
		p = p->next;
	}

	return 0;
}

/* 
 * 单链表线性表操作8：
 * 初始条件: 线性表L已存在
 * 操作结果: 若cur_e是L的数据元素,且不是第一个,则用pre_e返回它的前驱,返回OK;否则操作失败,pre_e无定义,返回INFEASIBLE 
 */
Status PriorElem(LinkList L, ElemType cur_e, ElemType *pre_e)
{
	LinkList q; 
	LinkList p = L->next; /* p指向第一个结点 */

	/* p所指结点有后继 */
	while (p->next) {
		q = p->next; /* q为p的后继 */
		if (q->data == cur_e) {
			*pre_e = p->data;
			return OK;
		}
		p = q; /* p向后移 */
	}

	return INFEASIBLE;
}

/* 
 * 单链表线性表操作9：
 * 初始条件：线性表L已存在
 * 操作结果：若cur_e是L的数据元素，且不是最后一个，则用next_e返回它的后继，返回OK;否则操作失败，next_e无定义，返回INFEASIBLE 
 */
Status NextElem(LinkList L, ElemType cur_e, ElemType *next_e)
{
	LinkList p = L->next; /* p指向第一个结点 */

	/* p所指结点有后继 */
	while (p->next) {
		if (p->data == cur_e) {
			*next_e = p->next->data;
			return OK;
		}
		p = p->next;
	}

	return INFEASIBLE;
}

/* 
 * 单链表线性表操作10：
 * 在带头结点的单链线性表L中第i个位置之前插入元素e 
 */
Status ListInsert(LinkList L, int i, ElemType e)
{
	int j = 0;
	LinkList p = L, s;

	/* 寻找第i-1个结点 */
	while((p && j) < (i - 1)) {
		p = p->next;
		j++;
	}
	/* i小于1或者大于表长 */
	if (!p || (j > i - 1))
		return ERROR;
	s = (LinkList)malloc(sizeof(struct LNode)); /* 生成新结点 */
	s->data = e; /* 插入L中 */
	s->next = p->next;
	p->next = s;

	return OK;
}

/* 
 * 单链表线性表操作11：
 * 在带头结点的单链线性表L中，删除第i个元素,并由e返回其值 
 */
Status ListDelete(LinkList L, int i, ElemType *e)
{
	int j = 0;
	LinkList p = L, q;

	/* 寻找第i个结点,并令p指向其前趋 */
	while ((p->next) && (j < i - 1)) {
		p = p->next;
		j++;
	}
	/* 删除位置不合理 */
	if (!p->next || (j>i-1))
		return ERROR;
	q = p->next; /* 删除并释放结点 */
	p->next = q->next;
	*e = q->data;
	free(q);

	return OK;
}

/* 
 * 单链表线性表操作12：
 * vi的形参类型为ElemType，与bo2-1.c中相应函数的形参类型ElemType&不同 
 * 初始条件：线性表L已存在 
 * 操作结果:依次对L的每个数据元素调用函数vi()。一旦vi()失败,则操作失败 
 */ 
Status ListTraverse(LinkList L, void(*vi)(ElemType))
{
	LinkList p = L->next;

	while (p) {
		vi(p->data);
		p = p->next;
	}
	printf("\n");

	return OK;
}
