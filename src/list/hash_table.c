/****************************************************************************************************
 * Include
 ****************************************************************************************************/

#include "hash_table.h"

/****************************************************************************************************
 * Function Definition (Public)
 ****************************************************************************************************/

/*** Destroy ***/
eclectic_status_t hashTable_destroy(hashTable_hashTable_t * const hashTable)
{
    size_t i;
    
    /*** Error Check ***/
    /* NULL Pointer */
    if(hashTable == NULL)
        return ECLECTIC_STATUS_ERROR_NULL_POINTER;
    
    /*** Destroy ***/
    /* Destroy Table List */
    for(i = 0; i < hashTable->buckets; i++)
        (void)list_destroy(&(hashTable->list[i]));
    
    /* Clean Up */
    eclectic_free((void **)&(hashTable->list));
    
    return ECLECTIC_STATUS_SUCCESS;
}

/*** Find ***/
bool hashTable_find(const hashTable_hashTable_t * const HashTable, const void * const Data)
{
    size_t bucket;
    
    /*** Error Check ***/
    /* NULL Pointer */
    if(HashTable == NULL)
        return false;
    
    /* Not Initialized */
    if(HashTable->hashCallback == NULL)
        return false;
    
    /*** Find ***/
    /* Hash */
    bucket = HashTable->hashCallback(Data) % HashTable->buckets;
    
    /* Find */
    return (list_find(&(HashTable->list[bucket]), Data) != LIST_FIND_NOT_FOUND_INDEX);
}

/*** Initialize ***/
eclectic_status_t hashTable_init(hashTable_hashTable_t * const hashTable, const size_t Buckets, const hashTable_compareCallback_t CompareCallback, const hashTable_destroyCallback_t DestroyCallback, hashTable_hashCallback_t HashCallback)
{
    size_t i;
    
    /*** Error Check ***/
    /* NULL Pointer */
    if((hashTable == NULL) || (CompareCallback == NULL) || (HashCallback == NULL))
        return ECLECTIC_STATUS_ERROR_NULL_POINTER;
    
    /* Length */
    if(Buckets == 0)
        return ECLECTIC_STATUS_ERROR_LENGTH;
    
    /*** Initialize ***/    
    /* Initialize */
    hashTable->buckets = Buckets;
    hashTable->hashCallback = HashCallback;
    if((hashTable->list = eclectic_malloc(Buckets * sizeof(*hashTable->list))) == NULL)
    {
        hashTable->buckets = 0;
        return ECLECTIC_STATUS_ERROR_MEMORY_ALLOCATION;
    }
    for(i = 0; i < hashTable->buckets; i++)
        (void)list_init(&(hashTable->list[i]), CompareCallback, DestroyCallback);
    
    return ECLECTIC_STATUS_SUCCESS;
}

/*** Insert ***/
eclectic_status_t hashTable_insert(hashTable_hashTable_t * const hashTable, void * const data)
{
    size_t bucket, i;
    eclectic_status_t status;
    
    /*** Error Check ***/
    /* NULL Pointer */
    if(hashTable == NULL)
        return ECLECTIC_STATUS_ERROR_NULL_POINTER;
    
    /* Not Initialized */
    if(hashTable->hashCallback == NULL)
        return ECLECTIC_STATUS_ERROR_NOT_INITIALIZED;
    
    /*** Insert ***/
    /* Hash */
    bucket = hashTable->hashCallback(data) % hashTable->buckets;
    
    /* Find */
    if(list_find(&(hashTable->list[bucket]), data) != LIST_FIND_NOT_FOUND_INDEX)
        return ECLECTIC_STATUS_ERROR_DUPLICATE;
    
    /* Insert */
    if((status = list_pushTail(&(hashTable->list[bucket]), data)) != ECLECTIC_STATUS_SUCCESS)
        return status;
    
    return ECLECTIC_STATUS_SUCCESS;
}

/*** Remove ***/
eclectic_status_t hashTable_remove(hashTable_hashTable_t * const hashTable, void ** const data)
{
    size_t bucket, index;
    
    /*** Error Check ***/
    /* NULL Pointer */
    if((hashTable == NULL) || (data == NULL))
        return ECLECTIC_STATUS_ERROR_NULL_POINTER;
    
    /* Not Initialized */
    if(hashTable->hashCallback == NULL)
        return ECLECTIC_STATUS_ERROR_NOT_INITIALIZED;
    
    /*** Remove ***/
    /* Hash */
    bucket = hashTable->hashCallback(data) % hashTable->buckets;
        
    /* Remove */
    if((index = list_find(&(hashTable->list[bucket]), *data)) == LIST_FIND_NOT_FOUND_INDEX)
        return ECLECTIC_STATUS_ERROR_NOT_FOUND;
    *data = list_popAt(&(hashTable->list[bucket]), index);
    
    return ECLECTIC_STATUS_SUCCESS;
}

/*** Size ***/
size_t hashTable_size(const hashTable_hashTable_t * const HashTable)
{
    size_t i, size = 0;
    
    /*** Error Check ***/
    if(HashTable != NULL)
    {
        /*** Size ***/
        for(i = 0; i < HashTable->buckets; i++)
            size += list_size(&(HashTable->list[i]));
    }

    return size;
}
