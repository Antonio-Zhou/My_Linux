/*
 * 	base.h	
 *
 *	Created on:	2013-05-10
 *	    Author:	Eric Zhou
 */

#include<string.h>
#include<ctype.h>
#include<malloc.h>
#include<limits.h>
#include<stdio.h>
#include<stdlib.h>
#include<math.h>

/* 函数结果状态代码 */
#define TRUE 1
#define FALSE 0
#define OK 1
#define ERROR 0
#define INFEASIBLE -1

/* Status是函数的类型,其值是函数结果状态代码，如OK等 */
typedef int Status;
/* Boolean是布尔类型,其值是TRUE或FALSE */
typedef int Boolean;
