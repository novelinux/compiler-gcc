全手工制作arm-linux交叉编译工具链
========================================

当我们需要从源代码编译出一个能运行在ARM 架构上的程序的时候，
我们可以有两种方法。第一种是像我们平常一样，使用相同架构机器
上的编译器，编译出运行在同一架构上的程序。在嵌入式系统领域，
这是较难实现的。嵌入式系统的在设计的时候由于考虑到功耗，体积
等要求，性能往往较弱。在编译一些比较大规模的程序时候，嵌入式
系统在编译上耗费的时间，是我们无法忍受的。因此，我们需要使用交叉编译技术。

交叉编译技术，是一种在一个异构平台上编译出目标平台程序的一种技术。
理论上来说，交叉工具链可以用在任何两种异构的系统中，例如，我们
可以构建出PowerPC-ARM工具链，Sun Sparc-x86工具链等等。目前交叉
工具链一般用于目标平台计算能力较弱，需要其他计算能力较强的平台
帮助产生可运行软件的场合。在基于ARM的嵌入式系统开发中，我们一般会
使用x86架构的计算机系统作为工作站，故最为常用的是x86-ARM交叉工具链。

目前市场上存在着各种各样不用的x86-ARM交叉工具链，有ARM公司自家在RVDS
开发工具中整合的armcc，有Microsoft 在Platform Builder中的armv4I 编译器等。
一般来说，这些由商业公司提供的工具链，都不会附有工具链相关的源代码和制作方法，
灵活性不足，并且它们一般都与整套开发系统捆绑销售使用，成本较高。为了更好的理解
这种技术和降低成本，获得更好的灵活性，市场上有不少的公司使用国际开源组织GNU开发
的工具链作为他们产品开发的工具。本文下面的章节就为大家展示一个完整的GNU工具链构建过程。
其中可能涉及到一些GNU Linux系统的基本操作知识，如果大家对此认识不深，请自行查阅相关资料。


工具链原理及构建方法
----------------------------------------

### 原理：

每一个软件，在编译的过程中，都要经过一系列的处理，才能从源代码变成可执行的目标代码。

这一系列处理包括：预编译，高级语言编译，汇编，连接及重定位。这一套流程里面用到的
每个工具和相关的库组成的集合，就称为工具链（tool chain）。以GNU的开发工具GCC为例，
它就包括了预编译器cpp，c编译器gcc，汇编器as，和连接器ld等。在GNU自己对工具链定义中，
还加进了一套额外的用于处理二进制包的工具包binutils，整个工具链应该是GCC+binutils+Glibc，
不过大家后面将会看到binutils其实与Glibc无关，它是可以被独立安装的，所以GNU工具链也可以狭义地被理解为GCC+Glibc。

一般的情况下，工具链运行的环境和它产生的目标代码的环境是一致的。例如我们在VC中编译一个程序，
工具链运行在x86平台，产生的也是运行在x86平台上的代码。但实际上，工具链产生的目标代码的
运行平台是可以跟工具链运行的环境不一致的。这种产生与运行环境不一致的目标代码的工具链，
称为交叉工具链(cross-compiler tool chain)，使用这种工具链的编译过程对应地被称为
交叉编译(cross-compile)。在GNU中，一般在普通工具链名称的前面加上特定的前缀，以表示
是什么类型的工具链。例如x86-arm的工具链，预编译器是arm-linux-cpp，c编译器arm-linux-gcc等。

要构建出一个交叉工具链，需要解决三个问题。
* 一是这个工具链必须是可以运行在原工作站平台上的。

* 二是我们需要更换一个与目标平台对应的汇编器，使得工具链能产生对应的目标代码.

* 三是要更换一套与目标平台对应的二进制库，使得工具链在连接时能找到正确的二进制库。


具体实现：
----------------------------------------

* http://cross-lfs.org/view/clfs-sysroot/arm/index.html

在此部分的制作过程主要参考以上网站完成，但其中制作过程有错，
在本文中Garfield Trump予以了纠正。

### 下载所需源码包

