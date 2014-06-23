/*
 *	Author:	Eric Zhou
 *	Date:	2013-05-05
 */

#include<stdio.h>
#include<malloc.h>

#define TRUE	1
#define FALSE	0

typedef int DataType;
int size;		/*数组的大小*/
int INC[4] = {5,3,2,1};	/*采用span=n/2,span=span/2方法获取增量表;该增量表的大小为4*/
DataType arr[10] = {15, 76, 99, 56, 90, 6, 108, 44, 88, 66};

/*初始化数组*/
void InitArr(void)
{
	arr[0] = 15;
	arr[1] = 76;
	arr[2] = 99;
	arr[3] = 56;
	arr[4] = 90;
	arr[5] = 6;
	arr[6] = 108;
	arr[7] = 44;
	arr[8] = 88;
	arr[9] = 66;
}

void swap(DataType *a, DataType *b)
{
	DataType temp;

	temp = *a;
	*a = *b;
	*b = temp;
}

/*
 * 插入排序1:直接插入排序,升序
 * 算法思想:
 * 	将待排序的数据分为有序组(初使大小为1)和无序区,依次将无序中的第一个元素从右至左查找适当的插入位置,直到无序区的大小为1.
 * */
void DirectInsertSort(DataType R[],int n)
{
	int i,j;
	/*用于保存无序组中的第1个元素(不妨称为参考值)*/
	DataType temp;

	/*总共要进行n-1轮处理,每一轮的处理使得有序区的大小加1*/
	for (i=0; i<n-1; i++) {	
		/*将当前无序组中的第一个元素保存,防止其在数据的比较,后移中被占位而丢失*/
		temp = R[i+1];
		j = i;
		/*从参考值的位置从右至左扫描,直至发现比参考值小的数或到达有序组的左端*/
		while (R[j]>temp && j>-1)  
			R[j+1] = R[j--]; /*如果扫描中发现比参考值大的数则将该数后移一位*/
		R[j+1] = temp;		/*将参考值放入空位*/
	}
}

/*
 * 插入排序2:希尔排序，又称缩小增量排序
 * 算法描述:
 * 	设待排序的对象序列有n个对象,
 * 	首先取一个整数span<n作为间隔,将全部对象分为span个子序列,所有距离为span的对象放在同一个序列中,在每一个子序列中分别施行直接插入排序
 * 	再缩小间隔span.如取span=span/2,重复上述的子序列划分和排序工作,直到最后取span为1为止.
 * */
void ShellSort(DataType R[], int n, int INC[], int NofINC)
{
	int i, j, k, m, span; /*span为增量(间隔)*/
	DataType temp;

	for (m=0; m<NofINC; m++) {
		span = INC[m];	/*取本轮的增量*/
		/*将原序列分为span个序列,分别为{R0,R0+span,…};{R1,R1+span,…};…{Rspan-1,Rspan-1+span,…};*/
		for (k=0; k<span; k++)
			/*下面再对每一个子序列进行直接插入排序*/
			for (i=k; i<n-span; i+=span) {
				temp = R[i+span];
				j = i;
				while (j>-1 && R[j]>temp) {
					R[j+span] = R[j];
					j -= span;
				}
				R[j+span] = temp;
			}
	}
}

/*
 * 不带增量表的希尔排序
 * */
void ShellSort2(DataType R[], int n)
{
	int i, j, k, span; /*span为增量(间隔)*/ 
	DataType temp;
	span = n;

	do {
		span = span/2; /*用span=n/2,span=span/2的方法取增量,直至增量为1*/
		/*将原序列分为span个序列，分别为{R0，R0+span,…}{{R1，R1+span,}.};…{Rspan，Rspan-1+span,…};*/
		for (k=0; k<span; k++)
			/*下面再对每一个子序列进行直接插入排序*/
			for (i=k; i<n-span; i+=span) {
				temp = R[i+span];
				j = i;
				while (j>-1 && R[j]>temp) {
					R[j+span] = R[j];
					j -= span;
				}
				R[j+span] = temp;
			}
	}while(span != 1);
}

/*
 * 选择排序1:直接选择排序
 * 算法思想:
 * 	将待排序的数据元素分为有序(初始为空)和无序两组，依次将本轮无序的数据元素中值最小的数据元素与无序组中的第一个元素交换，直到无序组的元素个数为1。
 * */
void DirectSelectSort(DataType R[],int n)
{
	int i,j,min;	/*min用于记录无序组中值最小的数据元素的下标*/
	DataType temp;

	/*总共要进行n-1轮处理,每一轮的处理使得有序区的大小加1*/
	for (i=0; i<n-1; i++) {
		min = i;  /*将无序组中的第一个元素的下标作为min的初值*/
		/*从左至右,查找该无序组中的最小值,并用min保存其下标*/
		for (j=i+1; j<n; j++)
			if(R[j] < R[min]) 
				min = j;
		/*如果无序组中的最小数不是该无序组中的第1个元素,则将两数交换*/
		if (min != i) {
			temp = R[i];
			R[i] = R[min];
			R[min] = temp;
		}
	}
}

/*
 * 冒泡排序,降序
 * */
