/*
 *	Created on:	2014-07-10
 *	    Author:	Eric Chou
 *
 * http://www.cnblogs.com/gaochundong/p/comparison_sorting_algorithms.html#odd_even_sort
 *
 * +--------------------------------------------------------------------------------------------------------------------+
 * |						比较排序算法(Comparison Sorts)						|
 * +--------------------+-----------------------+-------+---------------+---------------+---------------+---------------+
 * | Category		| Name			| Best	| Average	| Worst 	| Memory 	| Stability	|
 * +--------------------+-----------------------+-------+---------------+---------------+---------------+---------------+
 * | 插入排序		|插入排序		| n	| n^2		| n^2		| 1		| Stable	|
 * | (Insertion Sorts)	|(Insertion Sort)	|	|		|		|               |               |
 * |			+-----------------------+-------+---------------+---------------+---------------+---------------+
 * |			|希尔排序		| n	| nlog2n	| nlog2n	| 1		| Not Stable	|
 * |			|(Shell Sort)		|	|		|		|               |               |
 * +--------------------+-----------------------+-------+---------------+---------------+---------------+---------------+
 * |	交换排序	|快速排序		| nlogn	| nlogn		| n^2		| log n		| Not Stable    |
 * | (Exchange Sorts )	|(Quick Sort)		|	|		|		|               |               |
 * |			+-----------------------+-------+---------------+---------------+---------------+---------------+
 * |			|冒泡排序		| n	| n^2		| n^2		| 1		| Stable        |
 * |			|(Bubble Sort)		|	|		|		|               |               |
 * |			+-----------------------+-------+---------------+---------------+---------------+---------------+
 * |			|鸡尾酒排序		| n	| n^2		| n^2		| 1		| Stable        |
 * |			|(Cocktail Sort)	|	|		|		|               |               |
 * |			+-----------------------+-------+---------------+---------------+---------------+---------------+
 * |			|奇偶排序		| n	| n^2		| n^2		| 1		| Stable        |
 * |			|(Odd-Even Sort)	|	|		|		|               |               |
 * +--------------------+-----------------------+-------+---------------+---------------+---------------+---------------+
 * |选择排序		|选择排序		| n^2	| n^2		| n^2		| 1		| Not Stable    |
 * |(Selection Sorts)	|(Selection Sort)	|	|		|		|               |               |
 * |			+-----------------------+-------+---------------+---------------+---------------+---------------+
 * |			|堆排序			| nlogn	| nlogn		| nlogn		| 1		| Not Stable    |
 * |			|(Heap Sort)		|	|		|               |               |               |
 * +--------------------+-----------------------+-------+---------------+---------------+---------------+---------------+
 * |合并排序		|合并排序		| n	| nlogn		| nlogn		| n		| Stable        |
 * |(Merge Sorts)	|(Merge Sort)		|	|		|               |               |               |
 * +--------------------+-----------------------+-------+---------------+---------------+---------------+---------------+
 * |混合排序		|内省排序		| nlogn	| nlogn		| nlogn		| logn		| Not Stable    |
 * |(Hybrid Sorts)	|(Introspective Sort)	|	|		|               |               |               |
 * +--------------------------------------------------------------------------------------------------------------------+
 *
 * Stable 与 Not Stable 的比较
 * 	稳定排序算法会将相等的元素值维持其相对次序.
 * 	如果一个排序算法是稳定的,当有两个有相等的元素值R和S,且在原本的列表中R出现在S之前,那么在排序过的列表中R也将会是在S之前.
 * */

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <assert.h>
#include <string.h>
#include <math.h>
#include <time.h>
#include <sys/time.h>

#define ARRAY_LEN 100
#define BUCKET_SIZE 10

typedef void (*type_sort)(int *, int);

struct bucket_node {
	int key;
	struct bucket_node *next;
};

/*函数声明*/
/***************交换排序****************************/
void bubble_sort(int *pdata, int count);
void bubble_sort_flag(int *pdata, int count);
void quick_sort(int *pdata, int count);
void cocktail_sort(int *pdata, int count);
void odd_even_sort(int *pdata, int count);
/***************插入排序****************************/
void insert_sort(int *pdata, int count);
void shell_sort(int *pdata, int count);
/***************选择排序****************************/
void select_sort(int *pdata, int count);
void heap_sort(int *pdata, int count);
/***************合并排序****************************/
void merge_sort(int *pdata, int count);
/***************分配式排序****************************/
void radix_sort(int *pdata, int count);
void bucket_sort(int *pdata, int count);

