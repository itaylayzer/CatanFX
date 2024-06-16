#ifndef _INC_QUEUE

#define _INC_QUEUE

#include <stdbool.h>

#include "../linked_list/circular/linked_circular_list.h"

typedef linkNode Queue, *QueuePtr;

//----------------------------------------------------------------------------------------
//                                 queue init
//                                 ----------
//
// General      : The functions initialize a queue.
//
// Parameters   :
//          QueuePtr   - Queue Pointer (In/Out)
//
// Return Value : None.
//
//----------------------------------------------------------------------------------------
void queue_init(QueuePtr);

//----------------------------------------------------------------------------------------
//                                 enqueue
//                                 -------
//
// General      : The functions add a Value to the queue.
//
// Parameters   :
//          QueuePtr   - Queue Pointer (In/Out)
//          data       - Value (In)
//
// Return Value : True if the value were added to the queue, otherwise False.
//
//----------------------------------------------------------------------------------------
bool enqueue(QueuePtr, void *data);

//----------------------------------------------------------------------------------------
//                                 queue empty
//                                 -----------
//
// General      : The functions checks if the queue is empty.
//
// Parameters   :
//          Queue      - Queue (In)
//
// Return Value : True if the queue is empty, otherwise False.
//
//----------------------------------------------------------------------------------------
bool queue_empty(Queue);

//----------------------------------------------------------------------------------------
//                                 dequeue
//                                 -------
//
// General      : The functions removes a Value from the queue and returns it.
//
// Parameters   :
//          QueuePtr   - Queue Pointer (In/Out)
//
// Return Value : Value.
//
//----------------------------------------------------------------------------------------
void *dequeue(QueuePtr);

//----------------------------------------------------------------------------------------
//                                 queue destroy
//                                 -------------
//
// General      : The function destroys the queue and frees allocated memory.
//
// Parameters   :
//          QueuePtr   - queue pointer (In/Out)
//
// Return Value : None.
//
//----------------------------------------------------------------------------------------
void queue_destroy(QueuePtr);

#endif