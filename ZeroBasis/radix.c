#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define minRADIX 2    //最小进制数
#define maxRADIX 36   //最大进制数
#define maxN     36   //定义最大位数为36位

int checkInput(char *str, int in_radix);//输入检查函数，同时处理输入数据
unsigned int mypow(int base, int n);
void trim_in_radix2out_radix(char *str, int len, int in_radix, int out_radix);

int main(void)
{
	int in_radix = 2, out_radix = 2;//输入，输出进制数
	char inputstr[maxN] = {'\0'};//存储输入整数
	int len = 0;//存储输入字符串长度
	printf("%d-%d进制任意进制转换程序\n", minRADIX, maxRADIX);
	printf("************************\n");
	//输入数据程序段
	while (1) {
		printf("*************************");
		printf("\n请输入：  输入数的进制数(%d-%d):", minRADIX, maxRADIX);
		scanf("%d", &in_radix);
		if (in_radix < minRADIX || in_radix > maxRADIX) {
			printf("\n您输入的输入数的进制数有误，请重新输入(%d-%d之间的整数)！\n", minRADIX, maxRADIX);
			continue;
		}
		while (1) {
			printf("\n请输入%d进制数:", in_radix);
			scanf("%s", inputstr);
			len = strlen(inputstr);
			if (!checkInput(inputstr, in_radix)) {
				printf("\n 您的输入有误，请重新输入%d进制数！\n", in_radix);
				continue;
			}
			break;
		}
		while (1) {
			printf("\n请输入：  输出数的进制数(%d-%d):", minRADIX, maxRADIX);
			scanf("%d", &out_radix);
			if (out_radix < minRADIX || out_radix > maxRADIX) {
				printf("\n您输入的输出数的进制数有误，请重新输入(%d-%d)之间的整数)！\n", minRADIX, maxRADIX);
				continue;
			}
			break;
		}
		break;
	}
	//输出数据程序段
	trim_in_radix2out_radix(inputstr, len, in_radix, out_radix);
    	return 0;
}

/*由于输入的是字符，用以下转换,
    假定G代表16....Z代表35，F之前同十六进制
    {'0','1','2','3','4','5','6','7','8','9'
    ,'A','B','C','D','E','F','G','H','I','J'
    ,'K','L','M','N','O','P','Q','R','S','T'
    ,'U','V','W','X','Y','Z'};*/
int checkInput(char *str, int in_radix)
{
	int i;
	for (i=0; str[i]!='\0'; i++) {  
		if (str[i] >= '0' &&  str[i] <= '9')
			str[i] = str[i] - '0';
		if (str[i] >= 'a' && str[i] <= 'z')
			str[i] = str[i] - 'a' + 10;
		if (str[i] >= 'A' &&  str[i] <= 'Z')
			str[i] = str[i] - 'A' + 10;
		if (str[i] >= in_radix)
			return 0;//输入不符合in_radix进制，返回0，重新输入
	}
	return 1;//输入符合，返回1
}

/*求base的n次方*/
unsigned int mypow(int base, int n)
{
	unsigned int num;
	if (n == 0)
		return 1;
	if (n > 0)
		num = base * mypow(base, n-1);
	return num;
}

void trim_in_radix2out_radix(char *str, int len, int in_radix, int out_radix)
{
	unsigned long num = 0;
	char outputstr[maxN] = {'\0'};
	int i, j, temp, length;
	for (i=0; i<len; i++)  
		num += str[i] * mypow(in_radix, len-1-i);
	if (10 == out_radix)
		printf("\n从%d进制数转换为%d进制数，结果为：%lu \n", in_radix, out_radix, num);
	else {
		//从10进制转换为out_radix进制，结果为实际结果的倒序
		for (i=0; num!=0; i++) {
			outputstr[i] = (char)(num % out_radix);
			if (outputstr[i] >= 0 && outputstr[i] <= 9)
				outputstr[i] = outputstr[i] + '0';
			if (outputstr[i] >= 10 && outputstr[i] <= 35)
				outputstr[i] = outputstr[i] +' A' -  10;
			num /= out_radix;
		}
		//倒序排列字符串
		for (length=strlen(outputstr),j=0; j<(length+1)/2; j++) {
			temp = outputstr[j];
			outputstr[j] = outputstr[length-1-j];
			outputstr[length-1-j] = temp;
		}
		printf("\n从%d进制数转换为%d进制数，结果为：%s\n", in_radix, out_radix, outputstr);
	}
}
