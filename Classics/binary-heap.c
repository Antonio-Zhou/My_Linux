#include <stdlib.h>
#include <assert.h>
#include <stdio.h>
#include <time.h>

#define bool int
#define true 1
#define false 0

/*打算采用数组的方式实现完全二叉堆*/
typedef struct BinaryHeap {
	int *parray;		/*因为需要动态扩展,采用静态数组不方便*/
	int currentSize;	/*目前存在的结点*/
	int capacity;		/*树的实际容量*/
}BinaryHeap_t;

bool is_empty(BinaryHeap_t *heap)
{
	assert(heap != NULL);
	return heap->currentSize == 0;
}

bool init_BinaryHeap(BinaryHeap_t *heap, int capacity)
{
	int *parray = NULL;

	if (heap == NULL)
		return false;
	parray = (int *)calloc(capacity+1, sizeof(int));
	if(parray == NULL)
		return false;

	heap->parray = parray;
	heap->capacity = capacity;
	heap->currentSize = 0;

	return true;
}

void delete_BinaryHeap(BinaryHeap_t *heap)
{
	assert(heap != NULL && heap->parray != NULL);

	heap->capacity = 0;
	heap->currentSize = 0;

	free(heap->parray);
	heap->parray = NULL;
}

void free_BinaryHeap(BinaryHeap_t **heap)
{
	assert(*heap != NULL);

	(*heap)->capacity = 0;
	(*heap)->currentSize = 0;

	free((*heap)->parray);
	(*heap)->parray = NULL;

	free(*heap);
	*heap = NULL;
}

bool alloc_BinaryHeap(BinaryHeap_t **heap, int capacity)
{
	int *parray = NULL;

	if(*heap != NULL)
		return false;

	*heap = (BinaryHeap_t *)calloc(1, sizeof(BinaryHeap_t));
	if(*heap == NULL)
		return false;

	/*其中的1,主要是为了使得数组从下标1开始计算*/
	parray =(int *)calloc(capacity + 1, sizeof(int));
	if(parray == NULL)
		return false;

	(*heap)->parray = parray;
	(*heap)->capacity = capacity;
	(*heap)->currentSize = 0;

	return true;
}

/*
 * 采用上虑法实现数据的插入操作
 * 1.首先创建一个空节点
 * 2.将需要插入的值与当前空穴的父结点进行比较.如果大于父结点.直接插入空穴中;否则,将父结点的值下拉到空穴中
 * 3.之前父结点的位置就是空穴,接着与上层比较,直到找到父结点大于当前插入值的情况
 * */
bool insert(BinaryHeap_t *heap, int value)
{
	int index = 0;

	if(heap == NULL || heap->parray == NULL)
	return false;

	/*得到一个新的空穴下标*/
	index = ++heap->currentSize;
	/*条件是不是第一个下标和插入值比对应父结点小*/
	while (index > 1 && value < heap->parray[index/2]) {
		/*将父结点保存到当前结点处*/
		heap->parray[index] = heap->parray[index/2];
		/*得到父结点的空穴位置*/
		index /= 2;
	}
	/*将插入的值保存到剩余的空穴中*/
	heap->parray[index] = value;

	return true;
}

/*
 * 下虑法实现数据的重排序操作
 * 将子结点的两个儿子进行比较,将小的提升.需要注意的是如何让判断节点是否一定存在右儿子
 * 实现的方式主要是利用了二叉堆的特性:
 * 2*pare = L_child;
 * 2*pare + 1 = R_child;
 * */
static void presort_BinaryHeap(BinaryHeap_t *heap,int hole)
{
	/*这是二叉堆的特性*/
	int child = hole * 2;
	/*保存当前数据操作*/
	int tmp = 0;

	assert(heap != NULL && heap->parray != NULL);

	tmp = heap->parray[hole];
	/*hold * 2 <= heap->currentSize 判断了当前结点是否为最低层*/
	for (; hole * 2 <= heap->currentSize; hole = child) {
		child = hole * 2;

		/*这句代码解决是否存在右结点,并确定了那个子结点提升的问题*/
		if ((child != heap->currentSize) &&
				(heap->parray[child + 1] < heap->parray[child]))
			child ++;

		if (heap->parray[child] < tmp)
			/*将子结点提升为父结点*/
			heap->parray[hole] = heap->parray[child];
	}
	/*到达了最低的层,也就是树叶*/
	heap->parray[hole] = tmp;
}

/*
 * 实现数据的下虑法实现数据的删除操作
 * */
int deleteMin(BinaryHeap_t *heap)
{
	int ret = 0;
	int index = 0;

	assert(!is_empty(heap));
	/*需要返回的值*/
	ret = heap->parray[1];

	/*将最后需要释放内存空间的值保存到第一个内存空间中*/
	heap->parray[1] = heap->parray[heap->currentSize --];
	/*从表头开始将新的二叉树进行重新排序*/
	presort_BinaryHeap(heap, 1);

	return ret;
}


void print_binaryheap(BinaryHeap_t *heap)
{
	int i = 0;

	assert(heap != NULL && heap->parray != NULL);

	for (i = 1; i <= heap->currentSize; ++ i)
		if (i %6)
			printf("%d\t", heap->parray[i]);
		else
			printf("\n%d\t", heap->parray[i]);
	printf("\n");
}

int main(void)
{
	int i = 0;
	int value = 0;
	BinaryHeap_t *pheap = NULL;

	srand((int)time(0));
	printf("********Test Binaryheap**************\n");

	printf("init and alloc test:\n");
	if (alloc_BinaryHeap(&pheap, 15))
		printf("alloc_BInaryHeap() successed!\n");

	printf("***insert test*****\n");
	for (i = 0; i < 15; ++ i)
		if(!insert(pheap, i*8-rand()%20))
			printf("i = %d:insert failed!!\n",i);
	print_binaryheap(pheap);

	printf("****deleteMin test****\n");
	for (i = 0; i < 5; ++ i) {
		value = deleteMin(pheap);
		printf("pheap deleted:%d\n", value);
	}
	print_binaryheap(pheap);

	printf("deleteMin test successed\n");

	printf("****delete and free test:*******\n");
	delete_BinaryHeap(pheap);

	printf("Is the bheap empty ? %s\n",
			is_empty(pheap)?"Yes":"No");

	free_BinaryHeap(&pheap);

	printf("*********Test successed!***********\n");
	pheap = NULL;
	return 0;
}
