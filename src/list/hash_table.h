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
typedef list_compareCallback_t hashTable_compareCallback_t;
typedef list_destroyCallback_t hashTable_destroyCallback_t;
typedef size_t (*hashTable_hashCallback_t)(const void * const Data);

/*** Type Definition ***/
typedef struct hashTable_hashTable_s
{
    size_t buckets;
    hashTable_hashCallback_t hashCallback; 
    list_list_t *list;
} hashTable_hashTable_t;

/****************************************************************************************************
 * Function Prototype
 ****************************************************************************************************/

extern eclectic_status_t hashTable_destroy(hashTable_hashTable_t * const hashTable);
extern bool hashTable_find(const hashTable_hashTable_t * const HashTable, const void * const Data);
extern eclectic_status_t hashTable_init(hashTable_hashTable_t * const hashTable, const size_t Buckets, const hashTable_compareCallback_t CompareCallback, const hashTable_destroyCallback_t DestroyCallback, hashTable_hashCallback_t HashCallback);
extern eclectic_status_t hashTable_insert(hashTable_hashTable_t * const hashTable, void * const data);
extern eclectic_status_t hashTable_remove(hashTable_hashTable_t * const hashTable, void ** const data);
extern size_t hashTable_size(const hashTable_hashTable_t * const HashTable);
