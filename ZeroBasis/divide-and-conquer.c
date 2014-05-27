/*
 *	Created on:	Mar 13 2014
 *	Author:		Eric Zhou
 *	Email:		zhoubb.eric@gmail.com
 *
 * 一:思想
 *	有时候我们处理一个复杂的问题，可能此问题求解步骤非常杂，也可能是数据非常多，导致我们当时很难求出或者无法求出，古语有云：
 *	步步为营，各个击破，这个思想在算法中称为分治思想，就是我们可以将该问题分解成若干个子问题，然后我们逐一解决子问题，最后将子问题
 *	的答案组合成整个问题的答案。
 * 二: 条件
 *	①  求解问题确实能够分解成若干个规模较小的子问题，并且这些子问题最后能够实现接近或者是O(1)时间求解。
 *	②  各个子问题之间不能有依赖关系，并且这些子问题确实能够通过组合得到整个问题的解。
 * 三：步骤
 *	①  分解： 将问题分解成若干了小问题。
 *	②  求解： O(1)的时间解决该子问题。
 *	③  合并： 子问题逐一合并构成整个问题的解。
 * Description
 *	乒乓球比赛的安排(分治思想)
 *	初赛阶段是循环赛，有n个选手在n-1天进行比赛。每位选手与其他的每一位选手都要进行一次比赛，最后按照排名进入决赛。每位选手每天都会有比赛。
 *	按此要求进行安排。
 */

#include <stdio.h>

#define MAXN 64
int a[MAXN+1][MAXN+1] = {0};

/*
 * int k-->需要安排选手的起始序号
 * int n-->需要安排的选手数量
 */
void gamecal(int k, int n)
{
	int i, j;
	if (n == 2) {
		a[k][1] = k;	/*参赛选手编号*/
		a[k][2] = k+1;	/*对阵选手编号*/
		a[k+1][1] = k+1;
		a[k+1][2] = k;
	} else {
		gamecal(k, n/2);
		gamecal(k+n/2, n/2);
		/*填充右上角*/
		for (i=k; i<k+n/2; i++)
			for(j=n/2+1; j<=n; j++)
				a[i][j] = a[i+n/2][j-n/2];
		/*填充左下角*/
		for (i=k+n/2; i<k+n; i++)
			for(j=n/2+1; j<=n; j++)
				a[i][j] = a[i-n/2][j-n/2];
	}
}

int main(void)
{
	int num, i, j;
	
	printf("Please input the num of players:");
	scanf("%d", &num);
	j = 2;
	for (i=2; i<8; i++) {
		j = j * 2;
		if (j == num)
			break;
	}
	printf("i=%d\n", i);
	if (i >= 8) {
		printf("NO\n");
		return 0;
	}
	printf("\nnumber");
	for (i=2; i<num+1; i++)
		printf("%2d day", i-1);
	printf("\n");
	for (i=1; i<num+1; i++) {
		for (j=1; j<num+1; j++)
			printf("%4d  ", a[i][j]);
		printf("\n");
	}
	return 0;
}
