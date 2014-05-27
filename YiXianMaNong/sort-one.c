/*
 *	Author:	Eric Zhou
 *	Date:	2013-04-25
 */

/*
 * 排序算法比较
 * 1.快速排序法(Quick Sort)：
 * 	通过一趟扫描将要排序的数据分割成独立的两部分，其中一部分的所有数据都比另外一部分的所有数据都要小，然后再按此方法对这两部分数据分别进行快速排序。整个排序过程可以递归进行，以此达到整个数据变成有序序列的目的。
 *
 * 2.冒泡排序法(Bubble Sort):
 * 原理是临近的数字两两进行比较,按照从小到大或者从大到小的顺序进行交换，这样一趟过去后,最大或最小的数字被交换到了最后一位，然后再从头开始进行两两比较交换，直到倒数第二位时结束。
 * */

#include <stdio.h>

void bubble_sort(int a[],int n);
void quick_sort(int a[],int i,int j);

int main(void)
{
	int a[5] = {5, 4, 3, 2, 1};
	int i = 0;

	printf("The origin number of a is :");
	for (i=0; i<5; i++)
		printf("%d ",a[i]);
	printf("\r\n");

	quick_sort(a, 0, 4);
	printf("After quick sort the number of a is :");
	for (i=0; i<5; i++)
		printf("%d ",a[i]);
	printf("\r\n");

	int b[5] = {5, 4, 3, 2, 1};
	printf("The origin number of b is :");
	for (i=0; i<5; i++)
		printf("%d ",b[i]);
	printf("\r\n");

 	bubble_sort(b, 5);
	printf("After bubble sort the number of b is :");
	for (i=0; i<5; i++)
		printf("%d ",b[i]);
	printf("\r\n");

	return 0;
}

/*快速排序法*/
void quick_sort(int a[],int i,int j)
{
	int m, n, temp;
	int k;

	m = i;/*i:要排数组起始元素的下标*/
	n = j;/*j:要排数组结束元素的下标*/
	k = a[(1+j)/2];/*选取的参照*/
	do {
		while (a[m] < k && m < j)/*从左到右找比k大的元素*/
			m++;
		while (a[n] > k && n > i)/*从右到左找比k小的元素*/
			n--;
		if (m <= n) {/*交换*/
			temp=a[m];
			a[m]=a[n];
			a[n]=temp;
			m++;
			n--;
		}
	} while (m <= n);

	if (m < j)
		quick_sort(a, m, j);/*递归调用*/
	if (n > i)
		quick_sort(a, i, n);/*递归调用*/
}

void bubble_sort(int a[],int n)
{
	int i, j, temp, sign;/*sign=1,未交换.sign=0,交换*/

	for (i=0; i<n-1; i++) {
		for (j=i+1; j<n; j++) {
			sign = 1;
			if (a[i] > a[j]) {
				temp = a[i];
				a[i] = a[j];
				a[j] = temp;
				sign = 0;
			}
			if (sign == 1)
				break;
		}
	}
}