void swap_elem(int *a, int *b);

void selection_menu(void);
void select_function(int choice, int *flag);
void creat_randnum(int *pdata, int count);
void printf_array(int *pdata, int count);
void printf_bucket(struct bucket_node **bucket_table);

int count_time(type_sort fp, int *pdata, int count);
void call_sort_func(type_sort fp, int *pdata, int count);

int sort_data[ARRAY_LEN], mem_data[ARRAY_LEN];
struct bucket_node **bucket_table;

int main(void)
{
	int flag = 1, choice;

	selection_menu();

	while (flag) {
		printf("please input you selction:");
		scanf("%d", &choice);
		select_function(choice, &flag);
	}

	return 0;
}

/*
 * 冒泡排序
 * 设数组长度为N(从小到大排序举例)
 * 1.比较相邻的前后二个数据,如果前面数据大于后面的数据,就将二个数据交换。
 * 2.这样对数组的第0个数据到N-1个数据进行一次遍历后,最大的一个数据就“沉”到数组第N-1个位置.
 * 3.N=N-1,如果N不为0就重复前面二步,否则排序完成
 * */
void bubble_sort(int *pdata, int count)
{
	int i, j;

	/*气泡上升顶点*/
	for (i = 1; i < count; i++) {
		/*气泡上升过程*/
		for (j = count - 1; j >= i; j--) {
			if (*(pdata + j) < *(pdata + j - 1))
				swap_elem((pdata + j), (pdata + j - 1));
		}
	}
}

/*设置标志,减少次数*/
void bubble_sort_flag(int *pdata, int count)
{
	int j, k = count;
	bool flag = true;

	while (flag) {
		flag = false;
		for (j = 1; j < k; j++)
			if (*(pdata + j) < *(pdata + j - 1)) {
				swap_elem((pdata + j), (pdata + j - 1));
				flag = true;
			}
		k--;
	}
}

/*
 * 鸡尾酒排序
 * 	冒泡排序的一种变形.是先向一个方向,然后向另一个方向,来回冒泡,称为双向冒泡排序(Bidirectional Bubble Sort),是冒泡排序的一种变形.
 * 	此算法与冒泡排序的不同处在于排序时是以双向在序列中进行排序.如果序列中的大部分元素已经排序好时.可以得到比冒泡排序更好的性能.
 * */
void cocktail_sort(int *pdata, int count)
{
	bool swapped = false;
	int i, start = 0, end = count - 1;

	do {
		swapped = false;

		/*move the larger to right side*/
		for (i = start; i < end; i++)
			if (*(pdata + i) > *(pdata + i + 1)) {
				swap_elem((pdata + i), (pdata + i + 1));
				swapped = true;
			}

		/*we can exit the outer loop here if no swaps occurred.*/
		if (!swapped)
			break;
		swapped = false;
		end --;

		/*move the smaller to left side*/
		for (int j = end; j > start; j--) {
			if (*(pdata + j) < *(pdata + j - 1)) {
				swap_elem((pdata + j), (pdata + j - 1));
				swapped = true;
			}
		}

		start = start + 1;
	} while (swapped);
}

/*
 * 奇偶排序
 * 	通过比较数组中相邻的(奇-偶)位置元素.
 * 	如果该奇偶元素对是错误的顺序(前者大于后者),则交换元素.然后再针对所有的(偶-奇)位置元素进行比较.如此交替进行下去.
 *
 * */
void odd_even_sort(int *pdata, int count)
{
	bool swapped = true;
	int i, start = 0;

	while (swapped || start == 1) {
		swapped = false;

		for (i = start; i < count - 1; i += 2) {
			if (*(pdata + i) > *(pdata + i + 1)) {
				swap_elem((pdata + i), (pdata + i + 1));
				swapped = true;
			}
		}

		if (start == 0)
			start = 1;
		else
			start = 0;
	}
}

/*直接插入排序算法*/
void insert_sort(int *pdata, int count)
{
	int i, j;
	int tower;

	tower = *pdata;
	for (i = 1; i < count; i++) {
		tower = *(pdata + i);		/*pdata[0]为监视哨*/
		j = i - 1;			/*依次插入记录pdata[1],……，pdata[count]*/
		/*查找pdata[i]合适的位置*/
		while (tower < *(pdata + j)) {
			*(pdata + j + 1) = *(pdata + j);
			j--;
		}

		*(pdata + j + 1)= tower;
	}
}

