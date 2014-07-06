#include <stdio.h>
#include <malloc.h>
#include <stdlib.h>
#include <assert.h>

/*函数声明*/
struct LNode *init_list(struct LNode **head);
struct LNode *create_list(struct LNode *head);
struct LNode *create_list_2(struct LNode *head);
int insert_list(struct LNode *head, int pos, int e);
struct LNode *get_elem(struct LNode *head, int pos);
int search_elem(struct LNode *head, int e);
int delete_elem(struct LNode *head, int pos);
int clear_list(struct LNode *head);
void destory_list(struct LNode **head);
int empty_list(struct LNode *head);
int length_list(struct LNode *head);
struct LNode *get_privor(struct LNode *head, int e);
struct LNode *get_next(struct LNode *head, int e);
void printf_list(struct LNode *head);
struct LNode *reserve_list(struct LNode *head);
struct LNode *reserve_list_2(struct LNode *head);
void reserve_list_3(struct LNode *head);
void merge_list(struct LNode *La, struct LNode **Lb, struct LNode **Lc);
void move_node(struct LNode **destRef, struct LNode **sourceRef);
struct LNode *sorted_merge(struct LNode *a, struct LNode *b);
struct LNode *sorted_merge_2(struct LNode *a, struct LNode *b);
struct LNode *sorted_merge_3(struct LNode *a, struct LNode *b);
void selection_menu(void);
void select_function(int choice, int *flag);

struct LNode {
	int elem;
	struct LNode *next;
};

struct LNode *list, *list2, *list3, *list_merge;

int main(void)
{
	int choice, flag = 1;

	selection_menu();

	while (flag) {
		printf("please select the function:");
		scanf("%d", &choice);
		select_function(choice, &flag);
	}

	return 0;
}

void selection_menu(void)
{
	printf("本程序实现带头结点单链表的基本操作.\n");
	printf("		可以进行建立链表,查询,删除,反转等操作.\n");
	printf("\n");
	printf("|---------------------------------------------------------------|\n");
	printf("|		链表的基本操作如下:				|\n");
	printf("|			0.创建链表				|\n");
	printf("|			1.根据位置查看元素的值			|\n");
	printf("|			2.根据值查看元素的位置			|\n");
	printf("|			3.查看元素前驱				|\n");
	printf("|			4.查看元素后驱				|\n");
	printf("|			5.插入元素				|\n");
	printf("|			6.根据元素位置删除元素			|\n");
	printf("|			7.反转链表(三种方法)			|\n");
	printf("|			8.合并两个有序链表			|\n");
	printf("|			9.清空链表所有元素,并判断是否为空	|\n");
	printf("|			10.打印链表所有元素			|\n");
	printf("|			11.链表长度				|\n");
	printf("|			12.销毁链表				|\n");
	printf("|			13.退出程序				|\n");
	printf("|---------------------------------------------------------------|\n");
}

