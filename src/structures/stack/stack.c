#include "stack.h"

void stack_init(StackPtr pstk)
{
    linear_init(pstk);
}

void stack_push(StackPtr pstk, void *data)
{
    linear_push(pstk)->data = data;
}

bool stack_empty(Stack stk)
{
    return stk == NULL;
}

void *stack_pop(StackPtr stk)
{
    return linear_pop(stk);
}

void stack_destroy(StackPtr stk)
{
    while (!stack_empty(*stk))
    {
        stack_pop(stk);
    }
}