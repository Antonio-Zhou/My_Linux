/*
 *	Created on:	Mar 11 2014
 *	Author:		Eric Zhou
 *	Email:		zhoubb.eric@gmail.com
 *
 * Description
 *	计算阶乘
 */
#include <stdio.h>

long fact(int n)
{
	if (n <= 1)
		return 1;
	else
		return (n * fact(n-1));
}
int main(void)
{
	int i;
	printf("Please input the number:");
	scanf("%d", &i);
	while (i <= 0) {
		printf("Sorry,please input again:");
		scanf("%d", &i);
	}
	printf("The Factorial result of %d is %ld\n", i, fact(i));
	return 0;
}
