/*
 *  linux/lib/string.c
 *
 *  (C) 1991  Linus Torvalds
 */

#ifndef __GNUC__
#error I want gcc!
#endif


/*消除了两个关键字*/
#define extern		/*定义为空*/
#define inline		/*定义为空*/
#define __LIBRARY__
#include <string.h>