* http://ftp.gnu.org/gnu/binutils/binutils-2.18.tar.bz2
* http://svn.cross-lfs.org/svn/repos/cross-lfs/branches/clfs-sysroot/patches/binutils-2.18-posix-1.patch
* http://ftp.gnu.org/gnu/gcc/gcc-4.2.1/gcc-4.2.1.tar.bz2
* http://svn.cross-lfs.org/svn/repos/cross-lfs/branches/clfs-sysroot/patches/gcc-4.2.1-posix-1.patch
* http://svn.cross-lfs.org/svn/repos/cross-lfs/branches/clfs-sysroot/patches/gcc-4.2.1-cross_search_paths-1.patch
* http://ftp.gnu.org/gnu/glibc/glibc-2.6.1.tar.bz2
* http://svn.cross-lfs.org/svn/repos/cross-lfs/branches/clfs-sysroot/patches/glibc-2.6.1-sysdep_cancel-1.patch
* http://svn.cross-lfs.org/svn/repos/cross-lfs/branches/clfs-sysroot/patches/glibc-2.6.1-localedef_segfault-1.patch
* http://svn.cross-lfs.org/svn/repos/cross-lfs/branches/clfs-sysroot/patches/glibc-2.6.1-libgcc_eh-1.patch
* http://svn.cross-lfs.org/svn/repos/cross-lfs/branches/clfs-sysroot/patches/glibc-2.6.1-hppa_nptl-1.patch
* http://svn.cross-lfs.org/svn/repos/cross-lfs/branches/clfs-sysroot/patches/glibc-2.6.1-cross_hacks-1.patch
* http://svn.cross-lfs.org/svn/repos/cross-lfs/branches/clfs-sysroot/patches/glibc-2.6.1-RTLD_SINGLE_THREAD_P-1.patch

### 设置环境变量

```
export TAR=/tar //设置源文件压缩包的存放目录路径
export TOP=/usr/local/arm //设置目标工具链目录的顶成目录
export CLFS=${TOP}/clfs //设置编译过程工作目录
export SYSROOT=${TOP}/4.2.1 //设置目标工具链的工作目录
export TARGET_PREFIX=${TOP}/4.2.1 //设置目标工具链的安装目录
```


注意：这里的SYSROOT 和 TARGET_PREFIX 设成相同。跟http://cross-lfs.org/view/clfs-sysroot/arm/index.html
上所讲的有所不同，也是该网站出错最严重的地方，因为不但制作过程需要 Linux 内核头文件跟glibc 的头文件，
而且以后使用这个制作出来的目标交叉编译工具链来编译程序的时候也会用到这些头文件，特别是Linux内核的头文件。
所以Garfield Trump 把SYSROOT 和 TARGET_PREFIX设成相同，以便把头文件直接安装到目标交叉编译工具链的工作目录中。

```
LINUX_VERSION=2.6.22.6 //设置 Linux 的版本号
BINUTILS_VERSION=2.18 //设置 binutils 的版本号
GLIBC_VERSION=2.6.1 //设置 glibc 的版本号
GCC_VERSION=4.2.1 //设置 目标工具链的版本号
unset CFLAGS //禁用两个环境变量，因为可能导致编译过程出错
```

unset CXXFLAGS //默认情况下这两个环境变量为空，但随不同的Linux 平台的不同，
或者用户自己之前的使用过程中设置了这两个变量设置目标工具链的目标体系，
Gcc编译器会根据这个变量来确定目标交叉编译工具链是为那个目标体系的cpu编译程序的。

export CLFS_TARGET="arm-linux" //设置目标交叉编译工具链的编译器（arm-linux-gcc）的路径，
因为在完成第一阶段arm-linux-gcc的编译后，我们将会把它安装到这里，
并且使用它来编译出第二阶段 arm-linux-gcc 制作所依赖的动态链接库 glibc。

```
export PATH=${TARGET_PREFIX}/bin:/bin:/usr/bin:$PATH
```

### 建立相应工作目录安装Linux 内核头文件

```
rm -rf ${TOP} //清除可能干扰制作过程的目录
install -dv ${SYSROOT}/usr/include //建立Linux头文件安装目录
install -dv ${TOP}/source //建立源码解压目录
cd ${TOP}/source //进入源码解压目录
cp ${TAR}/linux-${LINUX_VERSION}.tar.gz ./ //拷贝源码包到解压目录
gzip -d ./linux-${LINUX_VERSION}.tar.gz //解压Linux 内核源码包
tar -xf linux-${LINUX_VERSION}.tar //解包 Linux 内核源码包
rm -rf linux-${LINUX_VERSION}.tar //解包完后删除 Linux 内核源码包
cd linux-${LINUX_VERSION} //进入Linux 内核源码目录
make s3c2410_defconfig ARCH=arm //设置 Linux 内核源码匹配的cpu 体系
```

