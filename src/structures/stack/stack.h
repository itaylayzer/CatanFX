#ifndef _INC_STACK

#define _INC_STACK

#include <stdbool.h>

#include "../linked_list/linear/linked_linear_list.h"

typedef linkNode Stack, *StackPtr;

void stack_init(StackPtr);

void stack_push(StackPtr, void *data);

bool stack_empty(Stack);

void *stack_pop(StackPtr);

void stack_destroy(StackPtr);

#endif