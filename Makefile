#
# if you want the ram-disk device, define this to be the
# size in blocks.
#
RAMDISK = #-DRAMDISK=512

AS86	=as86 -0 -a		#8086汇编编译器和连接器，-0:生成8086目标程序；-a:生成与gas和gld部分兼容的代码
LD86	=ld86 -0

AS	=gas	#GNU汇编编译器和连接器
LD	=gld

#gld 运行时用到的选项。
#-s:  输出文件中省略所有的符号信息
#-x:  删除所有局部符号
#-M:  表示需要在标准输出设备(显示器)上打印连接映像(link map)，是指由连接程序产生的一种内存地址映像
#其中列出了程序段装入到内存中的位置信息。具体如下:
#·目标文件及符号信息映射到内存中的位置
#·公共符号如何存放
#·连接中包含的所有文件成员及其引用的符号
LDFLAGS	=-s -x -M

#gcc 是GNU C 程序编译器，对于UNIX类的脚本(script)程序而言，
#在引用定义的标识符时，需在前面加上$符号并括号括住标识符
CC	=gcc $(RAMDISK)

#指定gcc使用的选项。
#-Wall:  打印所有警告信息
#-O:对代码进行优化
#'-f标志':  指定与机器无关的编译标志
#'-fstrength-reduce': 用于优化循环语句
#'-fomit-frame-pointer': 对于无需帧指针的函数，不要把帧指针保留在寄存器中。
#					  这样在函数中可以避免对帧指针的操作和维护。
#'-fcombine-regs':   用于指明编译器在组合编译阶段把复制一个寄存器到另一个寄存器的指令组合在一起
#'-mstring-insns':  是Linus在学习gcc编增加的选项，用于gcc-1.40在复制结构等操作时使用386 CPU 的字符串指令，
#				可去掉
CFLAGS	=-Wall -O -fstrength-reduce -fomit-frame-pointer \
-fcombine-regs -mstring-insns

# 下面CPP  是gcc  的前(预)处理器程序。
#前处理器用于进行程序中的宏替换处理，条件编译处理以及包含进指定文件的内容，
#		即把'#include'  指定的文件包含进来。源程序文件中所有以符号'#'  开始的行均需要由前处理器进行处理
#  		程序中所有'#define' 定义的宏都会使用其定义部分替换掉。
#		程序中所有'#if'  '#ifdef'  '#ifndef'  和'#endif'  等条件判别行用于确定是否包含其指定范围中的语句
#'-nostdinc -Iinclude':   不要搜索标准头文件目录中的文件，即不用系统/usr/include/目录下的头文件，
#					而是使用'-I'  选项指定的目录或者是在当前目录中搜索头文件
CPP	=cpp -nostdinc -Iinclude

#
# ROOT_DEV specifies the default root-device when making the image.
# This can be either FLOPPY, /dev/xxxx or empty, in which case the
# default of /dev/hd6 is used by 'build'.
#

#这里的/dev/hd6  对应第2  个硬盘的第1  个分区。这是Linus 开发Linux内核时自己机子上根文件系统所在的目录
ROOT_DEV=/dev/hd6

#下面是kerne 目录,mm目录 和fs目录所产生的目标代码文件，
#		为了方便引用这里将它们用ARCHIVES(归档文件)标识符表示
ARCHIVES=kernel/kernel.o mm/mm.o fs/fs.o

#快和字符设备库文件。'.a'  表示该文件时个归档文件，也即包含有许多可执行二进制代码子程序
#集合的库文件，通常是用GNU 的ar程序生成。
#ar  是GNU   的二进制文件处理程序，用于创建,  修改，以及从归档文件中抽取文件
DRIVERS =kernel/blk_drv/blk_drv.a kernel/chr_drv/chr_drv.a
MATH	=kernel/math/math.a		  #数学运算库文件
LIBS	=lib/lib.a		#由lib/目录中的文件锁编译生成的通用库文件


#下面是make  老式隐式后缀规则
#改行指示make  利用下面的命令将所有的'.c'  文件编译生成'.s'  汇编程序.
#':' 表示下面是该规则的命令。
#整句表示让gcc   采用前面的CFLAGS  所指的选项以及仅使用include/目录中的头文件，在适当的编译后不进行
#	汇编就停止(-S)。从而产生与输入的各个C  文件对应的汇编语言形式的代码语言文件。
#-o:  其后是输出文件的形式
#'$*.s':   自动目标变量
#'$<' :  代表第一个先决条件
.c.s:
	$(CC) $(CFLAGS) \
	-nostdinc -Iinclude -S -o $*.s $<

#将所有的.s汇编程序文件编译成.o  文件，使用gas
#'-c':  表示只编译或汇编，不进行连接操作
.s.o:
	$(AS) -c -o $*.o $<

#使用gcc 将C语言将文件编译成目标文件，但不连接
.c.o:
	$(CC) $(CFLAGS) \
	-nostdinc -Iinclude -c -o $*.o $<


#all :  创建Makefile  所知的最顶层目标，这里就是Image   文件，也就是引导启动盘映像文件bootimage。
all:	Image


#说明目标文件是有:  后面的4    个元素产生的
Image: boot/bootsect boot/setup tools/system tools/build

#使用tools目录下的build工具程序将bootsect   setup   system  文件以  $(ROOT_DEV)  为根目录系统设备组装成内核映像文件Image
	tools/build boot/bootsect boot/setup tools/system $(ROOT_DEV) > Image

