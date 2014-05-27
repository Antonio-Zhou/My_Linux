/*
 * 	LCS-2.c	
 *
 *	Created on:	2013-05-07
 *	    Author:	Eric Zhou
 */

#include <stdio.h>
#include <malloc.h>
#include <string.h>

#define max 100

void LCSLength(int m, int n, char *x, char *y, char b[][max]);
void LCS(int i, int j, char *x, char b[][max]);

int main(void)
{
	char x[max] = {'e', 'g', 'c', 'r', 'b', 'a', 'd'};
	char y[max] = {'e', 'd', 'h', 'e', 'b', 'd'};
	int m = 7;
	int n = 6;
	int j, k;
	char b[max][max] = {0};

	printf("    ");
	for(j = 1 ; j <= n ; j++ )
		printf("%2c", y[j-1]);
	printf("\n");
	printf(" ");
	for(k=1; k<=m; k++)
		 printf("%2d", 0);
	printf("\n");
	LCSLength(m, n, x, y, b);
	printf("最长公共序列串为:");
	LCS(m, n, x, b);
	printf("\n\n");
	return 0;
}

void LCSLength(int m, int n, char *x, char *y, char b[][max])
{
	int i , j , k;
	int c[max][max];

	for (i=1; i<=m; i++ )
		c[i][0] = 0;
	for (i=1; i<=n; i++ )
		c[0][i] = 0;

	for (i=1; i<=m; i++ ) {
		for ( j = 1 ; j <= n ; j++ ) {  
			if (x[i-1] == y[j-1]) {
				c[i][j] = c[i-1][j-1] + 1;
				b[i][j] = '\\';
			} else if ( c[i-1][j] >= c[i][j-1] ) {
				c[i][j] = c[i-1][j];
				b[i][j] = '|';
			} else {
				c[i][j] = c[i][j-1];
				b[i][j] = '-';
			}
		}

		printf("    ");

		for (j=1; j<=n; j++)
			printf("%2c",b[i][j]);
		printf("\n");
		printf("%2c",x[i-1]);
		printf("%2d",0);
		for( j = 1 ; j <= n ; j++ )
			printf("%2d",c[i][j]);
		printf("\n");
	}
	printf("\n");
}

void LCS(int i, int j, char *x, char b[][max])
{
	if (i==0 || j==0) 
		return;
	if ( b[i][j] == '\\' ) {
		LCS(i-1, j-1, x, b);
		printf("%c", x[i-1]);
	} else if (b[i][j] == '|') {
		LCS(i-1, j, x, b);
	} else {
		LCS(i, j-1, x, b);
	}
}
