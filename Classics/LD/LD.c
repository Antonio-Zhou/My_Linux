/*
 * 	LD.c	
 *
 *	Created on:	2013-05-08
 *	    Author:	Eric Zhou
 */

#include <stdio.h>
#include <malloc.h>
#include <string.h>

#define MAX(a,b) ((a)>(b)?(a):(b))

void matrix_print(char *matrix, int row, int col);
char matrix_value(char left, char top, char top_left, int cost);

int main(void)
{
	char *matrix;
	char str1[255], str2[255];
	int  row, col;
	int  i, j;
	int dis;
	double sim;

	printf("please input str1: ");
	scanf("%s", str1);
	printf("please input str2: ");
	scanf("%s", str2);
	/*
	 * 矩阵要比行列各多一来存储矩阵行列序号
	*/
	row = strlen(str1) + 1;
	col = strlen(str2) + 1;
	/*
	 * 申请空间
	*/
	matrix = (char*) malloc(col*row);
	/*
	 * 初始化首行、首列
	*/
	for (i=0; i<row; i++) 
		matrix[i*col] = i;
	for (j=0; j<col; j++) 
		matrix[j] = j;
	/*
	 * 根据算法填充矩阵
	 * 当字符相同时cost=0, 否则cost=1. 也就是代码中的 !(str1[i-1]==str2[j-1])
	 *
	 * 取待填值(val)位置的3个邻居left,top,top-left的值来进行比较，取最小值(min)。
	 * 当 left或top 为最小值时, val=min+1.
	 * 当 top-left 为最小值时, val=min+cost.
	*/
	for (i=1; i<row; i++)
		for (j=1; j<col; j++) 
			matrix[i*col+j] = matrix_value(matrix[i*col+j-1], matrix[(i-1)*col+j],
							matrix[(i-1)*col+j-1], !(str1[i-1]==str2[j-1]));
	/*
	 * 打印矩阵
	*/
	matrix_print(matrix, row, col);
	/*
	 * 矩阵最后的格子就是“编辑距离”
	*/
	dis = matrix[(row*col) - 1];
	printf("distance = %d\n", dis);
	/*
	 * 小应用。使用“编辑距离” 来算2个串的相似度。
	 * 可以根据自己需要设计不同的算法，我就是简单的除以了下长串。
	*/
	sim = 1 - ((double)dis/(double)MAX(row-1, col-1));
	printf("similar = %f\n", sim);
	free(matrix);

	return 0;
}

void matrix_print(char *matrix, int row, int col)
{
	int  i, j;

	for (i=0; i<row; i++)
		for (j=0; j<col; j++)
			printf("%4.2d", matrix[i*col+j]);
	printf("\n");
}

char matrix_value(char left, char top, char top_left, int cost)
{
	char ret;

	ret = left<top ? left : top;

	return (ret < top_left) ? (ret + 1) : (top_left + (char)cost);
}
