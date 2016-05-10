Summary
========================================

Build and verify Android with the Linaro Toolchain to comply with the specification Platform/Android/Specs/LinaroAndroidToolchain

Prerequisites
----------------------------------------

GCC requires that various tools and packages be available for use in the build procedure:

    GNU Multiple Precision Library (GMP)
    MPFR Library version
    MPC Library
    ISL (Integer Set Library)
    CLooG 0.18.0

However, you can ignore the above dependencies by using Linaro Toolchain Builder which builds these libraries from scratch and links into gcc statically.

Prebuilt Toolchain Binaries
----------------------------------------

Prebuilt toolchains for various GCC versions and Linaro milestone releases are provided at https://android-build.linaro.org/ , builds titled ~linaro-android/toolchain-*. Previews of upcoming releases can be found in builds titled ~linaro-android/toolchain-4.8-bzr and ~linaro-android/toolchain-4.7-bzr

To build Android Platform with Linaro Toolchain:

    If you are using Linaro Android, the toolchain is already included. Use

```
    $ make TARGET_TOOLS_PREFIX=`pwd`/prebuilts/gcc/linux-x86/arm/arm-linux-androideabi-4.8-linaro/bin/arm-linux-androideabi-
```

    or

```
    $ make TARGET_TOOLS_PREFIX=`pwd`/prebuilts/gcc/linux-x86/arm/arm-linux-androideabi-4.7-linaro/bin/arm-linux-androideabi-
```

    If you're using stock AOSP or another Android fork that doesn't include the Linaro toolchain:
        Extract toolchain tarball to some directory (/tmp/android-toolchain-eabi is assumed below)
        Change directory to Android toplevel directory and build:

```
$ make TARGET_TOOLS_PREFIX=/tmp/android-toolchain-eabi/bin/arm-linux-androideabi-
```

Use Linaro Toolchain Builder

    Fetch the source code

```
$ mkdir -p linaro-toolchain && cd linaro-toolchain
$ repo init -b linaro-master -u git://android.git.linaro.org/toolchain/manifest.git
```

    Once setup is complete, fetch projects by 'repo' (Use -jX to parallel fetch)

```
$ repo sync <-jX>
```

    Check the options provided by build script

```
$ cd linaro-toolchain/build
$ ./linaro-build.sh --help
```
Usage: linaro-build.sh [options]

Valid options (defaults are in brackets)
  --prefix=<path>             Specify installation path [/tmp/android-toolchain-eabi]
  --toolchain-src=<path>      Toolchain source directory [/home/jserv/Linaro/android/toolchain]
  --with-gcc=<path>           Specify GCC source (support: directory, bzr, url)
  --with-gdb=<path>           Specify gdb source (support: directory, bzr, url)
  --with-sysroot=<path>       Specify SYSROOT directory
  --apply-gcc-patch=<yes|no>  Apply Linaro's extra gcc-patches [no]
  --help                      Print this help message

    Configure and build toolchain

```
$ ./linaro-build.sh --with-gcc=gcc-linaro-4.8-2013.09
Use 64-bit Build Enviorment
checking build system type... x86_64-pc-linux-gnu
checking host system type... x86_64-pc-linux-gnu
checking target system type... arm-linux-androideabi
checking for Canadian cross... no
checking target binutils version to build... current
checking target gold binutils version to build... current
checking target gcc version to build... linaro-4.8-2013.09
checking gmp version... 5.0.2
checking mpfr version... 3.1.0
...
```

Here "--with-gcc=" option can be assigned to the following:

    local directory, such as gcc-linaro-4.8-2013.09
        NOTE: the local directory should inside directory ../gcc/ .

    bzr, such as lp:gcc-linaro/4.7

    URL, such as http://launchpad.net/gcc-linaro/4.8/4.8-2013.09/+download/gcc-linaro-4.8-2013.09.tar.xz

Similarily, you can use "--with-gdb=" to specify a different gdb. The default is a current build of the linaro gdb branch.

Situation with toolchain sysroot

It is possible to build toolchain with sysroot, but that is a special-purpose option. Here are short description of 4 types of toolchains available in Android world:

    arm-linux-androideabi toolchain. This is the main toolchain used for current builds. It differs from the bare-metal toolchain (see below) in that the target platform is arm-linux-androideabi, not arm-eabi.
    Bare-metal toolchain. In Android 2.x releases, this was used to build entire (monolithic) platform. This toolchain is built as decsribed above, without --with-sysroot switch. This is all that was needed to build platform, because libc, libm, etc. are built as part of the platform, and build takes care to pass explicit references to them to build other packages (it uses -nostdlib -libc, etc.). In newer releases, the arm-linux-androideabi toolchain is used instead. The bare-metal toolchain is still useful to build bits and pieces that are loaded before the OS is up, such as u-boot.
    Toolchain built using these instructions and --with-sysroot . Such toolchain is not needed during normal Android development process, which consists of building monolithic platform once, and then build (Java/Dalvik-based) applications for it using SDK/NDK. However, sometimes you need to build additional software which runs on system, platform level. One example is building busybox outside of normal platform build. Another is building official Android toolchain benchmark suite. For such cases, after you build monolothic platform, you can build a sysrooted toolchain for it. Note that you don't need to rebuild platform with this sysrooted toolchain - as described above, just bare-metal compiler needed to build platform, toolchain's libc is explicitly ignored during platform build using -nostdlib switch.
    NDK toolchain. This is used for Android application development. This is completely different layer from system, platform layer - all Android application are Dalvik managed once, any native-CPU code goes in form of JNI shared libraries. NDK toolchain is intended to build just such JNI shared libs. NDK toolchain is build separately from the platform toolchain, with other set of scripts (See "make ndk" in platform tree).

Planned improvements

    GCC multilib setting
        Default: arm, fpu and thumb
        The prebuilt google toolchain use: armv5te and mandroid.
    HardFP-ABI Support for Android.

Further improvements

    Enable LTO (Link Time Optimization, introduced since gcc-4.5) in Android TARGET_GLOBAL_CFLAGS
    Verify the functionality of FDO (Feedback Directed Optimization) by Google Compiler Team and introduce the approaches to integrate.
    Enable automated loop parallelization in parts of the OS

Reference Benchmark

Unofficial benchmark and un-reproducible results:

    Platform/Android/AndroidToolchainBenchmarking