void select_function(int choice, int *flag)
{
	struct LNode *p = NULL;
	int pos, e, ret;

	switch (choice) {
	case 0:
		printf("input the elem of list:\n");
		list = create_list(list);
		if (list == NULL)
			printf("Create list fail!\n");
		else
			printf("Create list success!\n");
		break;
	case 1:
		printf("Please input the position you want to find:\n");
		scanf("%d", &pos);

		p = get_elem(list, pos);
		if (p == NULL)
			printf("the position is beyond the list\n");
		else
			printf("the pos elem is %d\n", p->elem);
		break;
	case 2:
		printf("Please input the elem you want to find:\n");
		scanf("%d", &e);

		ret = search_elem(list, e);
		if (ret == -1)
			printf("The elem is not found\n");
		else
			printf("The elem is found, and the position is %d\n", ret);
		break;
	case 3:
		printf("Please input the elem you want to find:\n");
		scanf("%d", &e);

		p = get_privor(list, e);
		if (p == NULL)
			printf("can not find the elem's priv\n");
		else
			printf("find the elem's priv, is %d\n", p->elem);
		break;
	case 4:
		printf("Please input the elem you want to find:\n");
		scanf("%d", &e);

		p = get_next(list, e);
		if (p == NULL)
			printf("can not find the elem's next\n");
		else
			printf("find the elem's next, is %d\n", p->elem);
		break;
	case 5:
		printf("Please input the position you want insert, and the elem:\n");
		scanf("%d, %d", &pos, &e);

		ret = insert_list(list, pos, e);
		if ( ret == -1)
			printf("the priv of the elem is not exist, and insert error\n");
		else
			printf("insert error\n");
		break;
	case 6:
		printf("Please input the position you want to delete:\n");
		scanf("%d", &pos);

		ret = delete_elem(list, pos);
		if (ret == -1)
			printf("can not find the elem or its priv,delete error!\n");
		else
			printf("delete success!\n");
		break;
	case 7:
		p = reserve_list_2(list);
		if (p == NULL)
			printf("the list not exist,please enter 0 to create\n");
		else {
			printf("After reserve, the list is:\n");
			printf_list(p);
		}
		break;
	case 8:
		printf("input the elem of list2 to merge:\n");
		list2 = create_list(list2);
		if (list2 == NULL)
			printf("Create list2 fail!\n");
		else
			printf("Create list2 success!\n");
		printf_list(list2);

		printf("input the elem of list3 to merge:\n");
		list3 = create_list(list3);
		if (list3 == NULL)
			printf("Create list3 fail!\n");
		else
			printf("Create list3 success!\n");
		printf_list(list3);

		list_merge = sorted_merge_2(list2, list3);

		printf_list(list_merge);
		break;
	case 9:
		ret = clear_list(list);
		if (ret == -1)
			printf("the list not exist, please enter 0 to create\n");
		else {
			if (empty_list(list) == 0)
				printf("The list is empty, clear success\n");
			else
				printf("The list is not empty, clear fail\n");
		}
		break;
	case 10:
		printf_list(list);
		break;
	case 11:
		ret = length_list(list);
		printf("the list's length is %d\n", ret);
		break;
	case 12:
		destory_list(&list);
		break;
	default:
		*flag = 0;
		printf("the program is over ,enter any key to exit\n");
	}
}

struct LNode *init_list(struct LNode **head)
{
	*head = (struct LNode *)malloc(sizeof(struct LNode));
	if (!*head)
		return NULL;

	(*head)->next = NULL;
	return *head;
}

/*单链表的建立2，尾插法建立单链表*/
struct LNode *create_list(struct LNode *head)
{
	int e;
	struct LNode *new, *plist, *last;

	plist = init_list(&head);
	if (plist == NULL) {
		printf("init list error\n");
		exit(-1);
	}

	last = plist;				/*r始终指向终端结点,开始时指向头结点*/
	while (scanf("%d", &e)) {
		new = (struct LNode *)malloc(sizeof(struct LNode));
		if (!new) {
			printf("alloc new list node fail\n");
			exit(-1);
		}

		new->elem = e;
		last->next = new;		/*将结点插入到表头L-->|1|-->|2|-->NULL*/
		last = new;

		if (getchar() == '\n')
			break;
	}

	last->next = NULL;

	return plist;
}

/*单链表的建立1，头插法建立单链表*/
struct LNode *create_list_2(struct LNode *head)
{
	struct LNode *plist, *new;
	int e;

	plist = init_list(&head);
	if (plist == NULL) {
		printf("init list error\n");
		exit(-1);
	}

	while (scanf("%d", &e)) {
		new = (struct LNode *)malloc(sizeof(struct LNode));
		if (!new) {
			printf("alloc new list node fail\n");
			exit(-1);
		}

		new->elem = e;
		new->next = plist->next;		/*将结点插入到表头L-->|2|-->|1|-->NULL*/
		plist->next = new;

		if (getchar() == '\n')
			break;
	}

	return plist;
}

int insert_list(struct LNode *head, int pos, int e)
{
	/*遍历链表*/
	struct LNode *p = NULL;
	struct LNode *s = NULL;

	/*找到pos-1*/
	p = get_elem(head, pos-1);

	if (!p) {
		return -1;
	}

	/*利用s插入e*/
	s = (struct LNode *)malloc(sizeof(struct LNode));
	s->elem = e;
	s->next = p->next;
	p->next = s;

	return 0;
}

/*按位置查找*/
struct LNode *get_elem(struct LNode *head, int pos)
{
	int j = 0;
	struct LNode *p = head;

	while (p && (j < pos)) {
		p = p->next;
		j++;
	}

	if (!p || (j > pos))
		return NULL;

	return p;
}

/*通过位置删除*/
int delete_elem(struct LNode *head, int pos)
{
	struct LNode *p = NULL;
	struct LNode *s = NULL;

	p = get_elem(head, pos - 1);

	if (!p || !p->next)
		return -1;

	s = p->next;
	p->next = s->next;
	free(s);

	return 0;
}

