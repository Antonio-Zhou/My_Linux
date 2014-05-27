/*
 * 	headlist_main.h
 *
 *	Created on:	2013-05-15
 *	    Author:	Eric Zhou
 */

/* 
 * 带头结点的线性链表操作1：
 * 分配由p指向的值为e的结点，并返回OK；若分配失败。则返回ERROR 
 */
Status MakeNode(Link *p,ElemType e)
{
	*p = (Link)malloc(sizeof(LNode));

	if (!*p)
		return ERROR;
	(*p)->data = e;

	return OK;
}

/* 
 * 带头结点的线性链表操作2：
 * 释放p所指结点 
 */
void FreeNode(Link *p)
{
	free(*p);
	*p = NULL;
}

/* 
 * 带头结点的线性链表操作3：
 * 构造一个空的线性链表 
 */
Status InitList(LinkList *L)
{
	Link p = (Link)malloc(sizeof(LNode)); /* 生成头结点 */

	if (p) {
		p->next = NULL;
		(*L).head = (*L).tail = p;
		(*L).len = 0;
		return OK;
	} else
		return ERROR;
}

/* 
 * 带头结点的线性链表操作4：
 * 将线性链表L重置为空表，并释放原链表的结点空间 
 */
Status ClearList(LinkList *L)
{
	Link p,q;

	if ((*L).head != (*L).tail) {
		p = q = (*L).head->next;
		(*L).head->next = NULL;
		while (p != (*L).tail) {
			p = q->next;
			free(q);
			q = p;
		}
		free(q);
		(*L).tail = (*L).head;
		(*L).len = 0;
	}

	return OK;
}

/*
 * 带头结点的线性链表操作5：
 * 销毁线性链表L，L不再存在 
 */
Status DestroyList(LinkList *L)
{
	ClearList(L); /* 清空链表 */
	FreeNode(&(*L).head);
	(*L).tail = NULL;
	(*L).len = 0;

	return OK;
}

/* 
 * 带头结点的线性链表操作6：
 * h指向L的一个结点，把h当做头结点，将s所指结点插入在第一个结点之前 
 * 形参增加L,因为需修改L 
 */
Status InsFirst(LinkList *L, Link h, Link s)
{
	s->next = h->next;
	h->next = s;
	/* h指向尾结点 */
	if (h == (*L).tail)
		(*L).tail = h->next; /* 修改尾指针 */
	(*L).len++;

	return OK;
}

/* 
 * 带头结点的线性链表操作7：
 * h指向L的一个结点，把h当做头结点，删除链表中的第一个结点并以q返回。
 * 若链表为空(h指向尾结点)，q=NULL，返回FALSE   
 * 形参增加L,因为需修改L 
 */
Status DelFirst(LinkList *L, Link h, Link *q)
{
	*q = h->next;
	/* 链表非空 */
	if (*q) {
		h->next = (*q)->next;
		/* 删除尾结点 */
		if (!h->next)
		(*L).tail = h; /* 修改尾指针 */
		(*L).len--;
		return OK;
	} else
		return FALSE; /* 链表空 */
}

/* 
 * 带头结点的线性链表操作8：
 * 将指针s(s->data为第一个数据元素)所指(彼此以指针相链,以NULL结尾)的一串结点链接在线性链表L的最后一个结点之后,
 * 并改变链表L的尾指针指向新的尾结点 
 */
Status Append(LinkList *L, Link s)
{
	int i = 1;
	(*L).tail->next = s;

	while (s->next) {
		s = s->next;
		i++;
	}
	(*L).tail = s;
	(*L).len += i;

	return OK;
}

/* 
 * 带头结点的线性链表操作9：
 * 已知p指向线性链表L中的一个结点，返回p所指结点的直接前驱的位置,若无前驱，则返回NULL 
 */
Position PriorPos(LinkList L, Link p)
{
	Link q;
	q = L.head->next;
	/* 无前驱 */
	if (q == p)
		return NULL;
	else {
		/* q不是p的直接前驱 */
		while (q->next != p)
			q = q->next;
		return q;
	}
}

/* 
 * 带头结点的线性链表操作10：
 * 删除线性链表L中的尾结点并以q返回，改变链表L的尾指针指向新的尾结点 
 */
Status Remove(LinkList *L, Link *q)
{
	Link p = (*L).head;
	/* 空表 */
	if ((*L).len == 0) {
		*q = NULL;
		return FALSE;
	}
	while (p->next != (*L).tail)
		p = p->next;
	*q = (*L).tail;
	p->next = NULL;
	(*L).tail = p;
	(*L).len--;

	return OK;
}

/* 
 * 带头结点的线性链表操作11：
 * 已知p指向线性链表L中的一个结点，将s所指结点插入在p所指结点之前,并修改指针p指向新插入的结点 
 */
Status InsBefore(LinkList *L, Link *p, Link s)
{
	Link q = PriorPos(*L,*p); /* q是p的前驱 */
	/* p无前驱 */
	if (!q)
		q = (*L).head;
	s->next = *p;
	q->next = s;
	*p = s;
	(*L).len++;

	return OK;
}

