#include <stdio.h>
#include <stdlib.h>
#include "stack.h"

int main()
{
    stack s;
    init(&s);
    printf("%d\n", isEmpty(&s));



    return(EXIT_SUCCESS);
}

void init(stack *s)
{
    s->top = -1;
}

int isEmpty(stack *s)
{
    return(s->top < 0);
}

int top(stack *s)
{
    if (isEmpty(s))
    {
        return(0);
    }
    return(s->items[s->top]); //returns top item of stack
}

int push(stack* s)
{
    return;
}