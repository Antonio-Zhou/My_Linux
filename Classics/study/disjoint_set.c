/*
 *	Created on:	2014-07-15
 *	    Author:	Eric Chou
 *
 * 并查集(Disjoint set)
 * 题目大意:
 * 	学校有N个学生,编号为0~N-1,现在0号学生感染了非典,凡是和0在一个社团的人就会感染,并且这些人如果还参加了别的社团,他所在的社团照样全部感染,求感染的人数
 *
 * 并查集的学习:
 * 	一种简单的用途广泛的集合.并查集是若干个不相交集合,能够实现较快的合并和判断元素所在集合的操作.
 * 	应用很多,如其求无向图的连通分量个数等r最完美的应用当属:实现Kruskar算法求最小生成树.
 *
 * 并查集的精髓(即它的三种操作,结合实现代码模板进行理解):
 * 	1.make_set(x)	把每一个元素初始化为一个集合
 * 		初始化后每一个元素的父亲节点是它本身,每一个元素的祖先节点也是它本身(也可以根据情况而变).
 * 	2.find_set(x)	查找一个元素所在的集合
 * 		查找一个元素所在的集合,其精髓是找到这个元素所在集合的祖先!
 * 		判断两个元素是否属于同一集合,只要看他们所在集合的祖先是否相同即可.
 * 		合并两个集合,也是使一个集合的祖先成为另一个集合的祖先
 * 	3.union(x,y)	合并x,y所在的两个集合
 * 		利用Find_Set找到其中两个集合的祖先,将一个集合的祖先指向另一个集合的祖先
 * 并查集的优化
 * 	1.find_set(x)时 路径压缩
 * 		寻找祖先时我们一般采用递归查找,但是当元素很多亦或是整棵树变为一条链时,每次find_set(x)都是O(n)的复杂度
 * 		减小复杂度:即当我们经过"递推"找到祖先节点后,"回溯"的时候顺便将它的子孙节点都直接指向祖先,这样以后再次find_set(x)时复杂度就变成O(1)
 * 	2.union(x,y)时 按秩合并
 * 		即合并的时候将元素少的集合合并到元素多的集合中,这样合并之后树的高度会相对较小
 *
 * 并查集的练习:
 * 	POJ 1611 The Suspects          最基础的并查集
 * 	POJ 2524 Ubiquitous Religions 最基本的并查集
 * 	POJ 1182 食物链       并查集的拓展
 * 		注意: 只有一组数据;要充分利用题意所给条件:有三类动物A,B,C，这三类动物的食物链构成了有趣的环形。A吃B， B吃C，C吃A。也就是说:只有三个group
 * 	POJ 2492 A Bug's Life 并查集的拓展
 * 		法一:深度优先遍历,每次遍历记录下该点是男还是女，只有:男-〉女，女-〉男满足，否则，找到同性恋，结束程序。
 * 		法二:二分图匹配
 * 		法三:并查集的拓展:和1182很像，只不过这里就有两组，而1182是三组,1611无限制
 * 	POJ 1861 Network == zju_1542    并查集+自定义排序+贪心求"最小生成树"
 * 		答案不唯一.不过在ZOJ上用QSORT()和SORT()都能过，在POJ上只有SORT()才能过...
 * 	POJ 1703 Find them, Catch them 并查集的拓展
 * 		这个和POJ 2492 A Bug's Life很像
 * 		注意:And of course, at least one of them belongs to Gang Dragon, and the same for Gang Snake. 就是说只有两个组。
 * 	POJ 2236 Wireless Network        并查集的应用
 * 		需要注意的地方:
 * 			1.并查集
 * 			2.N的范围，可以等于1001
 * 			3.从N+1行开始，第一个输入的可以是字符串。
 * 	POJ 1988 Cube Stacking            并查集很好的应用
 * 		1,与银河英雄传说==NOI2002 Galaxy一样
 * 		2,增加了一个数组behind[x],记录战舰x在列中的相对位置
 * 		3.详细解题报告见银河英雄传说。
 * 	JOJ 1905 Freckles   == POJ 2560 最小生成树
 * 		法一:Prim算法
 * 		法二:并查集实现Kruskar算法求最小生成树
 * 	JOJ 1966 Super Market III == PKU 1456 Supermarket 带限制的作业排序问题(贪心+并查集)
 * 提高题目:
 * 	POJ 2912 Rochambeau
 * 	POJ 1733 Parity game
 * 	POJ 1308 Is It A Tree
 * */

#include <stdio.h>
#include <stdlib.h>

#define MAXNUM	30001

int rank[MAXNUM];		/*存储节点所在集合元素的个数*/
int father[MAXNUM];		/*存储节点的父节点*/

/*初始化集合*/
void make_set(int x)
{
	father[x] = -1;
	rank[x] = 1;
}

/*查找x元素所在的集合,返回根节点*/
int find_set(int x)
{
	/* 保存待查找值x */
	int r = x, tmp;

	/* 找到根节点r */
	while (father[r] != -1) {
		r = father[r];
	}
	/* 压缩路径,将路径上所有r的子孙都连接到r上 */
	while (x != r) {
		tmp = father[x];
		father[x] = r;
		x = tmp;
	}

	return x;
}

/*合并a,b所在的集合*/
int union_set(int a, int b)
{
	a = find_set(a);
	b = find_set(b);

	/*如果两个元素在同一个集合则不需要合并*/
	if (a == b)
		return 0;
	/*将小集合合并到大集合中,更新集合个数*/
	if (rank[a] <= rank[b]) {
		father[a] = b;
		rank[b] += rank[a];
	} else {
		father[b] = a;
		rank[a] += rank[b];
	}

	return 1;
}

int main()
{
	int total, group;	/*total-->总的人数, group-->分成的组数*/
	int rank_per_group, first, next;
	int i, j, ret;

	while (1) {
		scanf("%d%d", &total, &group);
		if (total + group == 0)
			break;

		for (i = 0; i < total; i++) {
			make_set(i);
		}

		for (i = 0; i < group; i++) {
			scanf("%d", &rank_per_group);
			scanf("%d", &first);

			for (j = 1; j < rank_per_group; j++) {
				scanf("%d", &next);
				ret = union_set(first, next);
				printf("the %d times, ret = %d\n", j, ret);
			}
		printf("The %d group is %d\n", i, rank[find_set(0)]);
		}
		printf("The 0 set is %d\n", rank[find_set(0)]);
	}
	return 0;
}