/* 
 * 带头结点的线性链表操作12：
 * 已知p指向线性链表L中的一个结点，将s所指结点插入在p所指结点之后，并修改指针p指向新插入的结点 
 */
Status InsAfter(LinkList *L, Link *p, Link s)
{
	/* 修改尾指针 */
	if (*p == (*L).tail)
		(*L).tail = s;
	s->next = (*p)->next;
	(*p)->next = s;
	*p = s;
	(*L).len++;

	return OK;
}

/* 
 * 带头结点的线性链表操作13：
 * 已知p指向线性链表中的一个结点，用e更新p所指结点中数据元素的值 
 */
Status SetCurElem(Link p, ElemType e)
{
	p->data = e;
	return OK;
}

/* 
 * 带头结点的线性链表操作14：
 * 已知p指向线性链表中的一个结点，返回p所指结点中数据元素的值 
 */
ElemType GetCurElem(Link p)
{
	return p->data;
}

/* 
 * 带头结点的线性链表操作15：
 * 若线性链表L为空表，则返回TRUE，否则返回FALSE 
 */
Status ListEmpty(LinkList L)
{
	if (L.len)
		return FALSE;
	else
		return TRUE;
}

/* 
 * 带头结点的线性链表操作16：
 * 返回线性链表L中元素个数 
 */
int ListLength(LinkList L)
{
	return L.len;
}

/* 
 * 带头结点的线性链表操作17：
 * 返回线性链表L中头结点的位置 
 */
Position GetHead(LinkList L)
{
	return L.head;
}

/* 
 * 带头结点的线性链表操作18：
 * 返回线性链表L中最后一个结点的位置 
 */
Position GetLast(LinkList L)
{
	return L.tail;
}

/* 
 * 带头结点的线性链表操作19：
 * 已知p指向线性链表L中的一个结点，返回p所指结点的直接后继的位置，若无后继，则返回NULL 
 */
Position NextPos(Link p)
{
	return p->next;
}

/* 
 * 带头结点的线性链表操作20：
 * 返回p指示线性链表L中第i个结点的位置，并返回OK，i值不合法时返回ERROR, i=0为头结点 
 */
Status LocatePos(LinkList L, int i, Link *p)
{
	int j;
	if (i < 0 || i > L.len)
		return ERROR;
	else {
		*p = L.head;
		for (j = 1; j <= i; j++)
			*p = (*p)->next;
		return OK;
	}
}

/* 
 * 带头结点的线性链表操作21：
 * 返回线性链表L中第1个与e满足函数compare()判定关系的元素的位置，若不存在这样的元素，则返回NULL 
 */
Position LocateElem(LinkList L, ElemType e, Status (*compare)(ElemType,ElemType))
{
	Link p = L.head;

	do
		p = p->next;
	while(p && !(compare(p->data, e))); /* 没到表尾且没找到满足关系的元素 */

	return p;
}

/* 
 * 带头结点的线性链表操作22：
 * 依次对L的每个数据元素调用函数visit()。一旦visit()失败，则操作失败 
 */
Status ListTraverse(LinkList L, void(*visit)(ElemType))
{
	Link p = L.head->next;
	int j;

	for (j = 1; j <= L.len; j++) {
		visit(p->data);
		p = p->next;
	}
	printf("\n");

	return OK;
}

/* 
 * 带头结点的线性链表操作23：
 * 已知L为有序线性链表，将元素e按非降序插入在L中。（用于一元多项式） 
 */
Status OrderInsert(LinkList *L, ElemType e, int (*comp)(ElemType,ElemType))
{
	Link o, p, q;

	q = (*L).head;
	p = q->next;
	/* p不是表尾且元素值小于e */
	while ((p != NULL) && (comp(p->data, e) < 0)) {
		q = p;
		p = p->next;
	}
	o = (Link)malloc(sizeof(LNode)); /* 生成结点 */
	o->data = e; /* 赋值 */
	q->next = o; /* 插入 */
	o->next = p;
	(*L).len++; /* 表长加1 */
	if (!p) /* 插在表尾 */
	(*L).tail = o; /* 修改尾结点 */

	return OK;
}

/* 
 * 带头结点的线性链表操作24：
 * 若表L中存在与e满足判定函数compare()取值为0的元素，则q指示L中第一个值为e的结点的位置，并返回TRUE；
 * 否则q指示第一个与e满足判定函数 
 * compare()取值>0的元素的前驱的位置。并返回FALSE。（用于一元多项式） 
 */
Status LocateElemP(LinkList L, ElemType e, Position *q, int(*compare)(ElemType,ElemType))
{
	Link p = L.head, pp;
	do {
		pp = p;
		p = p->next;
	}while(p && (compare(p->data,e) < 0)); /* 没到表尾且p->data.expn<e.expn */
	/* 到表尾或compare(p->data,e)>0 */
	if (!p || compare(p->data,e) > 0) {
		*q = pp;
		return FALSE;
	/* 找到 */
	} else {
		*q = p;
		return TRUE;
	}
}