/*
 * 希尔排序算法
 * 增量为d(i+1)=[d(i)/2]
 * */
void shell_sort(int *pdata, int count)
{
	/*change=1表示有交换,jump为增量步长*/
	int i, jump, change, temp, m;

	jump = count;
	while (jump > 0) {
		jump = jump / 2; /*取步长d(i+1)=[d(i)/2]*/
		do {
			change = 0;
			/*一趟排序完成*/
			for (i = 1;i < (count - jump); i++) {
				m = i + jump;				/*取本趟的增量*/
				if (*(pdata + i) > *(pdata + m)) {
					swap_elem((pdata + m), (pdata + i));
					change = 1;
				}
			}
		} while(change == 1);
	}			/*当增量jump=1且change=0时终止算法*/
}

/*快速排序算法中的一趟划分函数*/
int partition(int *pdata, int begin, int end)
{
	int i, j, temp;

	i = begin;
	j = end;
	temp = *(pdata + i);	/*temp为基准记录*/

	do {
		/*从右往左扫描,查找第一个小于temp的项*/
		while ((*(pdata + j) >= temp) && (i < j))
			j--;
		if (i < j) {
			*(pdata + i) = *(pdata + j);	/*交换pdata[i]和pdata[j]*/
			i++;
		}
		/*从左往右扫描,查找第一个大于temp的项*/
		while ((*(pdata + i) <= temp) && (i < j))
			i++;
		if (i < j) {
			*(pdata + j) = *(pdata + i);	/*交换pdata[i]和pdata[j]*/
			j--;
		}
	} while (i != j);	/*一次划分结束,基准记录达到其最终位置*/

	*(pdata + i) = temp;	/*最后将基准记录temp定位*/

	return i;
}

/*对pdata[begin]到pdata[end]进行快速排序*/
void _quick_sort(int *pdata, int begin, int end)
{
	int pos;

	/*只有一个记录或无记录时无需排序*/
	if (begin < end) {
		pos = partition(pdata, begin, end);	/*对pdata[begin]到pdata[end]进行一次划分*/
		_quick_sort(pdata, begin, pos - 1);	/*递归处理左区间*/
		_quick_sort(pdata, pos + 1, end);	/*递归处理右区间*/
	}
}

/*
 * 快速排序算法
 * */
void quick_sort(int *pdata, int count)
{
	int begin, end;

	printf("please input the begin pos and the end pos which you want to sort:\n");
	scanf("%d %d", &begin, &end);

	while (begin > count - 1 || end > count - 1 || begin > end || begin < 0 || end < 0) {
		printf("input error, please input again:\n");
		scanf("%d %d", &begin, &end);
	}

	_quick_sort(pdata, begin, end);
}

/*直接选择排序算法如下*/
void select_sort(int *pdata, int count)
{
	int i, j, k;

	/*做count-1趟选择排序*/
	for (i = 1;i <= count; i++) {
		k = i;
		/*在当前无序区中选择最小的项pdata[k]*/
		for (j = i + 1; j <= count; j++)
			if (*(pdata + j) < *(pdata + k))
				k = j;
		if (k != i) {
			swap_elem((pdata + k), (pdata + i));
		}
	}
}

/*
 * 堆排序算法
 * 主要分为两步:
 * 	1.堆的筛选,在数组中pdata[top]到pdata[bottom]中,调整堆pdata[top]
 * 	2.对数组pdata[1]到pdata[count]进行堆排序
 * */
void shift_heap(int *pdata, int top, int bottom)
{
	int j, temp;

	temp = *(pdata + top);
	j = 2 * bottom;
	/*j<=bottom, pdata[2*top]是pdata[top]的左孩子*/
	while (j <= bottom) {
		if ((j < bottom) && (*(pdata + j) < *(pdata + j + 1)))
			j++;				/*j指向pdata[top]的左右孩子中关键词较大者*/
		/*若孩子结点的关键词大于父结点*/
		if (temp < *(pdata + j)) {
			*(pdata + top) = *(pdata + j);	/*将pdata[j]调到父亲结点的位置上*/
			top = j;			/*调整top和j的值,以便继续“筛”结点*/
			j = 2 * top;
		} else
			j = bottom + 2;			/*调整完毕,退出循环*/
	}

	*(pdata + top) = temp;				/*将被筛选的结点放入正确的位置*/
}