注意：下面一步是Garfield Trump 给予纠正的一步，在官方网站
http://cross-lfs.org/view/clfs-sysroot/arm/index.html下是没有这一步的，会导致后面的编译出错。

```
make include/linux/version.h //生成Linux 版本头文件。
```

复制制作目标交叉编译工具链所要的Linux头文件

```
cp -a include/{asm-generic,linux,mtd,scsi,sound} ${SYSROOT}/usr/include
cp -a include/asm-arm ${SYSROOT}/usr/include/asm
```

### 建立binutils

```
mkdir -p ${TARGET_PREFIX} //建立目标交叉编译工具链安装目录
cd ${TOP}/source //进入源码解压目录
tar -jxf ${TAR}/binutils-${BINUTILS_VERSION}.tar.bz2 //解压binutils 源码包到此目录
cd binutils-${BINUTILS_VERSION} //进入 binutils 的源码目录
patch -Np1 -i ${TAR}/binutils-${BINUTILS_VERSION}-posix-1.patch //给binutils打补丁
mkdir -p ${TOP}/source/BUILD/binutils-${BINUTILS_VERSION} //建立 binutils 的编译目录
cd ${TOP}/source/BUILD/binutils-${BINUTILS_VERSION} //进入 binutils 的编译目录
//设立汇编器的环境变量
AR=ar
AS=as
//对 binutils 进行配置
${TOP}/source/binutils-${BINUTILS_VERSION}/configure --prefix=${TARGET_PREFIX} --target=${CLFS_TARGET} --with-sysroot=${SYSROOT} --disable-nls --enable-shared --disable-multilib
make configure-host //编译 binutils
make
make install //安装 binutils
//复制后续编译所要的 binutils 的头文件
cp -v ${TOP}/source/binutils-${BINUTILS_VERSION}/include/libiberty.h ${SYSROOT}/usr/include
```

### 安装Glibc 的头文件

```
cd ${TOP}/source //进入源码解压目录
tar -jxf ${TAR}/glibc-${GLIBC_VERSION}.tar.bz2 //解压 Glibc 的源码包到此目录
```

//改变源码目录名，为了标识这份源码是为安装头文件而解压、配置、补丁的。
http://cross-lfs.org/view/clfs-sysroot/arm/index.html 上说的是要我们在
编译安装完成每一个阶段后，把用过的源码包删除，下次用到时再次解压，以免
这次的补丁、配置影响下一次的编译。Garfield Trump 则采用改变目录名的方式来实现，
目的在于保留这些编译、设置过的源码，以后可以直接用这些编译过的内容直接安装，不需要再次编译。因为编译是很耗时的事情。

```
mv glibc-${GLIBC_VERSION} glibc-${GLIBC_VERSION}-headers-build
cd glibc-${GLIBC_VERSION}-headers-build //进入glibc头文件安装源码目录
```

//下面两行是为了消除在配置glibc时 gcc 对glibc的依赖。因为这里只是为了安装头文件，并不是要配置、编译、安装 Glibc。

```
cp configure{,.orig}
sed -e 's/3.4/3.[0-9]/g' configure.orig > configure
//解压glibc 的cpu体系支持源码包到glibc 头文件安装源码目录，因为glibc 默认是没有ARM cpu支持的，GNU组织专门加入了此源码包来解决此问题。
tar -jxf ${TAR}/glibc-ports-${GLIBC_VERSION}.tar.bz2
//改变cpu 体系支持源码目录为 ports ,因为glibc对所有加入的而外支持包的默认搜索目录为 ports。
mv glibc-ports-${GLIBC_VERSION} ports
//建立glibc头文件配置、安装的工作目录
mkdir -p ${TOP}/source/BUILD/glibc-${GLIBC_VERSION}-headers-build
//进入glibc头文件配置、安装的工作目录
cd ${TOP}/source/BUILD/glibc-${GLIBC_VERSION}-headers-build
//以下三步是为了把glibc头文件配置成支持 NPTL（linux 的新线程标准）而设立的必须条件，并把这些条件写入缓存文件（可以从下面的配置选项 --cache-file=config.cache看出）
echo "libc_cv_forced_unwind=yes" > config.cache
echo "libc_cv_c_cleanup=yes" >> config.cache
echo "libc_cv_arm_tls=yes" >> config.cache
CC=gcc //指定配置使用的编译器
//配置glibc
${TOP}/source/glibc-${GLIBC_VERSION}-headers-build/configure --prefix=/usr --host=${CLFS_TARGET} --with-headers=${SYSROOT}/usr/include --cache-file=config.cache
make install-headers install_root=${SYSROOT} //安装 glibc头文件
cp -v bits/stdio_lim.h ${SYSROOT}/usr/include/bits //复制编译后续编译所要的头文件
//建立必要的地头文件，内容为空，因为这个头文件在第一阶段Gcc 编译安装时要被 check 到，但这是后面编译安装目标机的glibc时才被生成安装的。
touch ${SYSROOT}/usr/include/gnu/stubs.h
//拷贝建立支持NPTL编译器所必须的头文件
cp --reply=yes ${TOP}/source/glibc-${GLIBC_VERSION}-headers-build/ports/sysdeps/unix/sysv/linux/arm/nptl/bits/pthreadtypes.h ${SYSROOT}/usr/include/bits
```

