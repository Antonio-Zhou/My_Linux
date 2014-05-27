/*
 *	Created on:	Mar 17 2014
 *	Author:		Eric Zhou
 *	Email:		zhoubb.eric@gmail.com
 * 贪心算法（又称贪婪算法）是指，在对问题求解时，总是做出在当前看来是最好的选择。也就是说，不从整体最优上加以考虑，他所做出的仅是在某种意义上的局部最优解。贪心算  * 法不是对所有问题都能得到整体最优解，但对范围相当广泛的许多问题他能产生整体最优解或者是整体最优解的近似解。
 * 基本思路：
 *	⒈.建立数学模型来描述问题。
 *	⒉.把求解的问题分成若干个子问题。
 *	⒊.对每一子问题求解，得到子问题的局部最优解。
 *	⒋.把子问题的解局部最优解合成原来解问题的一个解。
 * 	实现该算法的过程：
 *		从问题的某一初始解出发；
 *		while 能朝给定总目标前进一步 do
 *		求出可行解的一个解元素；
 *		由所有解元素组合成问题的一个可行解。
 * Description:
 *	超市找零钱，为使找回的纸币数(硬币数)最少。从最大面值的币种开始，按递减的顺序考虑各币种，先尽量用大面值的币种，当不足大面值币种的金额时，去考虑较小面值
 */

#include <stdio.h>

#define MAXN 9
int parvalue[MAXN] = {10000, 5000, 1000, 500, 200, 100, 50, 20, 10};
int num[MAXN] = {0};

int exchange(int n)
{
	int i, j;
	for (i=0; i<MAXN; i++)
		if (n > parvalue[i])
			break;		/*找到比n小的最大面额*/
	while (n > 0 && i < MAXN) {
		if (n >= parvalue[i]) {
			n -= parvalue[i];
			num[i]++;
		} else if (n < 10 && n >= 5) {
			num[MAXN-1]++;
			break;
		} else
			i++;
	}
	return 0;
}

int main(void)
{
	int i;
	float m;

	printf("Please input the m:");
	scanf("%f", &m);
	exchange((int)100*m);
	printf("\n%.2fzucheng: \n", m);
	for (i=0; i<MAXN; i++)
		if (num[i] > 0)
			printf("%6.2f:%d\n", (float)parvalue[i]/100.0, num[i]);
	return 0;
}
