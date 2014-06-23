/*http://blog.csdn.net/hackbuteer1/article/details/6583988*/
#include <stdio.h>
#include <stdlib.h>

#define STACKSIZE 100
#define QUEUESIZE 100  /*队列初始容量*/

/*定义二叉树*/
typedef struct node {
	int data;
	struct node *lchild, *rchild;
}BinTnode, *BinTree;

typedef struct Queue{
	BinTree data[QUEUESIZE];
	int front;		/*队列的头指针*/
	int rear;		/*队列的尾指针*/
}queue;

/*栈的相关操作*/
typedef struct{
	BinTree data[STACKSIZE];
	int top;
}SeqStack;

/*初始化栈*/
void initStack(SeqStack *S)
{
	S->top=-1;
}

/*
 * 进栈
 * 无论是进栈还是取栈顶元素都应该是指向树的指针
 * */
void Push(SeqStack *S, BinTree p)
{
	if (S->top == STACKSIZE - 1) {
		printf("the stack is over flow\n");
	} else {
		S->top = S->top + 1;
		S->data[S->top] = p;
	}
}

/*
 * 出栈
 * */
int Pop(SeqStack *S, BinTree *p)
{
	if (S->top == -1) {
		printf("the stack is under flow\n");
		return 0;
	} else {
		*p = S->data[S->top];
		--S->top;
		return 1;
	}
}

/*
 * 判断栈是不是空
 * */
int EmptyStack(SeqStack S)
{
	if (S.top == -1)
		return 1;
	else
		return 0;
}

/*取出栈顶元素*/
int GetTop(SeqStack S, BinTree *p)
{
	if (S.top==-1) {
		printf("the stack is empty\n");
		return 0;
	} else {
		*p = S.data[S.top];
		return 1;
	}
}

/*访问结点*/
char visit(BinTree p)
{
	return (*p).data;
}

/*************Queue**********************/
int CreateQueue(queue *q)
{
	q->front = -1;
	q->rear = -1;
}

int EnQueue(queue *q, BinTree c)		/*将元素c插入队尾*/
{
	q->rear++;		/*尾指针加1*/
	if (q->rear >= QUEUESIZE) {	/*若尾指针超出队列长度则提示错误*/
		printf("Queue overflow!\n");
		exit(0);
	}
	q->data[q->rear] = c;
	return 1;
}

int DeQueue(queue *q, BinTree *ret)			/*队头元素出队并用ret返回其值*/
{
	if (q->front == q->rear) {		/*若队列为空，则返回错误提示*/
		return 0;
	}
	q->front++;		/*头指针加1*/
	*ret = q->data[q->front];

	return 1;
}

int QueueEmpty(queue *q)
{ /* 若Q为空队列,则返回TRUE,否则返回FALSE */
	if (q->front == q->rear)
		return 1;
	else
		return 0;
}

/*创建二叉树*/
void CreateBinTree(BinTree *T)
{
	char ch;
	scanf("\n%c", &ch);
	if (ch == '#') {
		*T = NULL;
	} else {
		*T = (BinTnode *)malloc(sizeof(BinTnode));
		(*T)->data = ch;
		printf("请输入%c的左孩子：", ch);
		CreateBinTree(&((*T)->lchild));
		printf("请输入%c的右孩子：", ch);
		CreateBinTree(&((*T)->rchild));
	}
}

/*
 * 前序遍历
 * */
void pre_order(BinTree root)
{
	if (root != 0) {
		printf("%c ", root->data);
		pre_order(root->lchild);
		pre_order(root->rchild);
	}
}

/*
 * 中序遍历
 * */
void in_order(BinTree root)
{
	if (root != 0) {
		in_order(root->lchild);
		printf("%c ", root->data);
		in_order(root->rchild);
	}
}

/*
 * 后序遍历
 * */