### 编译安装第一阶段 arm-linux-gcc

```
cd ${TOP}/source //进入源码解压目录
tar -jxf ${TAR}/gcc-${GCC_VERSION}.tar.bz2 //解压gcc源码
//改变源码目录，为了标识这是第一阶段用的gcc源码，目的前面说过了。
mv gcc-${GCC_VERSION} gcc-${GCC_VERSION}-stage1
cd gcc-${GCC_VERSION}-stage1 //进入第一阶段源码目录
//打上支持posix 标准的补丁，新的线程标准NPTL是符合posix标准的。
patch -Np1 -i ${TAR}/gcc-${GCC_VERSION}-posix-1.patch
//打上交叉编译gcc搜索头文件路径的补丁
patch -Np1 -i ${TAR}/gcc-${GCC_VERSION}-cross_search_paths-1.patch
cd ${TOP}/source //进入源码解压目录
mkdir -p BUILD/gcc-${GCC_VERSION}-stage1 //建立编译第一阶段gcc的工作目录
cd BUILD/gcc-${GCC_VERSION}-stage1 //进入编译第一阶段gcc 的工作目录
//配置第一阶段gcc。
${TOP}/source/gcc-${GCC_VERSION}-stage1/configure --prefix=${TARGET_PREFIX} --host=i686-pc-linux-gnu --target=${CLFS_TARGET} --disable-multilib --with-sysroot=${SYSROOT} --disable-nls --disable-shared --enable-languages=c
注意配置选项--enable-languages=c 和--disable-shared！！！因为这一阶段的gcc 制作时，还没有生成其给目标机编译程序时所依赖的动态库，所以要—disable-shared，因此第一阶段的gcc是个静态的arm-linux-gcc。之所以只用了--enable-languages=c，而后面生成第二阶段的arm-linux-gcc却用了--enable-languages=c，c++,是因为我们这一阶段的arm-linux-gcc是为了生成制作第二阶段arm-linux-gcc所依赖的glibc动态函数库而制作，编译glibc只需要arm-linux-gcc,并不需要arm-linux-g++。加上c++也是没错的。
make all-gcc //编译第一阶段gcc
make install-gcc //安装第一阶段gcc
```

### 编译安装交叉编译工具链（第二阶段arm-linux-gcc）所依赖的glibc

