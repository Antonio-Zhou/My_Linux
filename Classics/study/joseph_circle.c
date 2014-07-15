/*
 * 	joseph_circle.c
 *
 *	Created on:	2014-07-13
 *	    Author:	Eric Chou
 *
 * 约瑟夫环是一个数学的应用问题:
 * 	已知n个人(以编号1,2,3...n分别表示)围坐在一张圆桌周围.从编号为k的人开始报数,数到m的那个人出列;
 * 	他的下一个人又从1开始报数,数到m的那个人又出列.
 * 	依此规律重复下去,直到圆桌周围的人全部出列.
 * */

#include <stdio.h>
#include <malloc.h>

struct joseph {
	int num;
	struct joseph *next;
};

/*
 * 思想:建立一个有N个元素的循环链表,然后从链表头开始遍历并记数,如果计数i==m(i初始为1)踢出元素,继续循环,当前元素与下一元素相同时退出循环.
 * */

/*创建约瑟夫环*/
void create_ring(struct joseph *pHead, int count)
{
	struct joseph *pCurr = NULL, *pPrev = NULL;
	int num = 1;

	pPrev = pHead;
	while (--count > 0) {
		pCurr = (struct joseph *)malloc(sizeof(struct joseph));
		num++;
		pCurr->num = num;
		pPrev->next = pCurr;
		pPrev = pCurr;
	}

	pCurr->next = pHead;  /*构成环状链表*/
}

void print_ring(struct joseph *pHead)
{
	struct joseph *pCurr;

	printf("%d", pHead->num);
	pCurr = pHead->next;

	while (pCurr != NULL) {
		if (pCurr->num == 1)
			break;
		printf("\n%d", pCurr->num);
		pCurr = pCurr->next;
	}
}

void kick_from_ring(struct joseph *pHead, int kick)
{
	struct joseph *pCurr, *pPrev;
	int count = 1, i = 0;

	pCurr = pPrev = pHead;

	while (pCurr != NULL) {
		if (count == kick) {
			/*踢出环*/
			printf("%d ", pCurr->num);
			pPrev->next = pCurr->next;
			free(pCurr);
			pCurr = pPrev->next;
			count = 1;
			i++;
			if (i % 10 == 0)
				printf("\n");
		}
		pPrev = pCurr;
		pCurr = pCurr->next;
		/*最后一个*/
		if (pPrev == pCurr) {
			free(pCurr);
			break;
		}
		count++;
	}
}

int main()
{
	int m = 0, n = 0, i, s = 0;
	struct joseph *pHead = NULL;

	printf("---------------Josephus Ring---------------\n");
	while (1) {
		printf("N(person count) = ");
		scanf("%d", &n);
		printf("M(out number) = ");
		scanf("%d", &m);
		if (n <= 0 || m <= 0) {
			printf("Input Error,intput again\n");
			continue;
		} else {
			break;
		}
	}

	/*建立链表*/
	pHead = (struct joseph *)malloc(sizeof(struct joseph));
	pHead->num = 1;
	pHead->next = NULL;
	create_ring(pHead, n);

	/*开始出圈*/
	printf("Kick Order:\n");
	kick_from_ring(pHead, m);

	/*计算出胜者*/
	for (i = 2; i <= n; i++)
		s = (s + m) % i;
	printf ("The winner is %d\n", s + 1);

	return 0;
}
