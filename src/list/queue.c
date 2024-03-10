/****************************************************************************************************
 * Include
 ****************************************************************************************************/

#include "queue.h"

/****************************************************************************************************
 * Function Definition (Public)
 ****************************************************************************************************/

/*** Dequeue ***/
void *queue_dequeue(queue_queue_t * const queue)
{
    /*** Dequeue ***/
    return list_popHead(queue);
}

/*** Destroy ***/
eclectic_status_t queue_destroy(queue_queue_t * const queue)
{
    /*** Destroy ***/
    return list_destroy(queue);
}

/*** Enqueue ***/
eclectic_status_t queue_enqueue(queue_queue_t * const queue, void * const data)
{
    /*** Enqueue ***/
    return list_pushTail(queue, data);
}

/*** Initialize ***/
eclectic_status_t queue_init(queue_queue_t * const queue, const queue_destroyCallback_t DestroyCallback)
{
    /*** Initialize ***/
    return list_init(queue, NULL, DestroyCallback);
}

/*** Size ***/
size_t queue_size(const queue_queue_t * const Queue)
{
   /*** Size ***/
   return list_size(Queue);
}