void post_order(BinTree root)
{
	if (root != 0) {
		post_order(root->lchild);
		post_order(root->rchild);
		printf("%c ", root->data);
	}
}
/*中序非递归遍历*/
void InorderTransverse(BinTree T)
{
	SeqStack S;
	BinTree p;

	initStack(&S);
	if(!T)    
		return;    
	Push(&S, T);

	while (!EmptyStack(S)) {
		while (GetTop(S, &p) && p)
			Push(&S, p->lchild);
		Pop(&S, &p);
		if(!EmptyStack(S)){
			Pop(&S, &p);
			printf("%c ", visit(p));
			Push(&S, p->rchild);
		}
	}
}

void InorderTransverse_1(BinTree T)
{
	SeqStack S;
	initStack(&S);

	if(!T)
		return;
	BinTree curr = T;    // 指向当前要检查的节点
	while (curr != NULL || !EmptyStack(S)) {
		while (curr != NULL) {
			Push(&S, curr);
			curr = curr->lchild;
		}
		if (!EmptyStack(S)) {
			Pop(&S,&curr);
			printf("%c ", curr->data);
			curr = curr->rchild;
		}
	}
}

void InorderTransverse_2(BinTree T)   // 中序遍历的非递归    
{
	SeqStack S;
	initStack(&S);
	if(!T)
		return;
	BinTree curr = T->lchild;    // 指向当前要检查的节点
	Push(&S, T);
	while (curr != NULL || !EmptyStack(S)) {
		while (curr != NULL) {   // 一直向左走   
		Push(&S, curr);
		curr = curr->lchild;
	}

	Pop(&S, &curr);
	printf("%c ", curr->data);
	curr = curr->rchild;
	}
}

/*  
 *  二叉树的非递归前序遍历，前序遍历思想：先让根进栈，只要栈不为空，就可以做弹出操作，  
 *  每次弹出一个结点，记得把它的左右结点都进栈，记得右子树先进栈，这样可以保证右子树在栈中总处于左子树的下面。  
 *  */  
/*先序非递归遍历*/
void PreorderTransverse(BinTree T)
{
	SeqStack S;
	BinTree p;
	if(!T)
		return;
	initStack(&S);
	Push(&S, T);
	printf("先序非递归序列是:");
	while (!EmptyStack(S)) {
		Pop(&S, &p);
		if (p != NULL) {
			printf("%c ", visit(p));
			Push(&S, p->rchild);
			Push(&S, p->lchild);
		}
	}
}

void PreorderTransverse_1(BinTree T)     //先序遍历的非递归   
{
	SeqStack S;
	BinTree curr = NULL;
	initStack(&S);
	if (!T)
		return;
	curr = T;
	while (curr != NULL || !EmptyStack(S)) {
		while (curr != NULL) {
			printf("%c ", curr->data);
			Push(&S, curr);
			curr = curr->lchild;
		}
		if (!EmptyStack(S)) {
			Pop(&S, &curr);
			curr = curr->rchild;
		}
	}
}

void PreorderTransverse_2(BinTree T)     //先序遍历的非递归    
{
	SeqStack S;
	initStack(&S);

	if (!T)
		return;

	while (T) {
		Push(&S, T);
		printf("%c ", T->data);
		T = T->lchild;
	}

	while (!EmptyStack(S)) {
		BinTree temp = NULL;  // 栈顶元素的右子树
		Pop(&S, &temp);                        // 弹出栈顶元素
		temp = temp->rchild;
		while (temp) {         // 栈顶元素存在右子树，则对右子树同样遍历到最下方
			printf("%c ", temp->data);
			Push(&S, temp);
			temp = temp->lchild;
		}
	}
}

/*后序非递归遍历*/
void PostorderTransverse(BinTree T)
{
	SeqStack S;
	BinTree p,q;
	initStack(&S);
	p = T;
	while (p || !EmptyStack(S)) {
		if (p != q) {
			while (p != NULL) {
				Push(&S, p);
				if (p->lchild != NULL)
					p = p->lchild;
				else
					p = p->rchild;
			}
		}
		if (EmptyStack(S))
			break;
		GetTop(S, &q);
		if (q->rchild == p) {
			Pop(&S, &p);
			printf("%c ", visit(p));
			p = q;
		} else {
			p = q->rchild;
		}
	}
}