```
cd ${TOP}/source //进入源码解压目录
tar -jxf ${TAR}/glibc-${GLIBC_VERSION}.tar.bz2 //解压glibc源码
cd glibc-${GLIBC_VERSION} //进入源码目录
//解压glibc 的cpu体系支持源码包到glibc 头文件安装源码目录，因为glibc 默认是没有ARM cpu支持的，GNU组织专门加入了此源码包来解决此问题。
tar -jxf ${TAR}/glibc-ports-${GLIBC_VERSION}.tar.bz2
//改变cpu 体系支持源码目录为 ports ,因为glibc对所有加入的而外支持包的默认搜索目录为 ports。
mv glibc-ports-${GLIBC_VERSION} ports
//以下四步均为给glibc打补丁，至于是什么补丁，自己到http://cross-lfs.org/view/clfs-sysroot/arm/index.html看！！
patch -Np1 -i ${TAR}/glibc-2.6.1-libgcc_eh-1.patch
patch -Np1 -i ${TAR}/glibc-2.6.1-localedef_segfault-1.patch
patch -Np1 -i ${TAR}/glibc-2.6.1-cross_hacks-1.patch
patch -Np1 -i ${TAR}/glibc-2.6.1-RTLD_SINGLE_THREAD_P-1.patch
mkdir -p ${TOP}/source/BUILD/glibc-${GLIBC_VERSION} //建立配置、编译、安装glibc的工作目录
cd ${TOP}/source/BUILD/glibc-${GLIBC_VERSION} //进入配置、编译、安装glibc的工作目录
//以下三步是为了把glibc头文件配置成支持 NPTL（linux 的新线程标准）而设立的必须条件，并把这些条件写入缓存文件（可以从下面的配置选项 --cache-file=config.cache看出）
echo "libc_cv_forced_unwind=yes" > config.cache
echo "libc_cv_c_cleanup=yes" >> config.cache
echo "libc_cv_arm_tls=yes" >> config.cache
//指定配置使用的编译器为gcc，即本机的gcc
BUILD_CC="gcc"
//指定编译glibc时使用第一阶段的交叉gcc，即上面生成的arm-linux-gcc
CC="${CLFS_TARGET}-gcc"
//指定汇编glibc时使用的汇编器为arm-linux-ar，即在制作binutils 时生成的交叉汇编器
AR="${CLFS_TARGET}-ar"
//这里我也不清楚，从环境变量命名看应该是编译时所用到的库吧！！！
RANLIB="${CLFS_TARGET}-ranlib"
//配置交叉编译工具链所依赖的动态库glibc
${TOP}/source/glibc-${GLIBC_VERSION}/configure --prefix=/usr --libexecdir=/usr/lib/glibc --host=${CLFS_TARGET} --build=i686-pc-linux-gnu --disable-profile --enable-add-ons --with-tls --enable-kernel=2.6.0 --with-__thread --with-binutils=${TARGET_PREFIX}/bin --with-headers=${SYSROOT}/usr/include --cache-file=config.cache
make //编译交叉编译工具链所依赖的动态库glibc
make install install_root=${SYSROOT} //安装交叉编译工具链所依赖的动态库
//以下的步骤是配置交叉编译工具链在使用动态库时所用的时区，语言。这里不多说了，照做就是了！！！
make localedata/install-locales
mkdir -pv ${SYSROOT}/usr/lib/locale
export I18NPATH=${PWD}/localedata
export GCONV_PATH=${PWD}/iconvdata
export LOCALEDEF="${PWD}/locale/localedef-native --alias-file=../intl/locale.alias"
cd ${TOP}/source/glibc-${GLIBC_VERSION}/localedata
${LOCALEDEF} -i locales/de_DE -f charmaps/ISO-8859-1 --prefix=${SYSROOT} de_DE
${LOCALEDEF} -i locales/de_DE@euro -f charmaps/ISO-8859-15 --prefix=${SYSROOT} de_DE@euro
${LOCALEDEF} -i locales/en_HK -f charmaps/ISO-8859-1 --prefix=${SYSROOT} en_HK
${LOCALEDEF} -i locales/en_PH -f charmaps/ISO-8859-1 --prefix=${SYSROOT} en_PH
${LOCALEDEF} -i locales/en_US -f charmaps/ISO-8859-1 --prefix=${SYSROOT} en_US
${LOCALEDEF} -i locales/es_MX -f charmaps/ISO-8859-1 --prefix=${SYSROOT} es_MX
${LOCALEDEF} -i locales/fa_IR -f charmaps/UTF-8 --prefix=${SYSROOT} fa_IR
${LOCALEDEF} -i locales/fr_FR -f charmaps/ISO-8859-1 --prefix=${SYSROOT} fr_FR
${LOCALEDEF} -i locales/fr_FR@euro -f charmaps/ISO-8859-15 --prefix=${SYSROOT} fr_FR@euro
${LOCALEDEF} -i locales/it_IT -f charmaps/ISO-8859-1 --prefix=${SYSROOT} it_IT
${LOCALEDEF} -i locales/ja_JP -f charmaps/EUC-JP --prefix=${SYSROOT} ja_JP
unset I18NPATH GCONV_PATH LOCALEDEF
/************************ 下面是配置动态库的使用环境文件 照做！！！******************/
cat > ${SYSROOT}/etc/nsswitch.conf << "EOF"
# Begin /etc/nsswitch.conf
passwd: files
group: files
shadow: files

hosts: files dns
networks: files

protocols: files
services: files
ethers: files
rpc: files
# End /etc/nsswitch.conf
EOF

TZDIR="${SYSROOT}/usr/share/zoneinfo"
${SYSROOT}/usr/bin/tzselect
cp -v --reply=yes --remove-destination ${SYSROOT}/usr/share/zoneinfo/America/New_York ${SYSROOT}/etc/localtime

cat > ${SYSROOT}/etc/ld.so.conf << "EOF"
# Begin /etc/ld.so.conf

/usr/local/lib
/opt/lib

# End /etc/ld.so.conf
EOF
````


### 第二阶段的gcc 制作。

即制作依赖上一阶段生成的glibc 的arm-linux-gcc等一系列交叉编译工具，统称交叉编译工具链。

```
cd ${TOP}/source //进入源码解压目录
tar -jxf ${TAR}/gcc-${GCC_VERSION}.tar.bz2 //解压gcc源代码

