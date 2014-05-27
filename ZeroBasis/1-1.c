/*
 *	Created on:	Mar 10 2014
 *	Author:		Eric Zhou
 *	Email:		zhoubb.eric@gmail.com
 *
 * Description
 *	看商品，猜价格。使用二分法去猜测，会更节省时间
 */

#include <stdio.h>

int guess_price(int right_price);
int guess_time=0; 
int main(void)
{
	int right_price=0;
	printf("Please set the price:\n");
	scanf("%d", &right_price);
	do {
		guess_time++;
		continue;
	} while(guess_price(right_price) != 1);
	return 0;
}

int guess_price(int right_price)
{
	int guess_price=0, i=0;
	printf("Please guess the price:\n");
	printf("actor:");
	scanf("%d", &guess_price);
	printf("host:");
	if (guess_price < right_price) {
		printf("low\n");
		return 0;
	} else if (guess_price > right_price) {
		printf("high\n");
		return 0;
	} else {
		printf("OK,you are get it! and the time is %d\n", guess_time);
		return 1;
	}
}
