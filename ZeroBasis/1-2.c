/*
 *	Created on:	Mar 11 2014
 *	Author:		Eric Zhou
 *	Email:		zhoubb.eric@gmail.com
 *
 * Description
 *	斐波那契数列
 * 经典问题：
 *	假定你有一雄一雌一对刚出生的兔子，它们在长到一个月大小时开始交配，在第二月结束时，雌兔子产下另一对兔子，过了一个月后它们也开始繁殖，
 * 如此这般持续下去。每只雌兔在开始繁殖时每月都产下一对兔子，假定没有兔子死亡，在一年后总共会有多少对兔子？
 * 其他有趣的问题：
 *	1，有一段楼梯有10级台阶,规定每一步只能跨一级或两级,要登上第10级台阶有几种不同的走法?
 *	答：这就是一个斐波那契数列：登上第一级台阶有一种登法；登上两级台阶，有两种登法；登上三级台阶，有三种登法；登上四级台阶，有五种方法……所以，1，2，3，5，8，
 * 13……登上十级，有89种。
 *	2，数列中相邻两项的前项比后项的极限是多少，就是问，当n趋于无穷大时，F(n)/F(n+1)的极限是多少？
 *	答：这个可由它的通项公式直接得到，极限是(-1+√5)/2，这个就是所谓的黄金分割点，也是代表大自然的和谐的一个数字。
 * 数学表示：
 *	Fibonacci数列的数学表达式就是：
 *		F(n) = F(n-1) + F(n-2)
 *		F(1) = 1
 *		F(2) = 1
 * 注：
 *	编译时，加上-lm
 */

/*介绍5种实现方法*/
#include <stdio.h>
#include <string.h>
#include <math.h>

/*定义2*2的矩阵*/
struct matrix {
	long a[2][2];
};

/*二叉递归程序*/
long fib1(int n)
{
	if (n <= 2)
		return 1;
	else
		return (fib1(n-1) + fib1(n-2));
}

/*一叉递归程序*/
long fib(int n, long a, long b, int count)
{
	if (count == n)
		return b;
	return fib(n, b, a+b, ++count);
}

long fib2(int n)
{
	return fib(n, 0, 1, 1);
}

/*迭代解法*/
/*也可以用数组将每次计算的f(n)存储下来，用来下次计算用（空间换时间）*/
long fib3 (int n)
{
	long x = 0, y = 1;
	int j;
	for (j = 1; j < n; j++) {
		y = x + y;
		x = y - x;
	}
	return y;
}

/*矩阵乘法*/
struct matrix mul(struct matrix x, struct matrix y)
{
	struct matrix res;
	int i, j, k;
	long sum;
	for (i=0; i<2; i++)
		for (j=0; j<2; j++) {
			sum = 0;
			for (k=0; k<2; k++)
				sum += x.a[i][k]*y.a[k][j];
			res.a[i][j] = sum;
		}
	x = res;
	return x;
}

struct matrix pow_matrix(struct matrix x, long e)
{
	struct matrix ans, temp;
	if (e == 0) {
		ans.a[0][0]=1;
		ans.a[0][1]=0;
		ans.a[1][0]=0;
		ans.a[1][1]=1;
		return ans;
	}
	if (e == 1)
		return x;
	temp = pow_matrix(x, e >> 1);
	ans = mul(temp, temp);
	if (e & 1)
		ans = mul(ans, x);
	return ans;
}

long fib4(int n) 
{
	struct matrix ans;
	struct matrix base = {{1, 1, 1, 0}};
	ans = pow_matrix(base, n-1);
	return ans.a[0][0];
}

/*公式解法*/
long fib5(int n)
{
	double z = sqrt(5.0);
	double x = (1 + z) / 2;
	double y = (1 - z) / 2;
	return (pow(x, n) - pow(y, n)) / z + 0.5;
}


int main(void)
{
	int n;
	printf("Please input the NUM:");
	scanf("%d", &n);
	printf("fb1:%ld\n", fib1(n));
	printf("fb2:%ld\n", fib2(n));
	printf("fb3:%ld\n", fib3(n));
	printf("fb4:%ld\n", fib4(n));
	printf("fb5:%ld\n", fib5(n));
	return 0;
}
