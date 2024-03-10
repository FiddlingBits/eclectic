/****************************************************************************************************
 * Pragma
 ****************************************************************************************************/

#pragma once

/****************************************************************************************************
 * Include
 ****************************************************************************************************/

#include "eclectic.h"
#include "list.h"

/****************************************************************************************************
 * Type Definition
 ****************************************************************************************************/

/*** Function Pointer ***/
typedef list_destroyCallback_t queue_destroyCallback_t;

/*** Type Definition ***/
typedef list_list_t queue_queue_t;

/****************************************************************************************************
 * Function Prototype
 ****************************************************************************************************/

extern void *queue_dequeue(queue_queue_t * const queue);
extern eclectic_status_t queue_destroy(queue_queue_t * const queue);
extern eclectic_status_t queue_enqueue(queue_queue_t * const queue, void * const data);
extern eclectic_status_t queue_init(queue_queue_t * const queue, const queue_destroyCallback_t DestroyCallback);
extern size_t queue_size(const queue_queue_t * const Queue);