void heap_sort(int *pdata, int count)
{
	int i;

	/*建立初始堆*/
	for (i = count / 2; i >= 0; i--)
		shift_heap(pdata, i, count);
	/*进行count-1趟筛选,交换,调整,完成堆排序*/
	for (i = count; i > 1; i--) {
		swap_elem((pdata + 1), (pdata + i));
		/*将数组元素pdata[1]到pdata[i-1]重新调整成为一个新堆*/
		shift_heap(pdata, 1, i - 1);
	}
}

/*
 * 二路归并排序算法
 * 将有序的pdata[s..u]和pdata[u+1..v]归并为有序的temp[s..v]
 * */
void merge(int *pdata, int *temp, int pos_first, int pos_second, int pos_third)
{
	int i, j, q;

	i = q = pos_first;
	j = pos_second + 1;

	while (i <= pos_second && j <= pos_third) {
		if (*(pdata + i) <= *(temp + j)) {
			*(temp + q) = *(pdata + i);
			q++;
			i++;
		} else {
			*(temp + q) = *(pdata + j);
			q++;
			j++;
		}
	}
	/*将X中剩余元素pdata[i..u]复制到temp*/
	while (i <= pos_second) {
		*(temp + q) = *(pdata + i);
		q++;
		i++;
	}
	/*将X中剩余元素pdata[j..v]复制到temp*/
	while(j <= pos_third) {
		*(temp + q) = *(pdata + j);
		q++;
		j++;
	}
}

/*
 * pdata[0..n-1]表示参加排序的初始序列
 * 整型变量i指出当前归并的两个子序列中第1个子序列的第1个元素的位置
 * temp[0..n-1]表示这一趟归并后的结果
 * */
void merge_pass(int *pdata, int *temp, int count, int len)
{
	int j, first = 0;

	/*将相邻的两个长度为t的各自有序的子序列合并成一个长度为2t的子序列*/
	while (count - first >= 2 * len) {
		merge(pdata, temp, first, first + len - 1, first + 2 * len - 1);
		first = first + 2 * len;
	}

	/*若最后剩下的元素个数大于一个子序列的长度t时*/
	if (count - first > len)
		merge(pdata, temp, first, first + len - 1, count - 1);
	/*相当于只是把pdata[first..count-1]序列中的数据赋值给temp[fitst..count-1]*/
	else
		for(j = first; j < count; ++j)
			*(temp + j) = *(pdata + j);
}

void merge_sort(int *pdata, int count)
{
	int len = 1;	/*len为某一趟归并时子序列的长度*/
	int *temp = NULL;

	temp = (int *)malloc(sizeof(int) * count);
	while (len < count) {
		merge_pass(pdata, temp, count, len);
		len *= 2;
		merge_pass(temp, pdata, count, len);
		len *= 2;
	}

	free(temp);
}

/*
 * 基数排序
 * 主要分为四部分:
 * 	a.计算在某一分位上的数据
 * 	b.对某一分位上的数据按照0~10排序
 * 	c.根据b中的排序结果,对实际的数据进行排序
 * 	d.把a,b,c组合起来构成基数排序,直到某一分位上的数据为0
 * */
int pre_process_data(int *pdata, int count, int weight)
{
	int index, value = 1;

	for (index = 0; index < weight; index++)
		value *= 10;

	for (index = 0; index < count; index ++)
		*(pdata + index) = *(pdata + index) % value / (value / 10);

	for (index = 0; index < count; index ++)
		if (*(pdata + index) != 0)
			return 1;

	return 0;
}

void sort_for_basic_number(int *pdata, int count, int *swap)
{
	int index, basic, total = 0;

	for (basic = -9; basic < 10; basic++) {
		for (index = 0; index < count; index++) {
			if ((*(pdata + index) != -10) && (basic == *(pdata + index))) {
				*(swap + total) = *(pdata + index);
				total++;
				*(pdata + index) = -10;
			}
		}
	}

	memmove(pdata, swap, sizeof(int) * count);
}

void sort_data_by_basic_number(int *pdata, int *temp, int *swap, int count, int weight)
{
	int index, outer, inner, value = 1;

	for (index = 0; index < weight; index++)
		value *= 10;

	for (outer = 0; outer < count; outer++) {
		for (inner = 0; inner < count; inner++) {
			if ((*(pdata + inner) != -10) &&
					*(temp + outer) == (*(pdata + inner) % value / (value / 10))) {
				*(swap + outer) = *(pdata + inner);
				*(pdata + inner) = -10;
				break;
			}
		}
	}
	memmove(pdata, swap, sizeof(int) * count);
}

