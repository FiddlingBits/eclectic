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
typedef list_compareCallback_t set_compareCallback_t;
typedef list_destroyCallback_t set_destroyCallback_t;

/*** Type Definition ***/
typedef list_list_t set_set_t;

/****************************************************************************************************
 * Function Prototype
 ****************************************************************************************************/

extern eclectic_status_t set_destroy(set_set_t * const set);
extern eclectic_status_t set_init(set_set_t * const set, const set_compareCallback_t CompareCallback, const set_destroyCallback_t DestroyCallback);
extern eclectic_status_t set_insert(set_set_t * const set, void *data);
extern void *set_peek(const set_set_t * const Set);
extern void *set_remove(set_set_t * const set);
extern size_t set_size(const set_set_t * const Set);
