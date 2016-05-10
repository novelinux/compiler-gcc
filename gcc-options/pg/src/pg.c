#include <stdio.h>
#include <stdlib.h>

static void test3(void)
{
    int i;

    for (i = 0; i < 100; ++i) {
        i += 1;
    }
}

static void test2(void)
{
    int i;

    for (i = 0; i < 1000; ++i) {
        i += 1;
    }
}

static void test1(void)
{
    int i;

    for (i = 0; i < 10; ++i) {
        i += 1;
        test2();
    }
}

int main(int argc, char *argv[])
{
    int i;

    for (i = 0; i < 100000; ++i) {
        test1();
        test3();
    }

    return 0;
}
