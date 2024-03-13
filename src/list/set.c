/****************************************************************************************************
 * Include
 ****************************************************************************************************/

#include "set.h"

/****************************************************************************************************
 * Function Definition (Public)
 ****************************************************************************************************/

/*** Destroy ***/
eclectic_status_t set_destroy(set_set_t * const set)
{
    /*** Destroy ***/
    return list_destroy(set);
}

/*** Initialize ***/
eclectic_status_t set_init(set_set_t * const set, const set_compareCallback_t CompareCallback, const set_destroyCallback_t DestroyCallback)
{
    /*** Initialize ***/
    return list_init(set, CompareCallback, DestroyCallback);
}

/*** Insert ***/
eclectic_status_t set_insert(set_set_t * const set, void *data)
{
    /*** Insert ***/
    if(list_find(set, data) == LIST_FIND_NOT_FOUND_INDEX)
        return list_pushSorted(set, data, false);
    else
        return ECLECTIC_STATUS_ERROR_DUPLICATE;
}

/*** Peek ***/
void *set_peek(const set_set_t * const Set)
{
    /*** Peek ***/
    return list_peekAt(Set, 0);
}

/*** Remove ***/
void *set_remove(set_set_t * const set)
{
    /*** Remove ***/
    return list_popHead(set);
}

/*** Size ***/
size_t set_size(const set_set_t * const Set)
{
   /*** Size ***/
   return list_size(Set);
}
