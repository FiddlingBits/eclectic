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
typedef list_destroyCallback_t stack_destroyCallback_t;

/*** Type Definition ***/
typedef list_list_t stack_stack_t;

/****************************************************************************************************
 * Function Prototype
 ****************************************************************************************************/

extern eclectic_status_t stack_destroy(stack_stack_t * const stack);
extern eclectic_status_t stack_init(stack_stack_t * const stack, const stack_destroyCallback_t DestroyCallback);
extern void *stack_pop(stack_stack_t * const stack);
extern eclectic_status_t stack_push(stack_stack_t * const stack, void * const data);
extern size_t stack_size(const stack_stack_t * const Stack);
