#ifndef _INC_LINKED_CIRCUAL_LIST

#define _INC_LINKED_CIRCUAL_LIST

#include <stdlib.h>

#include "../typedef.h"

// n = 0
void circular_init(linkNode *);
// n > 0
linkNode circular_insert_after(linkNode);
// n > 0
linkNode circular_insert_end(linkNode *);
// n > 1
void *circular_remove_end(linkNode *);
// n > 1
void *circular_remove_after(linkNode *);
// n = 1
void *circular_remove_last(linkNode *);

#endif