int clear_list(struct LNode *head)
{
	struct LNode *p = NULL, *s = NULL;

	if (head == NULL)
		return -1;

	p = head;
	while (p->next != NULL) {
		s = p->next;
		free(p);
		p = s;
	}
	head->next = NULL;

	return 0;
}

void destory_list(struct LNode **head)
{
	struct LNode *p = NULL;

	while (*head != NULL) {
		p = (*head)->next;
		free(*head);
		*head = p;
	}
}

int empty_list(struct LNode *head)
{
	if (head->next == NULL)
		return 0;
	else
		return 1;
}

int length_list(struct LNode *head)
{
	struct LNode *p = head->next;
	int len = 0;

	while (p != NULL) {
		p = p->next;
		len++;
	}

	return len;
}

/*按值查找*/
int search_elem(struct LNode *head, int e)
{
	struct LNode *p = head->next;
	int pos = 0;

	while (p != NULL && (p->elem != e)) {
		p = p->next;
		pos++;
	}

	if (!p)
		pos = -1;

	return pos;
}

/*根据值找到priv*/
struct LNode *get_privor(struct LNode *head, int e)
{
	struct LNode *p = head;
	struct LNode *s = p->next;

	while (p != NULL && (s->elem != e)) {
		p = s;
		s = p->next;
	}

	return p;
}

/*根据值找到next*/
struct LNode *get_next(struct LNode *head, int e)
{
	struct LNode *p = head;

	while (p->next != NULL && (p->elem != e))
		p = p->next;

	return p->next;
}

void printf_list(struct LNode *head)
{
	struct LNode *p = head;

	while (p->next) {
		p = p->next;
		printf("%d ", p->elem);
	}

	printf("\n");
}

/*
 * 逆置算法思路:
 * 先将head->next置为null,然后将链表的节点依次插入到head后,并与已插入的节点链接起来,就实现了逆置
 * 依次取原链表中每个结点,将其作为第一个结点插入到新的链表中去,指针p用来指向原表中当前结点,p为空时结束
 * 整个算法思想就是把每个结点都插到头结点之后,那两步表示把当前访问结点P直接插到头结点之后
 * */
struct LNode *reserve_list(struct LNode *head)
{
	struct LNode *p = head->next;
	struct LNode *s = NULL;

	head->next = NULL;

	while (p) {
		s = p;
		p = p->next;
		s->next = head->next;	/*将新插入节点与已插入的节点链接起来*/
		head->next = s;		/*将新插入节点与head节点链接起来*/
	}

	return head;
}

/*
 * 分析:
 * 可以用交换数据的方式来达到逆置的目的.
 * 但由于是单链表,可以利用指针改指来达到表逆置的目的.
 * 具体情况入下:
 * (1)当链表为空表或只有一个结点时,该链表的逆置链表与原表相同
 * (2)当链表含2个以上结点时,可将该链表处理成只含第一结点的带头结点链表和一个无头结点的包含该链表剩余结点的链表.
 * 	然后将该无头结点链表中的所有结点顺着链表指针,由前往后将每个结点依次从无头结点链表中摘下,作为第一个结点插入到带头结点链表中.
 * 	这样就可以得到逆置的链表
 * */
struct LNode *reserve_list_2(struct LNode *head)
{
	/*设置两个临时指针变量*/
	struct LNode *p, *q ;

	 /*当链表不是空表或单结点时*/
	if (head->next && head->next->next) {
		p = head->next;
		q = p->next;
		p->next = NULL;		/*将开始结点变成终端结点*/
		while (q) {		/*每次循环将后一个结点变成开始结点*/
			p = q;
			q = q->next;
			p->next = head-> next;
			head->next = p;
		}
		return head;
	}

	return head;			/*如是空表或单结点表,直接返回head*/
}

void reserve_list_3(struct LNode *head)
{
	struct LNode *p, *q, *s;

	p = head->next;
	if (p && p->next) {
		q = p->next;
		p->next = NULL;
	}

	while (q) {
		s = q->next;
		q->next = p;
		p = q;
		q = s;
	}

	head->next = p;
}

/*
 * 已知单链线性表La和Lb的元素按值非递减排列
 * 归并La和Lb得到新的单链线性表Lc，Lc的元素也按值非递减排列
 * */
void merge_list(struct LNode *La, struct LNode **Lb, struct LNode **Lc)
{
	struct LNode *pa = La->next, *pb = (*Lb)->next, *pc;

	/*用La的头结点作为Lc的头结点*/
	*Lc = pc = La;
	while (pa && pb) {
		if (pa->elem <= pb->elem) {
			pc->next = pa;
			pc = pa;
			pa = pa->next;
		} else {
			pc->next = pb;
			pc = pb;
			pb = pb->next;
		}
	}
	pc->next = pa ? pa : pb;	/* 插入剩余段 */
	free(*Lb);			/* 释放Lb的头结点 */
	Lb = NULL;
}

