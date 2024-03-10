/****************************************************************************************************
 * Include
 ****************************************************************************************************/

#include "stack.h"

/****************************************************************************************************
 * Function Definition (Public)
 ****************************************************************************************************/

/*** Destroy ***/
eclectic_status_t stack_destroy(stack_stack_t * const stack)
{
    /*** Destroy ***/
    return list_destroy(stack);
}

/*** Initialize ***/
eclectic_status_t stack_init(stack_stack_t * const stack, const stack_destroyCallback_t DestroyCallback)
{
    /*** Initialize ***/
    return list_init(stack, NULL, DestroyCallback);
}

/*** Pop ***/
void *stack_pop(stack_stack_t * const stack)
{
    /*** Pop ***/
    return list_popHead(stack);
}

/*** Push ***/
eclectic_status_t stack_push(stack_stack_t * const stack, void * const data)
{
    /*** Push ***/
    return list_pushHead(stack, data);
}

/*** Size ***/
size_t stack_size(const stack_stack_t * const Stack)
{
   /*** Size ***/
   return list_size(Stack);
}
