#ifndef _INC_LINKED_LINEAR_LIST

#define _INC_LINKED_LINEAR_LIST

#include <stdlib.h>

#include "../typedef.h"

// n = 0
void linear_init(linkNode *);
// i = 0
linkNode linear_push(linkNode *);
// i > 0
linkNode linear_insert_after(linkNode);
// n = 0
void *linear_pop(linkNode *);
// n > 0
void *linear_remove_after(linkNode);

#endif