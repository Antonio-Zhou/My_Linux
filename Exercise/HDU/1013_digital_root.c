/*
 *  Created on:     2014-08-01
 * 	Author:     Eric Chou
 *
 * 	Digital Roots
 *
 * Problem Description
 * 	The digital root of a positive integer is found by summing the digits of the integer. If the resulting value is a single digit then that digit is the
 * digital root.
 * 	If the resulting value contains two or more digits, those digits are summed and the process is repeated. This is continued as long as necessary to obtain
 * a single digit.
 * 	For example, consider the positive integer 24. Adding the 2 and the 4 yields a value of 6. Since 6 is a single digit, 6 is the digital root of 24.
 * Now consider the positive integer 39. Adding the 3 and the 9 yields 12. Since 12 is not a single digit, the process must be repeated. Adding the 1 and the 2
 * yeilds 3, a single digit and also the digital root of 39.
 *
 * Input
 * 	The input file will contain a list of positive integers, one per line. The end of the input will be indicated by an integer value of zero.
 * Output
 * 	For each integer in the input, output its digital root on a separate line of the output.
 *
 * Sample Input
 * 	24 39 0
 * Sample Output
 * 	6 3
 *
 * 翻译:
 * 	给你一个数,求每个数位上的数字之和.如果和大于9,就继续求'和'的每一个数位上的数字之和
 * */

#include<stdio.h>
#include<string.h>

int get_root(int sum)
{
	int ans = 0;

        while (sum) {
		ans += sum % 10;
		sum = sum / 10;
	}

	return ans;
}

int main(void)
{
	int sum, ans;
	int i, j;
	char a[100];

	while (~scanf("%s", a) && a[0] != '0') {
		sum = 0;
		for (i = 0; i < strlen(a); i++)
			sum += (a[i] - '0');

		while (sum > 9)
			sum = get_root(sum);

		printf("%d\n", sum);
	}

	return 0;
}