void BubbleSort(DataType R[],int n)
{
	int i,j;
	int change; /*用于标记本轮排序中是否有数据交换，如果无，说明剩下的数都是有序，无须再排序*/

	/*n-1轮排序*/
	for (i=1,change=1; change && i<n; i++) {
		change = 0; /*交换标记清0*/
		/*每轮排序要进行n-i次比较*/
		for(j=0; j<n-i; j++)
			/*如果相邻的两个值，左边的小，则交换，并置位交换标记*/
			if (R[j]<R[j+1]) {
				swap(&R[j], &R[j+1]);
				change = 1;
			}
	}
}

/*
 * 快速排序
 * */
void QuickSort(DataType R[],int low,int high)
{
	DataType temp;
	int i = low,j = high;
	temp = R[i]; /*基准,经过一轮排序后,该序列左边的数都不大于基准,右边的数都不小于基准*/

	do {
		/*从右向左扫描*/
		while ((R[j]>=temp) && (i<j))
			j--;
		/*右边存在小于基准的数,将它放在左边的空位*/
		if(i < j)	
			R[i++] = R[j];
		/*从左向右扫描*/
		while((R[i]<=temp) && (i<j)) 
			i++;
		/*左边存在大于基准的数,将它放在右边的空位*/
		if(i < j)
			R[j--] = R[i];
	}while(i != j); /*当i==j时,结束一轮的排序*/

	R[i] = temp; /*将基准放在恰当的位置,使该序列满足:左边的数都不大于基准,右边的数都不小于基准*/
	/*对基准左边序列进行递归*/
	if(low < i-1)
		QuickSort(R, low, i-1);
	/*对基准右边序列进行递归*/
	if(high > j+1)
		QuickSort(R, j+1, high);
}

/*
 * 归并排序
 * 合并一个序列中的两个有序的数据段Rs~Rm,Rm+1~Rh
 * */
void Merge(DataType R[],int s,int m,int h)
{
	int i = s,j = m+1,k = 0;
	DataType *p;
	p = (DataType *)malloc((h-s+1) * sizeof(DataType));

	while ((i<=m) && (j<=h)) {
		if (R[i] < R[j])	
			*(p+(k++)) = R[i++];
		else	
			*(p+(k++)) = R[j++];
	}
	while (i <= m)
		*(p+(k++)) = R[i++];
	while (j <= h)
		*(p+(k++)) = R[j++];
	for (k=0; k<h-s+1; k++)
		R[s+k] = *(p+k);

	free(p);
}

void MergeSort(DataType R[],int s,int h)
{
	int m;

	if (s == h) 
		return;
	m = (h+s)/2;
	MergeSort(R, s, m);
	MergeSort(R, m+1, h);
	Merge(R, s, m, h);
}

/*其它排序，未完待续……*/
/*基数排序*/
/*堆排序 */
/*折半插入排序*/
/*二路插入排序*/

/*
 * 动态建立数组
 * */
DataType *CreateArray()
{
	int i;
	DataType *p;

	printf("请输入数组的大小");
	scanf("%d", &size);
	p = (DataType *)malloc(size * sizeof(DataType));
	printf("请输入%d个数:", size);
	for(i=0; i<size; i++)
		scanf("%d", p+i);
	return p;
}

/*
 * 打印所有数据
 * */
void PrintArray(DataType R[],int n)
{
	int i;

	for (i=0; i<n; i++) {
		printf("%d\t", *(R+i));
		if(!(i+1) % 10)
			printf("\n");
	}
}

int main(void)
{
	int run = TRUE;
	int fun;
	int *array, *copy;

	size = 10;

	printf("*********使用说明，2-9的操作必须基于数组的建立（操作1）***********\r\n");
	printf("************************排序算法测试程序**************************\r\n");
	printf("************************0       退出程序 *************************\r\n");
	printf("************************1     初始化数组**************************\r\n");
	printf("************************2   直接插入排序**************************\r\n");	
	printf("************************3       希尔排序**************************\r\n");	//?
	printf("************************4      希尔排序2**************************\r\n");	//?
	printf("************************5   直接选择排序**************************\r\n");
	printf("************************6       冒泡排序**************************\r\n");
	printf("************************7       快速排序**************************\r\n");
	printf("************************8      (?)堆排序**************************\r\n");
	printf("************************9   打印所有数据**************************\r\n");
	printf("************************10      归并排序**************************\r\n");

	while (run) {
		printf("请输入操作代码:");
		scanf("%d", &fun);
		switch (fun) {
		case 0:
			run=FALSE;
			break;
		case 1:
			InitArr();
			break;
		case 2:
			DirectInsertSort(arr, size);
			break;
		case 3:
			ShellSort(arr, size, INC, 4);
			break;
		case 4:
			ShellSort2(arr, size);
			break;
		case 5:
			DirectSelectSort(arr, size);
			break;
		case 6:
			BubbleSort(arr, size);
			break;
		case 7:
			QuickSort(arr, 0, size-1);
			break;
		case 8:
			break;
		case 9:
			PrintArray(arr, size);
			break;
		case 10:
			MergeSort(arr, 0, 9);
			break;
		default:
			break;
		}
	}
	return 0;
}
