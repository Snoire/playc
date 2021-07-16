#include <stdio.h>

typedef struct {
    int a;
    int b;
#define C 0x1           // 为什么要这么写？和写在外面没区别呀
    int d;
} s;
/* */

int main()
{
    printf("C: %x, sizeof(s): %zu\n", C, sizeof(s));
    return 0;
}