void PostorderTransverse_1(BinTree T)  // 后序遍历的非递归      
{      
	SeqStack S;
	initStack(&S);
	BinTree curr = T ;           // 指向当前要检查的节点
	BinTree previsited = NULL;    // 指向前一个被访问的节点
	while (curr != NULL || !EmptyStack(S)) {  // 栈空时结束
		while (curr != NULL) {          // 一直向左走直到为空
			Push(&S, curr);
			curr = curr->lchild;
		}
		GetTop(S, &curr);
		// 当前节点的右孩子如果为空或者已经被访问，则访问当前节点
		if (curr->rchild == NULL || curr->rchild == previsited) {
			printf("%c ", curr->data);
			previsited = curr;
			Pop(&S, &curr);
			curr = NULL;
		} else {
			curr = curr->rchild;      // 否则访问右孩子
		}
	}
}

void PostorderTransverse_2(BinTree T)  // 后序遍历的非递归     双栈法    
{      
	SeqStack S1, S2;
	initStack(&S1);
	initStack(&S2);
	BinTree curr, p;           // 指向当前要检查的节点
	Push(&S1, T);
	while (!EmptyStack(S1)) {  // 栈空时结束
		Pop(&S1, &curr);
		Push(&S2, curr);
		if (curr->lchild)
			Push(&S1, curr->lchild);
		if (curr->rchild)
			Push(&S1, curr->rchild);
	}
	while (!EmptyStack(S2)) {
		GetTop(S2, &p);
		printf("%c ", p->data);
		Pop(&S2, &p);
	}
}

/* 
后序遍历由于遍历父节点是在遍历子节点之后，而且左节点和右节点遍历后的行为不一样， 
所以需要用变量来记录前一次访问的节点，根据前一次节点和现在的节点的关系来确定具体执行什么操作 
*/  
void PostorderTransverse_3(BinTree T)  
{  
	SeqStack S;
	initStack(&S);
	if (!T)
		return;
	BinTree prev = NULL ,curr = NULL;
	Push(&S, T);
	while (!EmptyStack(S)) {
		GetTop(S, &curr);
		if (prev == NULL || prev->lchild == curr || prev->rchild == curr) {
			if(curr->lchild != NULL)
				Push(&S, curr->lchild);
			else if(curr->rchild != NULL)
				Push(&S, curr->rchild);
		} else if (curr->lchild == prev) {
			if (curr->rchild != NULL)
				Push(&S, curr->rchild);
		} else {
			printf("%c ", curr->data);
			Pop(&S, &curr);
		}
		prev = curr;
	}
}

int LayerTravelBitTree(BinTree T)		/*层次遍历二叉树*/
{
	queue  tq;		/*队列tq*/
	BinTree  res;		/*用来返回对头元素*/

	CreateQueue(&tq);		/*创建队列*/
	EnQueue(&tq, T);		/*将T插入队尾*/
	while (DeQueue(&tq,&res) == 1) {		/*当队头元素不空时执行while循环*/
		if (res) {
			printf("%c ", res->data);		/*访问队头元素*/
			EnQueue(&tq,res->lchild);		/*将结点左孩子插入队尾*/
			EnQueue(&tq,res->rchild);		/*将结点右孩子插入队尾*/
		}
	}
}

#if 0
void LeverTraverse(BinTree T)   //方法一、非递归层次遍历二叉树     
{    
	queue  tq;		/*队列tq*/
	CreateQueue(&tq);		/*创建队列*/
    BinTree p;    
    p = T;    
    if (p != NULL)    
	EnQueue(&tq, p);		/*将T插入队尾*/
    while (!QueueEmpty(&tq)) {
	   DeQueue(&tq, &p);
        if(visit(p->lchild) == 1)
		printf("%c ", p->data);		/*访问队头元素*/
		EnQueue(&tq,p->lchild);
        if(visit(p->rchild) == 1)
		EnQueue(&tq,p->rchild);
    }    
}    
#endif
void LevelOrder(BinTree T)     //方法二、非递归层次遍历二叉树
{
	queue tq;
	BinTree res;
	if (T == NULL)
		return;
	CreateQueue(&tq);		/*创建队列*/
	EnQueue(&tq, T);		/*将T插入队尾*/
	while (DeQueue(&tq,&res) == 1) { //如果队尾指针不等于对头指针时
		printf("%c ", res->data);
		if(res->lchild!=NULL)  //将队首结点的左孩子指针入队列
			EnQueue(&tq,res->lchild);
		if(res->rchild!=NULL)
			EnQueue(&tq,res->rchild);
	}
}

