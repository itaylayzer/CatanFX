#ifndef _INC_HEAP

#define _INC_HEAP

#include <stdlib.h>
#include <stdbool.h>

typedef struct heapInfo
{
    void *data;
    signed char score;
} heapInfo;

typedef struct heapType
{
    struct heapInfo *data;
    unsigned char size;
    unsigned char length;
} Heap;

//----------------------------------------------------------------------------------------
//                                 heap init
//                                 ---------
//
// General      : The functions initialize a heap.
//
// Parameters   :
//          Heap*           - Heap Pointer (In/Out)
//          unsigned char   - the maximum size of the heap (In)
//
// Return Value : None.
//
//----------------------------------------------------------------------------------------
void heap_init(Heap *, unsigned char);

//----------------------------------------------------------------------------------------
//                                 heap insert
//                                 -----------
//
// General      : The functions insert a value into a heap
//
// Parameters   :
//          Heap*      - Heap Pointer (In/Out)
//          data       - Value (In)
//          score      - The Score of the Value (In)
//          cmp        - Compare Function (In)
//
// Return Value : True if the value were added to the heap, otherwise False.
//
//----------------------------------------------------------------------------------------
bool heap_insert(Heap *, void *data, signed char score,
                 signed char (*cmp)(signed char, signed char));

//----------------------------------------------------------------------------------------
//                                 heap extract
//                                 ------------
//
// General      : The functions removes a value from a heap and returns it.
//
// Parameters   :
//          Heap*      - Heap Pointer (In/Out)
//          cmp        - Compare Function (In)
//          scoreptr   - Number Pointer (Out)
//
// Return Value : Value.
//
//----------------------------------------------------------------------------------------
void *heap_extract(Heap *,
                   signed char (*cmp)(signed char, signed char),
                   signed char *scoreptr);

//----------------------------------------------------------------------------------------
//                                 heap top
//                                 --------
//
// General      : The functions return the top value of the heap.
//
// Parameters   :
//          Heap*      - Heap Pointer (In)
//
// Return Value : the top Value of the heap.
//
//----------------------------------------------------------------------------------------
void *heap_top(Heap *);

//----------------------------------------------------------------------------------------
//                                 heap top score
//                                 --------------
//
// General      : The functions return the top value score of the heap.
//
// Parameters   :
//          Heap*      - Heap Pointer (In)
//
// Return Value : the top value Score of the heap.
//
//----------------------------------------------------------------------------------------
signed char heap_top_score(Heap *);

//----------------------------------------------------------------------------------------
//                                 heap empty
//                                 ----------
//
// General      : The function checks if the heap is empty
//
// Parameters   :
//          Heap       - Heap (In)
//
// Return Value : True if the heap is empty, otherwise False.
//
//----------------------------------------------------------------------------------------
bool heap_empty(Heap);

//----------------------------------------------------------------------------------------
//                                 heap destroy
//                                 ------------
//
// General      : The function destroys the heap and frees allocated memory.
//
// Parameters   :
//          Heap*      - Heap Pointer (In/Out)
//
// Return Value : None.
//
//----------------------------------------------------------------------------------------
void heap_destroy(Heap *);

//----------------------------------------------------------------------------------------
//                                 Minimum Heap Compare
//                                 --------------------
//
// General      : Compare function: Minimum Heap
//
// Parameters   :
//          signed char - First Score (In)
//          signed char - Second Score (In)
//
// Return Value : Compre Value of the scores.
//
//----------------------------------------------------------------------------------------
signed char heap_min(signed char, signed char);

//----------------------------------------------------------------------------------------
//                                 Maximum Heap Compare
//                                 --------------------
//
// General      : Compare function: Maximum Heap
//
// Parameters   :
//          signed char - First Score (In)
//          signed char - Second Score (In)
//
// Return Value : Compre Value of the scores.
//
//----------------------------------------------------------------------------------------
signed char heap_max(signed char, signed char);
#endif