#if 0
struct list *list_merge(struct list *head1, struct list *head2)
{
	struct list *res;

	if (head1 == NULL) return head2;
	if (head2 == NULL) return head1;

	if (head1->key < head2->key) {
		res = head1;
		printf("res1 %d\n",res->key);
		res->next = list_merge(head1->next, head2);
	} else {
		res = head2;
		printf("res2 %d\n",res->key);
		res->next = list_merge(head1, head2->next);
	}

	return res;
}


struct list *list_merge2(struct list *head1, struct list *head2)
{
	struct list *head,*p1,*p2, *pcurrent;

	if (head1 == NULL)
		return head2;

	if (head2 == NULL)
		return head1;

	if (head1->key < head2->key) {
		head = head1;
		p1 = head1->next;
		p2 = head2;
	} else {
		head = head2;
		p2 = head2->next;
		p1 = head1;
	}

	pcurrent = head;
	while (p1 != NULL && p2 != NULL) {
		if (p1->key <= p2->key) {
			pcurrent->next = p1;
			pcurrent = p1;
			p1 = p1->next;
		} else {
			pcurrent->next = p2;
			pcurrent = p2;
			p2 = p2->next;
		}
	}

	if (p1 != NULL)
		pcurrent->next = p1;
	if (p2 != NULL)
		pcurrent->next = p2;

	return head;
}
#endif

/*
 * pull off the front node of the source and put it in dest
 * MoveNode() function takes the node from the front of the source, and move it to the front of the dest.
 * It is an error to call this with the source list empty.
 * 	Before calling MoveNode():
 * 	source == {1, 2, 3}
 * 	dest == {1, 2, 3}
 *
 * 	After calling MoveNode():
 * 	source == {2, 3}
 * 	dest == {1, 1, 2, 3}
 * */
void move_node(struct LNode **destRef, struct LNode **sourceRef)
{
	/* the front source node */
	struct LNode *newNode = *sourceRef;
	assert(newNode != NULL);

	/*Advance the source pointer */
	*sourceRef = newNode->next;
	/* Link th eold dest off the new node */
	newNode->next = *destRef;
	/*Move dest to point to the new node */
	*destRef = newNode;
}

/*
 * see http://www.geeksforgeeks.org/merge-two-sorted-linked-lists/
 * Takes two lists sorted in creasing order, and splices their nodes together to
 * make ont big sorted list which is returned.
 * */
struct LNode *sorted_merge(struct LNode *a, struct LNode *b)
{
	/* a dummy first node to hang the result on */
	struct LNode dummy;

	/* tail points to the last result node */
	struct LNode* tail = &dummy;

	/*so tail->next is the places to add new nodes to the result*/
	dummy.next = NULL;
	while (1) {
		if (a == NULL) {
			tail->next = b;
			break;
		} else if (b == NULL) {
			tail->next = a;
			break;
		}

		if (a->elem <= b->elem)
			move_node(&(tail->next), &a);
		else
			move_node(&(tail->next), &b);

		tail = tail->next;
	}

	return (dummy.next);
}

/* method2 Using local References */
struct LNode *sorted_merge_2(struct LNode *a, struct LNode *b)
{
	struct LNode *result = NULL;

	/*point to the last result pointer */
	struct LNode **lastPtrRef = &result;

	while (1) {
		if (a == NULL) {
			*lastPtrRef = b;
			break;
		} else if(b == NULL) {
			*lastPtrRef = a;
			break;
		}

		if (a->elem <= b->elem)
			move_node(lastPtrRef, &a);
		else
			move_node(lastPtrRef, &b);
		/*tricky:advance to point to the next ".next" field */
		lastPtrRef = &((*lastPtrRef)->next);
	}

	return (result);
}

/*Using Recursion*/
struct LNode *sorted_merge_3(struct LNode *a, struct LNode *b)
{
	struct LNode *result = NULL;

	/*Base cases*/
	if (a == NULL)
		return (b);
	else if(b == NULL)
		return (a);

	/*Pick either a or b, and recur */
	if (a->elem <= b->elem) {
		result = a;
		result->next = sorted_merge_3(a->next, b);
	} else {
		result = b;
		result->next = sorted_merge_3(a, b->next);
	}

	return (result);
}