int depth(BinTree T)   //树的深度
{
	int d1,d2;
	if(!T)
		return 0;

	d1 = depth(T->lchild);
	d2 = depth(T->rchild);

	return (d1>d2?d1:d2)+1;
}

int CountNode(BinTree T)
{
	if(T == NULL)
		return 0; 
	return (1 + CountNode(T->lchild) + CountNode(T->rchild));
}

void main()
{
	BinTree root = NULL;
	int flag = 1, k;
	printf("                     本程序实现二叉树的基本操作.\n");
	printf("可以进行建立二叉树,递归先序,中序,后序遍历,非递归先序,中序遍历及非递归层序遍历等操作.\n");

	while (flag) {
		printf("\n");
		printf("|--------------------------------------------------------------|\n");
		printf("|                    二叉树的基本操作如下:                     |\n");
		printf("|                        0.创建二叉树                          |\n");
		printf("|                        1.递归先序遍历                        |\n");
		printf("|                        2.递归中序遍历                        |\n");
		printf("|                        3.递归后序遍历                        |\n");
		printf("|                        4.非递归先序遍历                      |\n");
		printf("|                        5.非递归中序遍历                      |\n");
		printf("|                        6.非递归后序遍历                      |\n");
		printf("|                        7.非递归层序遍历                      |\n");
		printf("|                        8.二叉树的深度                        |\n");
		printf("|                        9.二叉树的结点个数                    |\n");
		printf("|                        10.退出程序                           |\n");
		printf("|--------------------------------------------------------------|\n");
		printf("                        请选择功能：");
		scanf("%d", &k);
		switch (k) {
		case 0: 
			printf("请建立二叉树并输入二叉树的根节点：");
			CreateBinTree(&root);
			break;
		case 1:
			if (root) {
				printf("递归先序遍历二叉树的结果为：");
				pre_order(root);
				printf("\n");
			} else {
				printf("          二叉树为空！\n");
			}
			break;    
		case 2:    
			if (root) {    
				printf("递归中序遍历二叉树的结果为：");
				in_order(root);
				printf("\n");
			} else {
				printf("          二叉树为空！\n");
			}
			break;
		case 3:
			if (root) {
				printf("递归后序遍历二叉树的结果为：");
				post_order(root);
				printf("\n");
			} else {
				printf("          二叉树为空！\n");
			}
			break;
		case 4:
			if (root) {
				printf("非递归先序遍历二叉树：");
				PreorderTransverse(root);
				printf("\n");
			} else {
				printf("          二叉树为空！\n");
			}
			break;    
		case 5:
			if (root) {
				printf("非递归中序遍历二叉树：");
				InorderTransverse(root);
				printf("\n");
			} else {
				printf("          二叉树为空！\n");
			}
			break;
		case 6:
			if (root) {
				printf("非递归后序遍历二叉树：");
				PostorderTransverse(root);
				printf("\n");
				PostorderTransverse_3(root);
				printf("\n");
			} else {
				printf("          二叉树为空！\n");
			}
			break;
		case 7:
			if (root) {
				printf("非递归层序遍历二叉树：");
				LayerTravelBitTree(root);
				printf("\n");
				LevelOrder(root);
				printf("\n");
			} else {
				printf("          二叉树为空！\n");
			}
			break;
		case 8:
			if(root)
				printf("这棵二叉树的深度为：%d\n", depth(root));
			else
				printf("          二叉树为空！\n");
			break;
		case 9:
			if(root)
				printf("这棵二叉树的结点个数为：%d\n", CountNode(root));
			else
				printf("          二叉树为空！\n");
			break;
		default:
			flag=0;
			printf("程序运行结束，按任意键退出！\n");
		}
	}
}
