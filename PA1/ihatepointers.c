#include <stdio.h>
#include <stdlib.h>

void f2(long** num)
{
    **num += 2;
    return;
}

void f1 (long* num)
{
    f2(&num);
    return;
}

void main()
{

    long t = 0;
    printf("%d\n",t);
    f1(&t);
    printf("%d", t);

    return;
}
