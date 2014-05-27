/*
 *	Created on:	Mar 11 2014
 *	Author:		Eric Zhou
 *	Email:		zhoubb.eric@gmail.com
 *
 * Description
 *	数制转换--十进制转换为其他进制
 */
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

void other_convto_dec(char *s, )
{
	long t1;
	int i, n, t, t3;
	char a[100];

	printf("请输入一个数:\n");
	gets(a);		//将输入的n进制的数储存在数组a中
	strupr(a);      //将数组中的小写字母转化为大写字母
	t3 = strlen(a); //求出数组的长度
	t1 = 0;         
	printf("请输入转换的数的进制:\n");  //输入要转换的数的进制
	scanf("%d", &n);
	for(i=0; i<t3; i++)
	{
		if(a[i] - '0' >= n && a[i] < 'A' || a[i] - 'A' + 10 >n )  //判断输入的数据和进制数是否符合
		{
			printf("data error!!\n"); //错误
			exit (0);
		}
		if(a[i] >='0' && a[i] <= '9')  //判断是否为数字
			t = a[i] - '0';            //求出该数字赋值给t
		else if (n >=11 && (a[i] >= 'A' && a[i] <= 'A' +n -10)) //判断是否为字母
			t = a[i] - 'A' +10; //求出该字母所代表的十进制数
		t1 = t1 *n +t; //求出最终转换成的10进制数

	}
	printf("转化为的10进制数为 %ld\n", t1);
	return 0;
}
void dec_convto_other(char *s, int n, int b)
{
	char bit[]= {"0123456789ABCDEF"};
	int len;
	if (n == 0) {
		strcpy(s, "");
		return;
	}
	convto(s, n/b, b);
	len = strlen(s);
	s[len] = bit[n%b];
	s[len+1] ='\0'; 
}

int main(void)
{
	char s[80];
	int base, old;
	printf("Please input a decimal number:");
	scanf("%d", &old);
	printf("Please input the base:");
	scanf("%d", &base);
	dec_convto_other(s, old, base);
	printf("The result is %s\n", s);
	return 0;
}
