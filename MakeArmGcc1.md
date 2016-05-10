Make Gcc For Arm
========================================

GNU交叉工具链下载地址
----------------------------------------

ARM工具链的官方下载地址：

* ftp://ftp.arm.linux.org.uk/pub/arm-linux-/toolchain/cross2.95.3.tar.bz2

需解压到根目录下：tar jxvf cross2.95.3.tar.bz2 -C /

* ftp://ftp.arm.linux.org.uk/pub/arm-linux-/toolchain/cross3.0.tar.bz2

* ftp://ftp.arm.linux.org.uk/pub/arm-linux-/toolchain/cross3.2.tar.bz2

可以下载arm-linux-gcc-3.4.1 编译linux2.6的内核:

* http://www.handhelds.org/download/projects/toolchain/

常用工具介绍
----------------------------------------

* arm-linux-as                编译ARM汇编程序
* arm-linux-ar                把多个.o文件合并成一个.o文件或静态库文件（.a文件）
* arm-linux-ramlib            为库文件建立索引，相当于arm-linux-ar -s
* arm-linux-ld                连接器（Linker），把多高.o文件或库文件连接成一个可执行文件
* arm-linux-objdump           查看目标文件（.o文件）和库文件(.a文件）信息
* arm-linux-objcopy           转换可执行文件的格式
* arm-linux-strip             去掉elf可执行文件的信息
* arm-linux-readelf           读elf可执行文件的信息
* arm-linux-gcc               编译.c或.s头的C文件或汇编程序
* arm-linux-nm                用来列出目标文件的符号清单

交叉编译工具链介绍
----------------------------------------

为什么要用交叉编译器？
交叉编译通俗地讲就是在一种平台上编译出能运行在体系结构不同的
另一种平台上的程式，比如在PC平台（X86 CPU）上编译出能运行在以
ARM为内核的CPU平台上的程式，编译得到的程式在X86 CPU平台上是
不能运行的，必须放到ARM CPU平台上才能运行，虽然两个平台用的都
是Linux系统。这种方法在异平台移植和嵌入式研发时非常有用。相对和
交叉编译，平常做的编译叫本地编译，也就是在当前平台编译，编译得到
的程式也是在本地执行。用来编译这种跨平台程式的编译器就叫交叉编译器，
相对来说，用来做本地编译的工具就叫本地编译器。所以要生成在目标机上
运行的程式，必须要用交叉编译工具链来完成。在裁减和制定Linux内核用于
嵌入式系统之前，由于一般嵌入式研发系统存储大小有限，通常都要在性能
优越的PC上建立一个用于目标机的交叉编译工具链，用该交叉编译工具链
在PC上编译目标机上要运行的程式。交叉编译工具链是个由编译器、连接器
和解释器组成的综合研发环境，交叉编译工具链主要由binutils、gcc和glibc
3个部分组成。有时出于减小 libc 库大小的考虑，也能用别的 c 库来代替
glibc，例如 uClibc、dietlibc 和 newlib。建立交叉编译工具链是个相当
复杂的过程，如果不想自己经历复杂繁琐的编译过程，网上有一些编译好的
可用的交叉编译工具链能下载，但就以学习为目的来说读者有必要学习自己
制作一个交叉编译工具链。本章通过具体的实例讲述基于ARM的嵌入式Linux
交叉编译工具链的制作过程。

构建交叉编译器的第一个步骤就是确定目标平台。在GNU系统中，每个目标平台
都有一个明确的格式，这些信息用于在构建过程中识别要使用的不同工具的
正确版本。因此，当在一个特定目标机下运行GCC时，GCC便在目录路径中查找
包含该目标规范的应用程式路径。GNU的目标规范格式为CPU-PLATFORM-OS。
例如x86/i386 目标机名为i686-pc-linux-gnu。

通常构建交叉工具链有3种方法:

* 分步编译和安装交叉编译工具链所需要的库和原始码，最终生成交叉编译工具链。
  该方法相对比较困难，适合想深入学习构建交叉工具链的读者。如果只是想使用交叉工具链，
  建议使用方法二或方法三构建交叉工具链。

* 通过Crosstool脚本工具来实现一次编译生成交叉编译工具链，该方法相对于方法一要简单许多，
  并且出错的机会也非常少，建议大多数情况下使用该方法构建交叉编译工具链。

* 直接通过网上（ftp.arm.kernel.org.uk）下载已制作好的交叉编译工具链。该方法的好处不用多说，
  当然是简单省事，但和此同时该方法有一定的弊端就是局限性太大，因为毕竟是别人构建好的，
  也就是固定的没有灵活性，所以构建所用的库及编译器的版本也许并不适合你要编译的程式，
  同时也许会在使用时出现许多莫名的错误，建议读者慎用此方法。


分步构建交叉编译链
----------------------------------------

分步构建，顾名思义就是一步一步地建立交叉编译链，不同于Crosstool脚本
工具一次编译生成的方法，该方法适合那些希望深入学习了解构建交叉编译
工具链的读者。该方法相对来说难度较大，通常情况下困难重重，

表2.1  所需资源

安装包  下载地址

* linux-2.6.10.tar.gz ftp.kernel.org
* glibc-2.3.2.tar.gz ftp.gnu.org
* binutils-2.15.tar.bz2 ftp.gnu.org
* glibc-linuxthreads-2.3.2.tar.gz ftp.gnu.org
* gcc-3.3.6.tar.gz ftp.gnu.org

通过相关站点下载以上资源后，就能开始建立交叉编译工具链了。

### 1．建立工作目录

首先建立工作目录，工作目录就是在什么目录下构建交叉工具链，
目录的构建一般没有特别的需求，能根据个人喜好建立。以下
所建立的目录是作者自定义的，当前的用户定义为mike，
因此用户目录为/home/mike，在用户目录下首先建立一个工作目录（armlinux），建立工作目录的命令行操作如下：

```
# cd /home/mike
# mkdir armlinux
```

再在这个工作目录armlinux下建立3个目录 build-tools、kernel 和 tools。具体操作如下：

```
# cd armlinux
# mkdir build-tools kernel tools
```

其中各目录的作用如下。
* build-tools  用来存放下载的binutils、gcc、glibc等原始码和用来编译这些原始码的目录；
* kernel  用来存放内核原始码；
* tools  用来存放编译好的交叉编译工具和库文件。

### 2．建立环境变量

该步骤的目的是为了方便重复输入路径，因为重复操作每件相同
的事情总会让人觉得非常麻烦，如果读者不习惯使用环境变量就
能略过该步，直接输入绝对路径就能。声明以下环境变量的目的
是在之后编译工具库的时候会用到，非常方便输入，尤其是能降低输错路径的风险。

# export PRJROOT=/home/mike/armlinux
# export TARGET=arm-linux
# export PREFIX=$PRJROOT/tools
# export TARGET_PREFIX=$PREFIX/$TARGET
# export PATH=$PREFIX/bin:$PATH

注意，用export声明的变量是临时的变量，也就是当注销或更换了控制台，
这些环境变量就消失了，如果还需要使用这些环境变量就必须重复export操作，
所以有时会非常麻烦。值得庆幸的是，环境变量也能定义在bashrc文件中，
这样当注销或更换控制台时，这些变量就一直有效，就不用老是export这些变量了。

### 3．编译、安装Binutils

Binutils是GNU工具之一，他包括连接器、汇编器和其他用于目标文件和档案的工具，
他是二进制代码的处理维护工具。安装Binutils工具包含的程式有
addr2line、ar、as、c++filt、gprof、ld、nm、objcopy、objdump、
ranlib、readelf、size、strings、strip、libiberty、libbfd和libopcodes。
对这些程式的简单解释如下。

* addr2line  把程式地址转换为文件名和行号。在命令行中给他一个地址和一个可执行文件名，
             他就会使用这个可执行文件的调试信息指出在给出的地址上是哪个文件及行号。
* ar  建立、修改、提取归档文件。归档文件是包含多个文件内容的一个大文件，其结构确保了能恢复原始文件内容。
* as  主要用来编译GNU C编译器gcc输出的汇编文件，产生的目标文件由连接器ld连接。
* c++filt  连接器使用他来过滤 C++ 和 Java 符号，防止重载函数冲突。
* gprof  显示程式调用段的各种数据。
* ld  是连接器，他把一些目标和归档文件结合在一起，重定位数据，并连接符号引用。通常，建立一个新编译程式的最后一步就是调用ld。
* nm  列出目标文件中的符号。
* objcopy  把一种目标文件中的内容复制到另一种类型的目标文件中。
* objdump  显示一个或更多目标文件的信息。使用选项来控制其显示的信息，他所显示的信息通常只有编写编译工具的人才感兴趣。
* ranlib  产生归档文件索引，并将其保存到这个归档文件中。在索引中列出了归档文件各成员所定义的可重分配目标文件。
* readelf  显示elf格式可执行文件的信息。
* size  列出目标文件每一段的大小及总体的大小。默认情况下，对于每个目标文件或一个归档文件中的每个模块只产生一行输出。
* strings  打印某个文件的可打印字符串，这些字符串最少4个字符长，也能使用选项
           -n设置字符串的最小长度。默认情况下，他只打印目标文件初始化和可加载段中
           的可打印字符；对于其他类型的文件他打印整个文件的可打印字符。这个程式对于了解非文本文件的内容非常有帮助。
* strip  丢弃目标文件中的全部或特定符号。
*libiberty  包含许多GNU程式都会用到的函数，这些程式有getopt、obstack、strerror、strtol和strtoul。
* libbfd  二进制文件描述库。
* libopcode  用来处理opcodes的库，在生成一些应用程式的时候也会用到他。

Binutils工具安装依赖于Bash、Coreutils、Diffutils、GCC、Gettext、Glibc、
Grep、Make、Perl、Sed、Texinfo等工具。

介绍完Binutils工具后，下面将分步介绍安装binutils-2.15的过程。

(1). 首先解压binutils-2.15.tar.bz2包，命令如下：

```
# cd $PRJROOT/build-tools
# tar -xjvf binutils-2.15.tar.bz2
```

接着设置Binutils工具，建议建立一个新的目录用来存放设置和编译文件，这样能使源文件和编译文件独立开，具体操作如下：

```
# cd $PRJROOT/build-tools
# mkdir build-binutils
# cd build-binutils
# ../ binutils-2.15/configure --target=$TARGET --prefix=$PREFIX
```

其中选项?target的意思是制定生成的是 arm-linux 的工具，--prefix
是指出可执行文件安装的位置。执行上述操作会出现非常多check信息，最
后产生 Makefile 文件。接下来执行make和安装操作，命令如下：

```
# make
# make install
```

该编译过程较慢，需要数十分钟，安装完成后查看/home/mike/armlinux/tools/bin
目录下的文件，如果查看结果如下，表明此时Binutils工具已安装结束。

```
# ls $PREFIX/bin
arm-linux-addr2line   arm-linux-ld        arm-linux-ranlib    arm-linux-strip
arm-linux-ar             arm-linux-nm         arm-linux-readelf
arm-linux-as             arm-linux-objcopy  arm-linux-size
arm-linux-c++filt        arm-linux-objdump  arm-linux-strings
```

### 4．获得内核头文件

编译器需要通过系统内核的头文件来获得目标平台所支持的系统函数调用所需要的信息。
对于Linux内核，最佳的方法是下载一个合适的内核，然后复制获得头文件。
需要对内核做一个基本的设置来生成正确的头文件；不过，不必编译内核。对于本例中的目标arm-linux，需要以下步骤。

(1). 在kernel目录下解压linux-2.6.10.tar.gz内核包，执行命令如下：

```
# cd $PRJROOT/kernel
# tar -xvzf linux-2.6.10.tar.gz
```

(2). 接下来设置编译内核使其生成正确的头文件，执行命令如下：

```
# cd linux-2.6.10
# make ARCH=arm CROSS_COMPILE=arm-linux- menuconfig
```

其中ARCH=arm表示是以arm为体系结构，CROSS_COMPILE=arm-linux-表示是以
arm-linux-为前缀的交叉编译器。也能用config和xconfig来代替menuconfig，
推荐用make menuconfig，这也是内核研发人员用的最多的设置方法。注意在
设置时一定要选择处理器的类型，这里选择三星的S3C2410（System Type->ARM System Type->/Samsung S3C2410），
如图2.1所示。设置完退出并保存，检查一下内核目录中的include/linux/version.h和include/linux/autoconf.h
文件是不是生成了，这是编译glibc时要用到的，如果version.h 和 autoconf.h 文件存在，说明生成了正确的头文件。


复制头文件到交叉编译工具链的目录，首先需要在/home/mike/armlinux/tools/arm-linux目录下建立工具的
头文件目录inlcude，然后复制内核头文件到此目录下，具体操作如下：

```
# mkdir -p $TARGET_PREFIX/include
# cp -r $PRJROOT/kernel/linux-2.6.10/include/linux $TARGET_PREFIX/include
# cp -r $PRJROOT/kernel/linux-2.6.10/include/asm-arm $TARGET_PREFIX/include/asm
```

### 5．编译安装boot-trap gcc

这一步的目的主要是建立arm-linux-gcc工具，注意这个gcc没有glibc库的支持，
所以只能用于编译内核、BootLoader等不必C库支持的程式，后面创建C库也要用到
这个编译器，所以创建他主要是为创建C库做准备，如果只想编译内核和BootLoader，
那么安装完这个就能到此结束。安装命令如下：

```
# cd $PRJROOT/build-tools
# tar -xvzf gcc-3.3.6.tar.gz
# mkdir build-gcc
# cd gcc-3.3.6
# vi gcc/config/arm/t-linux
```

由于是第一次安装ARM交叉编译工具，没有支持libc库的头文件，所以在gcc/config/arm/t-linux
文件中给变量TARGET_LIBGCC2_CFLAGS增加操作参数选项-Dinhibit_libc  -D__gthr_ posix_h
来屏蔽使用头文件，否则一般默认会使用/usr/inlcude头文件。

将TARGET_LIBGCC2-CFLAGS = -fomit-frame-pointer ?fPIC
改为TARGET_LIBGCC2- CFLAGS=-fomit-frame-pointer-fPIC -Dinhibit_libc -D__gthr_posix_h

修改完t-linux文件后保存，紧接着执行设置操作，如下命令：

```
# cd build-gcc
# ../ build-gcc /configure --target=$TARGET --prefix=$PREFIX --enable-languages=c
  --disable-threads --disable-shared
```

其中选项--enable-languages=c表示只支持C语言，--disable-threads表示去掉thread功能，
这个功能需要glibc的支持。--disable-shared表示只进行静态库编译，不支持共享库编译。

接下来执行编译和安装操作，命令如下：

```
# make
# make install
```

安装完成后，在/home/mike/armlinux/tools/bin下查看，如果arm-linux-gcc等工具已生成，表示boot-trap gcc工具已安装成功。

### 6．建立glibc库

glibc是GUN C库，他是编译Linux系统程式非常重要的组成部分。安装glibc-2.3.2版本之前推荐先安装以下的工具：

* GNU make 3.79或更新；
* GCC 3.2或更新；
* GNU binutils 2.13或更新。

首先解压glibc-2.2.3.tar.gz和glibc-linuxthreads-2.2.3.tar.gz原始码，操作如下：

```
# cd $PRJROOT/build-tools
# tar -xvzf glibc-2.2.3.tar.gz
# tar -xzvf glibc-linuxthreads-2.2.3.tar.gz --directory=glibc-2.2.3
```

然后进行编译设置，glibc-2.2.3设置前必须新建一个编译目录，否则在
glibc-2.2.3目录下不允许进行设置操作，此处在$PRJROOT/build-tools
目录下建立名为build-glibc的目录，设置操作如下：

```
# cd $PRJROOT/build-tools
# mkdir build-glibc
# cd build-glibc
# CC=arm-linux-gcc ../glibc-2.2.3 /configure --host=$TARGET --prefix="/usr"
--enable-add-ons --with-headers=$TARGET_PREFIX/include
```

选项CC=arm-linux-gcc是把CC（Cross Compiler）变量设成刚编译完的gcc，
用他来编译glibc。--prefix="/usr"定义了一个目录用于安装一些和目标机器
无关的数据文件，默认情况下是/usr/local目录。--enable-add-ons是告诉glibc
用linuxthreads包，在上面已将他放入glibc原始码目录中，这个选项等价于
-enable-add-ons=linuxthreads。--with-headers告诉glibc linux内核头文件的目录位置。

设置完后就能编译和安装 glibc了，具体操作如下：

```
# make
# make install
```

### 7．编译安装完整的gcc

由于第一次安装的gcc没有交叉glibc的支持，目前已安装了glibc，所以需要重新编译
来支持交叉glibc。并且上面的gcc也只支持C语言，目前能让他同时支持C语言还要和C++语言。具体操作如下：

```
# cd $PRJROOT/build-tools/gcc-2.3.6
# ./configure --target=arm-linux --enable-languages=c,c++ --prefix=$PREFIX
# make
# make install
```

安装完成后会发目前$PREFIX/bin目录下又多了arm-linux-g++ 、arm-linux-c++等文件。

```
# ls $PREFIX/bin
arm-linux-addr2line arm-linux-g77        arm-linux-gnatbind arm-linux-ranlib
arm-linux-ar         arm-linux-gcc       arm-linux-jcf-dump  arm-linux-readelf
arm-linux-as         arm-linux-gcc-3.3.6 arm-linux-jv-scan   arm-linux-size
arm-linux-c++        arm-linux-gccbug   arm-linux-ld         arm-linux-strings
arm-linux-c++filt  arm-linux-gcj       arm-linux-nm         arm-linux-strip
arm-linux-cpp        arm-linux-gcjh      arm-linux-objcopy   grepjar
arm-linux-g++        arm-linux-gcov       arm-linux-objdump   jar
```

### 8．测试交叉编译工具链

到此为止，已介绍完了用分步构建的方法建立交叉编译工具链。下面通过一个简单的程式测试
刚刚建立的交叉编译工具链看是否能够正常工作。写一个最简单的hello.c源文件，内容如下：

```
#include
int main( )
{

     printf(“Hello,world!＼n”);
     return 0;
}
```

通过以下命令进行编译，编译后生成名为hello的可执行文件，通过file命令能查看文件的类型。
当显示以下信息时表明交叉工具链正常安装了，通过编译生成了ARM体系可执行的文件。
注意，通过该交叉编译链编译的可执行文件只能在ARM体系下执行，不能在基于X86的普通PC上执行。

```
# arm-linux-gcc -o hello hello.c
# file hello
hello: ELF 32-bit LSB executable, ARM, version 1 (ARM), for GNU/Linux 2.4.3,
dynamically linked (uses shared libs), not stripped
```

用Crosstool工具构建交叉工具链
----------------------------------------

Crosstool是一组脚本工具集，可构建和测试不同版本的gcc和glibc，
用于那些支持glibc的体系结构。他也是个开源项目，下载地址是:

* http://kegel.com/crosstool

用Crosstool构建交叉工具链要比上述的分步编译容易得多，并且也方便许多，
对于仅仅为了工作需要构建交叉编译工具链的读者建议使用此方法。用Crosstool工具构建所需资源如下所示:

安装包  下载地址

* crosstool-0.42.tar.gz http://kegel.com/crosstool
* linux-2.6.10.tar.gz ftp.kernel.org
* binutils-2.15.tar.bz2 ftp.gnu.org
* gcc-3.3.6.tar.gz ftp.gnu.org
* glibc-2.3.2.tar.gz ftp.gnu.org
* glibc-linuxthreads-2.3.2.tar.gz ftp.gnu.org
* linux-libc-headers-2.6.12.0.tar.bz2 ftp.gnu.org

### 1．准备资源文件

首先从网上下载所需资源文件linux-2.6.10.tar.gz、binutils-2.15.tar.bz2、
gcc-3.3.6.tar.gz、glibc- 2.3.2.tar.gz、glibc-linuxthreads-2.3.2.tar.gz和
linux-libc-headers-2.6.12.0.tar.bz2。然后将这些工具包文件放在新建的
/home/mike/downloads目录下，最后在/home/mike目录下解压crosstool-0.42.tar.gz，命令如下：

```
# cd /home/mike
# tar ?xvzf crosstool-0.42.tar.gz
```

### 2．建立脚本文件

接着需要建立自己的编译脚本，起名为arm.sh，为了简化编写arm.sh，寻找一个最接近的
脚本文件demo-arm.sh作为模板，然后将该脚本的内容复制到arm.sh，修改arm.sh脚本，具体操作如下：

```
# cd crosstool-0.42
# cp demo-arm.sh arm.sh
# vi arm.sh
```

修改后的arm.sh脚本内容如下：

```
#!/bin/sh
set -ex
TARBALLS_DIR=/home/mike/downloads   # 定义工具链源码所存放位置。
RESULT_TOP=/opt/crosstool            # 定义工具链的安装目录
export TARBALLS_DIR RESULT_TOP
GCC_LANGUAGES="c,c++"                # 定义支持C, C++语言
export GCC_LANGUAGES
# 创建/opt/crosstool目录
mkdir -p $RESULT_TOP
# 编译工具链，该过程需要数小时完成。
eval ’cat arm.dat gcc-3.3.6-glibc-2.3.2.dat’  sh all.sh --notest
echo Done.
```

### 3．建立设置文件

在arm.sh脚本文件中需要注意arm.dat和gcc-3.3.6-glibc-2.3.2.dat两个文件，
这两个文件是作为Crosstool的编译的设置文件。其中arm.dat文件内容如下，
主要用于定义设置文件、定义生成编译工具链的名称及定义编译选项等。

```
KERNELCONFIG=’pwd’/arm.config  # 内核的设置
TARGET=arm-linux-                # 编译生成的工具链名称
TARGET_CFLAGS="-O"                # 编译选项
```

gcc-3.3.6-glibc-2.3.2.dat文件内容如下，该文件主要定义编译过程中
所需要的库及他定义的版本，如果在编译过程中发现有些库不存在时，
Crosstool会自动在相关网站上下载，该工具在这点上相对比较智能，也非常有用。

```
BINUTILS_DIR=binutils-2.15
GCC_DIR=gcc-3.3.6
GLIBC_DIR=glibc-2.3.2
GLIBCTHREADS_FILENAME=glibc-linuxthreads-2.3.2
LINUX_DIR=linux-2.6.10
LINUX_SANITIZED_HEADER_DIR=linux-libc-headers-2.6.12.0
```

### 4．执行脚本

将Crosstool的脚本文件和设置文件准备好之后，开始执行arm.sh脚本来编译交叉编译工具。具体执行命令如下：

```
# cd crosstool-0.42
# ./arm.sh
```

经过数小时的漫长编译之后，会在/opt/crosstool目录下生成新的交叉编译工具，其中包括以下内容：

```
arm-linux-addr2line arm-linux-g++        arm-linux-ld         arm-linux-size
arm-linux-ar         arm-linux-gcc        arm-linux-nm         arm-linux-strings
arm-linux-as         arm-linux-gcc-3.3.6 arm-linux-objcopy   arm-linux-strip
arm-linux-c++        arm-linux-gccbug    arm-linux-objdump   fix-embedded-paths
arm-linux-c++filt   arm-linux-gcov       arm-linux-ranlib
arm-linux-cpp        arm-linux-gprof      arm-linux-readelf
```

### 5．添加环境变量

然后将生成的编译工具链路径添加到环境变量PATH上去，

```
export PATH=/opt/crosstool/gcc-3.3.6-glibc-2.3.2/arm-linux/bin:$PATH
```

设置完环境变量，也就意味着交叉编译工具链已构建完成，然后就能用2.2.1.8节中的方法进行测试刚刚建立的工具链，此处就不用再赘述。

常见问题
----------------------------------------

* 问题1：编译boot-trap gcc时出现如图2.3所示的错误，
提示：crti.o: No such file: No such file or directory collect2: ld returned 1 exit status，为什么会出现这样的错误？

参考答案：
由于在设置时没有选择--disable-shared 选项，该选项的意思是只编译静态库。
默认选项为--enable-shared，而libf2c 和libiberty 不支持共享库。

* 问题2：Glibc里静态库和共享库有什么差别？

参考答案：
应用程式在链接静态库时，会把引用到的数据和代码放到生成的可执行文件中，
程式运行时就不再需要库了。应用程式链接共享库时，连接器不会把引用到的
数据和代码放到可执行文件中，而仅做一个标记，当程式运行时，系统会去加载
相应的共享库。链接共享库时，可执行文件的大小会小一些，但运行时依赖于共享库。
启动静态库和共享库的方法分别是在设置时用 --disable-shared和--enable-shared选项。

* 问题3：本地编译器和交叉编译器的作用。

参考答案：
编译器能生成用来在和编译器本身所在的计算机和操作系统（平台）相同的环境下运行的目标代码，
这种编译器叫做本地编译器。另外，编译器也能生成用来在其他平台上运行的目标代码，这种编译器叫做交叉编译器。
