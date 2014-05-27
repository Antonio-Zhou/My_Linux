/*
 * 	LCS-2.c	
 *
 *	Created on:	2013-05-07
 *	    Author:	Eric Zhou
 */

#include <stdio.h>
#include <malloc.h>
#include <string.h>

#define M 100

char* LCS(char left[],char right[]);

int  main(void)
{    
	char str1[M],str2[M];

	printf("请输入字符串1:");
	gets(str1);
	printf("请输入字符串2:");
	gets(str2);
	printf("最长子串为:");
	printf("%s\n",LCS(str1,str2));
}

char* LCS(char left[],char right[])
{    
	/*LCS问题就是求两个字符串最长公共子串的问题*/
	int lenLeft = strlen(left),lenRight=strlen(right),k;
	/*
	 * 获取左子串的长度,获取右子串的长度
	 * 注意这里要写成char型,而不是int型,否则输入整型数据时会产生错误.
	 * 矩阵c纪录两串的匹配情况
	 * */
	char*c = malloc(lenRight),*p;
	int start,end,len,i,j;/*start表明最长公共子串的起始点，end表明最长公共子串的终止点*/
	end = len = 0;/*len表示最长公共子串的长度*/
	/*串1从前向后比较*/
	for (i=0; i<lenLeft; i++) {
		/*
		 * 串2从后向前比较,为什么要从后向前呢?是把一维数组c[ ]当二维数组来用,
		 * 如果要从前向后,可以将c申明为一个二维数组c[M][M].但程序要做相应调整.
		 * */
		for (j=lenRight-1; j>=0; j--) {
			/*元素相等时*/
			if (left[i] == right[j]) {

				if (i==0 || j==0)
					c[j]=1;/*c[i][j]=1;*/
				else
					c[j]=c[j-1]+1;/*c[i][j]=c[i-1][j-1]+1;*/

			} else
				c[j] = 0;/*c[i][j]=0;*/
			if (c[j] > len) {/*if (c[i][j]>len)*/
				len = c[j];/*len=c[i][j];*/
				end = j;
	    		}
		}
	}

	start = end-len+1;
	/*数组p纪录最长公共子串*/
	p = (char*)malloc(len+1);
	for (i=start; i<=end; i++)
		p[i-start] = right[i];
	p[len] = '\0';

	return p;
}
