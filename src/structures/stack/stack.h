#ifndef _INC_STACK

#define _INC_STACK

#include <stdbool.h>

#include "../linked_list/linear/linked_linear_list.h"

typedef linkNode Stack, *StackPtr;

//----------------------------------------------------------------------------------------
//                                 stack_init
//                                 ----------
//
// General      : The function initializes a stack.
//
// Parameters   :
//          StackPtr   - Stack pointer (In/Out)
//
// Return Value : None.
//
//----------------------------------------------------------------------------------------
void stack_init(StackPtr);

//----------------------------------------------------------------------------------------
//                                 stack push
//                                 ----------
//
// General      : The function pushes a value into a stack.
//
// Parameters   :
//          StackPtr   - Stack pointer (In/Out)
//          data       - Value (In)
//
// Return Value : True if the value were added to the stack, otherwise False.
//
//----------------------------------------------------------------------------------------
bool stack_push(StackPtr, void *data);

//----------------------------------------------------------------------------------------
//                                 stack empty
//                                 ----------
//
// General      : The function checks if the stack is empty.
//
// Parameters   :
//          Stack   - Stack (In)
//
// Return Value : True if the stack is empty, otherwise False.
//
//----------------------------------------------------------------------------------------
bool stack_empty(Stack);

//----------------------------------------------------------------------------------------
//                                 stack pop
//                                 ------------
//
// General      : The function removes a Value from the stack and returns it.
//
// Parameters   :
//          StackPtr   - Stack pointer (In/Out)
//
// Return Value : Value.
//
//----------------------------------------------------------------------------------------
void *stack_pop(StackPtr);

//----------------------------------------------------------------------------------------
//                                 stack destroy
//                                 ------------
//
// General      : The function destroys the stack and frees allocated memory.
//
// Parameters   :
//          StackPtr   - Stack pointer (In/Out)
//
// Return Value : None.
//
//----------------------------------------------------------------------------------------
void stack_destroy(StackPtr);

#endif