//改名源码目录，为了标识这是编译第二阶段的gcc 所用的源码
mv gcc-${GCC_VERSION} gcc-${GCC_VERSION}-stage2
cd gcc-${GCC_VERSION}-stage2 //进入源码目录
//打上支持posix 标准的补丁，新的线程标准NPTL是符合posix标准的。
patch -Np1 -i ${TAR}/gcc-${GCC_VERSION}-posix-1.patch
//打上交叉编译gcc搜索头文件路径的补丁
patch -Np1 -i ${TAR}/gcc-${GCC_VERSION}-cross_search_paths-1.patch
cd ${TOP}/source //进入源码解压目录
mkdir -p BUILD/gcc-${GCC_VERSION}-stage2 //建立第二阶段gcc的编译工作目录
cd BUILD/gcc-${GCC_VERSION}-stage2 //进入第二阶段gcc的编译工作目录
//配置第二阶段交叉编译工具链

${TOP}/source/gcc-${GCC_VERSION}-stage2/configure --prefix=${TARGET_PREFIX} --host=i686-pc-linux-gnu --target=${CLFS_TARGET} --disable-multilib --with-sysroot=${SYSROOT} --disable-nls --enable-shared --enable-languages=c,c++ --enable-__cxa_atexit --enable-c99 --enable-long-long --enable-threads=posix

make all-gcc //编译第二阶段交叉编译工具链
make install-gcc install_root=${TARGET_PREFIX} //安装交叉编译工具链
```

疑难解答：
----------------------------------------

5.1 第一阶段gcc 跟第二阶段 gcc ，一个是静态一个是动态，那么是在哪里指定了第二阶段gcc是跟动态函数库关联，而且是关联到哪个动态库，怎样关联到我们制作出来的动态库呢？

答：
1、比较两次的配置参数
Gcc stage1：
configure --prefix=${TARGET_PREFIX} --host=i686-pc-linux-gnu --target=${CLFS_TARGET} --disable-multilib --with-sysroot=${SYSROOT} --disable-nls --disable-shared --enable-languages=c
Gcc stage2：
configure --prefix=${TARGET_PREFIX} --host=i686-pc-linux-gnu --target=${CLFS_TARGET} --disable-multilib --with-sysroot=${SYSROOT} --disable-nls --enable-shared --enable-languages=c,c++ --enable-__cxa_atexit --enable-c99 --enable-long-long --enable-threads=posix
2、因为指定了--with-sysroot=${SYSROOT} 那么编译时就会以该目录为系统根目录作为动态共享函数库目录、头文件存放目录的顶层搜索目录。如果--enable-shared 就会在该目录下搜索lib目录、usr/lib目录下的动态共享函数库文件，并在该目录下搜索include 目录、/usr/include目录中的头文件。这样只要我们把我们制作的glibc指定安装到${SYSROOT}，制作出来的交叉编译器就会关联到我们的glibc，并且在以后使用这个交叉编译器编译程序时，编译器就会到${SYSROOT}下关联我们制做的动态函数库，查找用户程序所使用的Linux内核头文件。但如果使用--disable-shared参数，就不会去搜索一切${SYSROOT}下的动态函数库文件夹，来关联动态函数库文件了。