#sync 同步命令是迫使缓冲块数据立即写盘并更新超级块
	sync


#disk  这个目标要由Image产生.
disk: Image
	dd bs=8192 if=Image of=/dev/PS0

tools/build: tools/build.c		#由tools  目录下的build.c    程序生成执行文件build
	$(CC) $(CFLAGS) \
	-o tools/build tools/build.c		#编译生成执行程序build  的命令

boot/head.o: boot/head.s		#利用上面给出的.s  .o  规则生成head.o  目标文件

#tools/system文件由:   右边所列的元素生成
tools/system:	boot/head.o init/main.o \
		$(ARCHIVES) $(DRIVERS) $(MATH) $(LIBS)

#生成system的命令
#'> System.map':  表示gld  要将连接 映像重定位存放在System.map  中
	$(LD) $(LDFLAGS) boot/head.o init/main.o \
	$(ARCHIVES) \
	$(DRIVERS) \
	$(MATH) \
	$(LIBS) \
	-o tools/system > System.map

#数学协处理函数文件math.a  :  进入kernel/math/目录;  运行make工具程序
kernel/math/math.a:
	(cd kernel/math; make)

kernel/blk_drv/blk_drv.a:		#生成块设备库文件blk_drv.a  其中含有可重定位目标文件
	(cd kernel/blk_drv; make)

kernel/chr_drv/chr_drv.a:		#字符设备函数文件chr_drv.a
	(cd kernel/chr_drv; make)

kernel/kernel.o:		#内核目标模块kernel.o
	(cd kernel; make)

mm/mm.o:		#内存管理模块mm.o
	(cd mm; make)

fs/fs.o:	#文件系统目标模块fs.o
	(cd fs; make)

lib/lib.a:		#库函数lib.a
	(cd lib; make)

boot/setup: boot/setup.s					#这里开始的三行使用8086   汇编和连接器
	$(AS86) -o boot/setup.o boot/setup.s		#对setup.s文件进行编译生成setup   文件
	$(LD86) -s -o boot/setup boot/setup.o	#-s  :  去除目标文件中的符号信息

boot/bootsect:	boot/bootsect.s			#同上，生成bootsect.o   磁盘引导块
	$(AS86) -o boot/bootsect.o boot/bootsect.s
	$(LD86) -s -o boot/bootsect boot/bootsect.o


#在bootsect.s  文本程序开始处添加一行有关system  模块文件长度信息，
#		在把system  模块加载到内存期间用于指明系统模块的长度。
#首先生成只含有'SYSSIZE = system文件实际长度'  一行信息的tmp.s   文件，然后将bootsect.s  文件添加其后。
#取得system  长度的方法是:  首先利用 ls   对编译生成的system   模块文件进行长列表显示，
#用 grep   取得列表行上文件字节数字段信息，并定向保存到tmp.s  临时文件中。
#cut   用于剪切字符串，
#tr   去除行尾的回车符，其中(实际长度+  15)/16 :    获取用'节'表示的长度信息。1节=16字节
#此规则已不用!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
tmp.s:	boot/bootsect.s tools/system
	(echo -n "SYSSIZE = (";ls -l tools/system | grep system \
		| cut -c25-31 | tr '\012' ' '; echo "+ 15 ) / 16") > tmp.s
	cat boot/bootsect.s >> tmp.s

#当执行'make clean'  时，执行这里的命令，去除所有编译连接生成的文件，
clean:
	rm -f Image System.map tmp_make core boot/bootsect boot/setup
	rm -f init/*.o tools/system tools/build boot/*.o
	(cd mm;make clean)
	(cd fs;make clean)
	(cd kernel;make clean)
	(cd lib;make clean)

#首先执行上面的clean  规则，然后对linux/  目录进行压缩，生成'backup.Z' 压缩文件
backup: clean
	(cd .. ; tar cf - linux | compress - > backup.Z)
	sync

#用于长生个文件之间的依赖
#目的是为了让make  命令用它们来关系确定是否要重建一个目标对象。例如改动过
#输出为删除Makefile  中'### Dependencies'  行后面的所有行，并生成tmp_make  临时文件。
#		指定目录(init/)  的每一个C  文件执行gcc   预处理操作
#		-M  :   告诉预处理程序cpp 输出藐视每个目标文件相关性的规则，并且这些符合make  语法
#  '$$i'就是'$($i)'   $i  就是这句前面的shell  变量'i'  的值
dep:
	sed '/\#\#\# Dependencies/q' < Makefile > tmp_make
	(for i in init/*.c;do echo -n "init/";$(CPP) -M $$i;done) >> tmp_make
	cp tmp_make Makefile
	(cd fs; make dep)		#对fs/  目录下的Makefile 文件也做同样的操作
	(cd kernel; make dep)
	(cd mm; make dep)

### Dependencies:
init/main.o : init/main.c include/unistd.h include/sys/stat.h \
  include/sys/types.h include/sys/times.h include/sys/utsname.h \
  include/utime.h include/time.h include/linux/tty.h include/termios.h \
  include/linux/sched.h include/linux/head.h include/linux/fs.h \
  include/linux/mm.h include/signal.h include/asm/system.h include/asm/io.h \
  include/stddef.h include/stdarg.h include/fcntl.h 