void radix_sort(int *pdata, int count)
{
	int *temp, *swap;
	int ret, weight = 1;

	if (pdata == NULL || count == 0)
		return;

	temp = (int *)malloc(sizeof(int) * count);
	assert(temp != NULL);
	memmove(temp, pdata, count * sizeof(int));

	swap = (int *)malloc(sizeof(int) * count);
	assert(swap != NULL);

	while (1) {
		ret = pre_process_data(temp, count, weight);
		if(!ret)
			break;

		sort_for_basic_number(temp, count, swap);
		sort_data_by_basic_number(pdata, temp, swap, count, weight);
		memmove(temp, pdata, count * sizeof(int));
		weight++;
	}

	free(temp);
	free(swap);
}

/*桶排序算法*/
int num_of_digits(int *pdata, int count)
{
	int i;
	int largest = 0, digits = 0;	/*digits为最大值的位数*/

	/*获取最大值*/
	for (i = 0; i < count; i++)
		if (*(pdata + i) > largest)
			largest = *(pdata + i);
	while (largest) {
		digits++;
		largest /= 10;
	}

	return digits;
}

void bucket_sort(int *pdata, int count)
{
	int i, j;
	int divisor = 10; /*除数*/
	int digits, index;
	struct bucket_node *k, *p, *node;

	bucket_table = (struct bucket_node **)malloc(BUCKET_SIZE * sizeof(struct bucket_node *));
	for (i = 0; i < BUCKET_SIZE; i++) {
		bucket_table[i] = (struct bucket_node *)malloc(sizeof(struct bucket_node));
		bucket_table[i]->key = 0; /*记录当前桶中的数据量*/
		bucket_table[i]->next = NULL;
	}

	digits = num_of_digits(pdata, count);
	for (i = 1; i < digits; i++)
		divisor *= 10;

	for (j = 0; j < count; j++) {
		node = (struct bucket_node *)malloc(sizeof(struct bucket_node));
		node->key = *(pdata + j);
		node->next = NULL;
		/*映射函数计算桶号,index为相应的(divisor/10)位的值,如当divisor=10时,求的是个位数*/
		index = (*(pdata + j) % divisor -  *(pdata + j) % (divisor / 10)) / (divisor / 10);
		/*初始化P成为桶中数据链表的头指针*/
		p = bucket_table[index];
		/*该桶中还没有数据*/
		if (p->key == 0) {
			bucket_table[index]->next = node;
			(bucket_table[index]->key)++;
		} else {
			/*链表结构的插入排序*/
			while (p->next != NULL && p->next->key <= node->key)
				p = p->next;
			node->next = p->next;
			p->next = node;
			(bucket_table[index]->key)++;
		}
	}
}

void call_sort_func(type_sort fp, int *pdata, int count)
{
	fp(pdata, count);
}

