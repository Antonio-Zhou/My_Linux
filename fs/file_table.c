/*
 * 定义一个文件句柄(描述符) 结构数组
 * */

/*
 *  linux/fs/file_table.c
 *
 *  (C) 1991  Linus Torvalds
 */

#include <linux/fs.h>

struct file file_table[NR_FILE];
