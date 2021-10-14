#ifndef __stack__
#define __stack__

#define STACKSIZE 100

typedef struct _stack
{
    int top;
    int items[STACKSIZE];
} stack;

void init(stack*);
int isEmpty(stack*);
int top(stack*);
int pop(stack*);
int push(stack*);

#endif