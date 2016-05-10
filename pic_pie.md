gcc/g++中的pic与pie
========================================

PIE这个安全机制从4.1引入，但是Android L之前的系统版本并不会去检验可执行文件是否基于PIE编译出的。
因此不会报错。但是Android L已经开启验证，如果调用的可执行文件不是基于PIE方式编译的，则无法运行。
PIE是啥东西呀，我只知道以前编译共享库（动态库）时需要使用PIC的呀，所以特意查了一下文档，
记录一下以备以后不时之需

PIE Position-Independent-Executable

用来创建介于共享库和通常可执行代码之间的代码–能像共享库一样可重分配地址的程序
有 -fpie, -fPIE, -pie三个参数-fpie与-fPIE 效果一样，用于编译-pie 用于链接
但要生成PIE程序，必须两者配合使用，即使用-fpie -pie或-fPIE -pie

PIC——Position-Independent-Code用来创建共享库时需要用到的参数有-fpic, -fPIC两个参数，都用于编译.
-fPIC 各平台一般都支持, -fpic 在一些特定的平台下会有一些优化

以下内容选自man手册

```
-pie
    Produce a position independent executable on targets that support it.  For predictable
    results, you must also specify the same set of options used for compilation (-fpie,
    -fPIE, or model suboptions) when you specify this linker option.

-fpic
    Generate position-independent code (PIC) suitable for use in a shared library, if
    supported for the target machine.  Such code accesses all constant addresses through a
    global offset table (GOT).  The dynamic loader resolves the GOT entries when the program
    starts (the dynamic loader is not part of GCC; it is part of the operating system).  If
    the GOT size for the linked executable exceeds a machine-specific maximum size, you get
    an error message from the linker indicating that -fpic does not work; in that case,
    recompile with -fPIC instead.  (These maximums are 8k on the SPARC and 32k on the m68k
    and RS/6000.  The 386 has no such limit.)

    Position-independent code requires special support, and therefore works only on certain
    machines.  For the 386, GCC supports PIC for System V but not for the Sun 386i.  Code
    generated for the IBM RS/6000 is always position-independent.

    When this flag is set, the macros "__pic__" and "__PIC__" are defined to 1.

-fPIC
    If supported for the target machine, emit position-independent code, suitable for dynamic
    linking and avoiding any limit on the size of the global offset table.  This option makes
    a difference on the m68k, PowerPC and SPARC.

    Position-independent code requires special support, and therefore works only on certain
    machines.

    When this flag is set, the macros "__pic__" and "__PIC__" are defined to 2.

-fpie
-fPIE
    These options are similar to -fpic and -fPIC, but generated position independent code can
    be only linked into executables.  Usually these options are used when -pie GCC option is
    used during linking.

    -fpie and -fPIE both define the macros "__pie__" and "__PIE__".  The macros have the
    value 1 for -fpie and 2 for -fPIE.
```

PIE与none-PIE的程序可通过file来识别

```
$ g++ -o test_non_pie test.cpp
$ g++ -o test_pie -fpie -pie test.cpp
$ file test_pie
test_pie: ELF 64-bit LSB  shared object, x86-64, version 1 (SYSV), dynamically linked (uses shared libs), for GNU/Linux 2.6.24, BuildID[sha1]=d83c8ed281e84b8ed73f545c530dcce0d1eedf39, not stripped
$ file test_non_pie
test_non_pie: ELF 64-bit LSB  executable, x86-64, version 1 (SYSV), dynamically linked (uses shared libs), for GNU/Linux 2.6.24, BuildID[sha1]=4112d35f407b9f99b335e6e4a1b6a36bc52d679d, not stripped
```

注意上面的shared object与executable的微小区别
