/*
 * 包含一个内核使用的时间函数mktime()
 * 用于计算从1970年1月1日0时起到开机当日的秒数,作为开机秒时间,仅在init/main.c中被调用一次
 * */

/*
 *  linux/kernel/mktime.c
 *
 *  (C) 1991  Linus Torvalds
 */

#include <time.h>

/*
 * This isn't the library routine, it is only used in the kernel.
 * as such, we don't care about years<1970 etc, but assume everything
 * is ok. Similarly, TZ etc is happily ignored. We just do everything
 * as easily as possible. Let's find something public for the library
 * routines (although I think minix times is public).
 */
/*
 * PS. I hate whoever though up the year 1970 - couldn't they have gotten
 * a leap-year instead? I also hate Gregorius, pope or no. I'm grumpy.
 */
#define MINUTE 60						/*1分钟的秒数*/
#define HOUR (60*MINUTE)					/*1小时的秒数*/
#define DAY (24*HOUR)						/*1天的秒数*/
#define YEAR (365*DAY)						/*1年的秒数*/

/* interestingly, we assume leap-years */
/*以年为界限,定义了每个月开始时的秒数时间*/
static int month[12] = {
	0,
	DAY*(31),
	DAY*(31+29),
	DAY*(31+29+31),
	DAY*(31+29+31+30),
	DAY*(31+29+31+30+31),
	DAY*(31+29+31+30+31+30),
	DAY*(31+29+31+30+31+30+31),
	DAY*(31+29+31+30+31+30+31+31),
	DAY*(31+29+31+30+31+30+31+31+30),
	DAY*(31+29+31+30+31+30+31+31+30+31),
	DAY*(31+29+31+30+31+30+31+31+30+31+30)
};

/*
 * 功能: 将读取出来的信息转换成从1970年1月1日午夜0时开始计起到现在的以秒为单位的时间
 * 参数: struct tm *tm-->tm中各字段已经在ini/main.c中被赋值,信息取自CMOS
 * 返回值:
 * */
 
long kernel_mktime(struct tm * tm)
{
	long res;
	int year;

	/*tm_year为什么是两位*/
	year = tm->tm_year - 70;
/* magic offsets (y+1) needed to get leapyears right.*/
	/*res=这些年经过的秒数+每个闰年时多1天的秒数时间+当年到当月时的秒数*/
	res = YEAR*year + DAY*((year+1)/4);
	res += month[tm->tm_mon];
/* and (y+2) here. If it wasn't a leap-year, we have to adjust */
	if (tm->tm_mon>1 && ((year+2)%4))		/*不是闰年*/
		res -= DAY;
	res += DAY*(tm->tm_mday-1);
	res += HOUR*tm->tm_hour;
	res += MINUTE*tm->tm_min;
	res += tm->tm_sec;
	return res;
}