void select_function(int choice, int *flag)
{
	int timeuse;

	switch (choice) {
	case 0:
		creat_randnum(sort_data, ARRAY_LEN);
		printf("the origin number is\n");
		printf_array(sort_data, ARRAY_LEN);
		break;
	case 1:
		/*冒泡排序(两种)*/
		timeuse = count_time(bubble_sort, sort_data, ARRAY_LEN);
		printf("bubble sort use time: %d ms\n", timeuse);
		printf_array(sort_data, ARRAY_LEN);
		break;
	case 2:
		/*直接插入排序*/
		timeuse = count_time(insert_sort, sort_data, ARRAY_LEN);
		printf("insert sort use time: %d ms\n", timeuse);
		printf_array(sort_data, ARRAY_LEN);
		break;
	case 3:
		/*快速排序*/
		timeuse = count_time(quick_sort, sort_data, ARRAY_LEN);
		printf("select sort use time: %d ms\n", timeuse);
		printf_array(sort_data, ARRAY_LEN);
		break;
	case 4:
		/*选择排序*/
		timeuse = count_time(select_sort, sort_data, ARRAY_LEN);
		printf("select sort use time: %d ms\n", timeuse);
		printf_array(sort_data, ARRAY_LEN);
		break;
	case 5:
		/*希尔排序*/
		timeuse = count_time(shell_sort, sort_data, ARRAY_LEN);
		printf("shell sort use time: %d ms\n", timeuse);
		printf_array(sort_data, ARRAY_LEN);
		break;
	case 6:
		/*堆排序*/
		timeuse = count_time(heap_sort, sort_data, ARRAY_LEN);
		printf("heap sort use time: %d ms\n", timeuse);
		printf_array(sort_data, ARRAY_LEN);
		break;
	case 7:
		/*二路归并排序*/
		timeuse = count_time(merge_sort, sort_data, ARRAY_LEN);
		printf("merge sort use time: %d ms\n", timeuse);
		printf_array(sort_data, ARRAY_LEN);
		break;
	case 8:
		/*基数排序*/
		timeuse = count_time(radix_sort, sort_data, ARRAY_LEN);
		printf("radix sort use time: %d ms\n", timeuse);
		printf_array(sort_data, ARRAY_LEN);
		break;
	case 9:
		/*桶排序*/
		timeuse = count_time(bucket_sort, sort_data, ARRAY_LEN);
		printf("bucket sort use time: %d ms\n", timeuse);
		printf_bucket(bucket_table);
		break;
	case 10:
		/*鸡尾酒排序*/
		timeuse = count_time(cocktail_sort, sort_data, ARRAY_LEN);
		printf("bubble sort use time: %d ms\n", timeuse);
		printf_array(sort_data, ARRAY_LEN);
		break;
	case 11:
		/*奇偶排序*/
		timeuse = count_time(odd_even_sort, sort_data, ARRAY_LEN);
		printf("bubble sort use time: %d ms\n", timeuse);
		printf_array(sort_data, ARRAY_LEN);
		break;
	default:
		*flag = 0;
		printf("the program is over ,enter any key to exit\n");
	}
}

int count_time(type_sort fp, int *pdata, int count)
{
	int timeuse;
	struct timeval start, end;

	gettimeofday(&start, NULL);
	call_sort_func(fp, pdata, count);
	gettimeofday(&end, NULL);
	timeuse = 1000000 * (end.tv_sec - start.tv_sec) + end.tv_usec - start.tv_usec;

	return timeuse;
}

/*产生给定个数和范围的随机整数函数*/
void creat_randnum(int *pdata, int count)
{
	int i = 0;
	int num;
	int range = 30000;

	memset(pdata, 0, count);

	srand(time(NULL));

	while (i < count) {
		num = rand();
		if ((num > 10000) && (num < 100000)) {
			*(pdata + i) = num;
			i++;
		}
	}
}

void selection_menu(void)
{
	printf("|---------------------------------------------------------------|\n");
	printf("|		排序算法的比较----主控模块:			|\n");
	printf("|			0. 创建排序数组				|\n");
	printf("|			1. 冒泡排序				|\n");
	printf("|			2. 直接插入排序				|\n");
	printf("|			3. 快速排序				|\n");
	printf("|			4. 选择排序				|\n");
	printf("|			5. 希尔排序				|\n");
	printf("|			6. 堆排序				|\n");
	printf("|			7. 二路归并排序				|\n");
	printf("|			8. 基数排序				|\n");
	printf("|			9. 桶排序				|\n");
	printf("|			10. 鸡尾酒排序				|\n");
	printf("|			11. 奇偶排序				|\n");
	printf("|			12. 退出				|\n");
	printf("|---------------------------------------------------------------|\n");
}

void swap_elem(int *a, int *b)
{
	/*方法一*/
	int tmp = 0;
	int switch_func = 0;

	if (switch_func == 0) {
		tmp = *b;
		*b = *a;
		*a = tmp;
	} else if (switch_func == 1) {
		*a = *a + *b;
		*b = *a - *b;
		*a = *a - *b;
	} else if (switch_func == 2) {
		*a ^= *b ^= *a ^= *b;
	} else if (switch_func == 3) {
		*a = *a + *b - (*b = *a);
	}
}

void printf_array(int *pdata, int count)
{
	int i;

	for (i = 1; i <= count; i++) {
		printf("%d ", *(pdata + i - 1));
		if (i % 10 == 0)
			printf("\n");
	}
	printf("\n");
}

void printf_bucket(struct bucket_node **bucket_table)
{
	int i, j = 0;
	struct bucket_node *k;

	for (i = 0; i < BUCKET_SIZE; i++)
		for (k = bucket_table[i]->next; k != NULL; k = k->next) {
			printf("%d ", k->key);
			j++;
			if (j % 10 == 0)
				printf("\n");
		}
	printf("\n");
}
