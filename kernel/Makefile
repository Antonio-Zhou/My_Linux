#
# Makefile for the FREAX-kernel.
#
# Note! Dependencies are done automagically by 'make dep', which also
# removes any old dependencies. DON'T put your own dependencies here
# unless it's something special (ie not a .c file).
#

AR	=gar			#GNU二进制文件处理程序,用于创建,修改以及归档文件中抽取文件
AS	=gas			#GNU汇编程序
LD	=gld			#GNU连接程序
LDFLAGS	=-s -x			#连接程序的所有参数
				#-s: 输出文件中省略所有符号信息
				#-x: 删除所有局部符号
CC	=gcc			#GNU C 语言编译器

#c编译程序选项
#-Wall: 显示所有的警告信息
#-O: 优化选项,优化代码长度和执行时间
#-fstrength-reduce: 优化循环执行代码,排除重复变量
#-fomit-frame-pointer: 省略保存不必要的框架指针
#-fcombine-regs: 合并寄存器,减少寄存器类的使用
#-finline-functions: 将所有简单短小的函数代码嵌入调用程序中
#-mstring-insns: Linus自己填加的优化选项,以后都不再使用
#-nostdinc -I../include: 不使用默认路径中的包含文件,而使用这里指定目录中的(../include)
CFLAGS	=-Wall -O -fstrength-reduce -fomit-frame-pointer -fcombine-regs \
	-finline-functions -mstring-insns -nostdinc -I../include

#C前处理选项
#-E: 只运行C前处理,对所有指定的C程序进行预处理并将处理结果输出到标准输出设备或指定的输出文件
CPP	=gcc -E -nostdinc -I../include

#make产生文件的规则
.c.s:
	$(CC) $(CFLAGS) \
	-S -o $*.s $<
.s.o:
	$(AS) -c -o $*.o $<
.c.o:
	$(CC) $(CFLAGS) \
	-c -o $*.o $<

#定义目标文件变量OBJS
OBJS  = sched.o system_call.o traps.o asm.o fork.o \
	panic.o printk.o vsprintf.o sys.o exit.o \
	signal.o mktime.o

#在有了先决条件OBJS后使用下面的命令链接成目标kernel.o
#-r: 指示生成可重定向的输出,即产生可以作为连接器ld输入的目标文件
kernel.o: $(OBJS)
	$(LD) -r -o kernel.o $(OBJS)
	sync

#执行清理工具
#-f: 忽略不存在的文件,并且不显示删除信息
clean:
	rm -f core *.o *.a tmp_make keyboard.s
	for i in *.c;do rm -f `basename $$i .c`.s;done
	(cd chr_drv; make clean)		#进入chr_drv/目录,执行该目录Makefile中的clean规则
	(cd blk_drv; make clean)
	(cd math; make clean)

#用于检查各文件之间的依赖关系,不是很明白
dep:
	sed '/\#\#\# Dependencies/q' < Makefile > tmp_make
	(for i in *.c;do echo -n `echo $$i | sed 's,\.c,\.s,'`" "; \
		$(CPP) -M $$i;done) >> tmp_make
	cp tmp_make Makefile
	(cd chr_drv; make dep)
	(cd blk_drv; make dep)

### Dependencies:
exit.s exit.o : exit.c ../include/errno.h ../include/signal.h \
  ../include/sys/types.h ../include/sys/wait.h ../include/linux/sched.h \
  ../include/linux/head.h ../include/linux/fs.h ../include/linux/mm.h \
  ../include/linux/kernel.h ../include/linux/tty.h ../include/termios.h \
  ../include/asm/segment.h 
fork.s fork.o : fork.c ../include/errno.h ../include/linux/sched.h \
  ../include/linux/head.h ../include/linux/fs.h ../include/sys/types.h \
  ../include/linux/mm.h ../include/signal.h ../include/linux/kernel.h \
  ../include/asm/segment.h ../include/asm/system.h 
mktime.s mktime.o : mktime.c ../include/time.h 
panic.s panic.o : panic.c ../include/linux/kernel.h ../include/linux/sched.h \
  ../include/linux/head.h ../include/linux/fs.h ../include/sys/types.h \
  ../include/linux/mm.h ../include/signal.h 
printk.s printk.o : printk.c ../include/stdarg.h ../include/stddef.h \
  ../include/linux/kernel.h 
sched.s sched.o : sched.c ../include/linux/sched.h ../include/linux/head.h \
  ../include/linux/fs.h ../include/sys/types.h ../include/linux/mm.h \
  ../include/signal.h ../include/linux/kernel.h ../include/linux/sys.h \
  ../include/linux/fdreg.h ../include/asm/system.h ../include/asm/io.h \
  ../include/asm/segment.h 
signal.s signal.o : signal.c ../include/linux/sched.h ../include/linux/head.h \
  ../include/linux/fs.h ../include/sys/types.h ../include/linux/mm.h \
  ../include/signal.h ../include/linux/kernel.h ../include/asm/segment.h 
sys.s sys.o : sys.c ../include/errno.h ../include/linux/sched.h \
  ../include/linux/head.h ../include/linux/fs.h ../include/sys/types.h \
  ../include/linux/mm.h ../include/signal.h ../include/linux/tty.h \
  ../include/termios.h ../include/linux/kernel.h ../include/asm/segment.h \
  ../include/sys/times.h ../include/sys/utsname.h 
traps.s traps.o : traps.c ../include/string.h ../include/linux/head.h \
  ../include/linux/sched.h ../include/linux/fs.h ../include/sys/types.h \
  ../include/linux/mm.h ../include/signal.h ../include/linux/kernel.h \
  ../include/asm/system.h ../include/asm/segment.h ../include/asm/io.h 
vsprintf.s vsprintf.o : vsprintf.c ../include/stdarg.h ../include/string.h 
