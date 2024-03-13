/****************************************************************************************************
 * Include
 ****************************************************************************************************/

#include "list.h"

/****************************************************************************************************
 * Function Prototype
 ****************************************************************************************************/

static bool list_pushSortedCompare(const list_list_t * const List, const void * const Data1, const void * const Data2, const bool Descending);
static list_node_t *list_createNode(void * const data);

/****************************************************************************************************
 * Function Definition (Public)
 ****************************************************************************************************/

/*** Destroy ***/
eclectic_status_t list_destroy(list_list_t * const list)
{
    void *data;
    
    /*** Error Check ***/
    /* NULL Pointer */
    if(list == NULL)
        return ECLECTIC_STATUS_ERROR_NULL_POINTER;
    
    /*** Destroy ***/
    /* Pop Until Empty */
    while(list->size > 0)
    {
        data = list_popHead(list);
        if(list->destroyCallback != NULL)
            list->destroyCallback(data);
    }
    
    return ECLECTIC_STATUS_SUCCESS;
}

/*** Find ***/
size_t list_find(const list_list_t * const List, const void * const Data)
{
    size_t foundIndex = LIST_FIND_NOT_FOUND_INDEX, i;
    const list_node_t *Node;
    
    /*** Find ***/
    /* Error Check */
    if((List != NULL) && (List->compareCallback != NULL))
    {
        /* Find */
        Node = List->head;
        for(i = 0; i < list_size(List); i++)
        {
            /* Compare */
            if(List->compareCallback(Data, Node->data) == 0)
            {
                foundIndex = i;
                break;
            }
            
            /* Go To Next Node */
            Node = Node->next;
        }
    }
    
    return foundIndex;
}

/*** Initialize ***/
eclectic_status_t list_init(list_list_t * const list, const list_compareCallback_t CompareCallback, const list_destroyCallback_t DestroyCallback)
{    
    /*** Error Check ***/
    /* NULL Pointer */
    if(list == NULL)
        return ECLECTIC_STATUS_ERROR_NULL_POINTER;
    
    /*** Initialize ***/
    list->compareCallback = CompareCallback;
    list->destroyCallback = DestroyCallback;
    list->head = list->tail = NULL;
    list->size = 0;
    
    return ECLECTIC_STATUS_SUCCESS;
}

/*** Peek At ***/
void *list_peekAt(const list_list_t * const List, const size_t Index)
{
    void *data = NULL;
    size_t i;
    const list_node_t *Node = NULL;
    
    /*** Peek At ***/
    /* Error Check */
    if((List != NULL) && (Index < List->size))
    {
        /* Peek At */
        Node = List->head;
        for(i = 0; i < Index; i++)
            Node = Node->next;
        data = Node->data;
    }
    
    return data;
}

/*** Pop At ***/
void *list_popAt(list_list_t * const list, const size_t Index)
{
    void *data = NULL;
    size_t i;
    const list_node_t *Node = NULL;
    
    /*** Pop At ***/
    /* Error Check */
    if((list != NULL) && (Index < list->size))
    { 
        /* Adjust List */
        if(Index == 0)
        {
            /* Node At Head */
            data = list_popHead(list);
        }
        else if(Index == (list->size - 1))
        {
            /* Node At Tail */
            data = list_popTail(list);
        }
        else
        { 
            /* Find */
            Node = list->head;
            for(i = 0; i < Index; i++)
                Node = Node->next;
            data = Node->data;
            
            /* Repair Hole */
            Node->prev->next = Node->next;
            Node->next->prev = Node->prev;
            
            /* Decrement Size */
            list->size--;
            
            /* Clean Up */
            eclectic_free((void **)&Node);
        }
    }
    
    return data;
}

/*** Pop Head ***/
void *list_popHead(list_list_t * const list)
{
    void *data = NULL;
    list_node_t *oldHead;
    
    /*** Pop Head ***/
    /* Error Check */
    if((list != NULL) && (list->size > 0))
    {
        /* Pop Head */
        oldHead = list->head;
        data = oldHead->data;
        
        /* Decrement Size */
        list->size--;
        
        /* Adjust List */
        if(list->size == 0)
        {
            /* List Is Empty */
            list->head = list->tail = NULL;
        }
        else
        {
            /* New Head Is Next Node */
            list->head = list->head->next;
            list->head->prev = NULL;
        }
        
        /* Clean Up */
        eclectic_free((void **)&oldHead);
    }
    
    return data;
}

/*** Pop Tail ***/
void *list_popTail(list_list_t * const list)
{
    void *data = NULL;
    list_node_t *oldTail;
    
    /*** Pop Tail ***/
    /* Error Check */
    if((list != NULL) && (list->size > 0))
    {
        /* Pop Tail*/
        oldTail = list->tail;
        data = oldTail->data;
        
        /* Decrement Size */
        list->size--;
        
        /* Adjust List */
        if(list->size == 0)
        {
            /* List Is Empty */
            list->head = list->tail = NULL;
        }
        else
        {
            /* New Tail Is Previous Node */
            list->tail = list->tail->prev;
            list->tail->next = NULL;
        }
        
        /* Clean Up */
        eclectic_free((void **)&oldTail);
    }
    
    return data;
}

/*** Push At ***/
eclectic_status_t list_pushAt(list_list_t * const list, void * const data, const size_t Index)
{
    size_t i;
    list_node_t *newNode, *oldNode;
    
    /*** Error Check ***/
    /* NULL Pointer */
    if(list == NULL)
        return ECLECTIC_STATUS_ERROR_NULL_POINTER;
    
    /* Length */
    if(Index >= list->size)
        return ECLECTIC_STATUS_ERROR_INVALID;
    
    /*** Push At ***/
    if(Index == 0)
    {
        /* Node At Head */
        return list_pushHead(list, data);
    }
    else
    {
        /* Memory Allocation */
        if((newNode = list_createNode(data)) == NULL)
            return ECLECTIC_STATUS_ERROR_MEMORY_ALLOCATION;
        
        /* Find */
        oldNode = list->head;
        for(i = 0; i < Index; i++)
            oldNode = oldNode->next;
        
        /* Adjust */
        oldNode->prev->next = newNode;
        newNode->prev = oldNode->prev;
        newNode->next = oldNode;
        oldNode->prev = newNode;
        
        /* Increment Size */
        list->size++;
    }
    
    return ECLECTIC_STATUS_SUCCESS;
}

/*** Push Head ***/
eclectic_status_t list_pushHead(list_list_t * const list, void * const data)
{
    list_node_t *node;
    
    /*** Error Check ***/
    /* NULL Pointer */
    if(list == NULL)
        return ECLECTIC_STATUS_ERROR_NULL_POINTER;
    
    /* Memory Allocation */
    if((node = list_createNode(data)) == NULL)
        return ECLECTIC_STATUS_ERROR_MEMORY_ALLOCATION;
    
    /*** Push Head ***/
    /* Push Head */
    if(list->size == 0)
    {
        /* New Node Is Both Head And Tail */
        list->head = list->tail = node;
    }
    else
    {
        /* New Node Is New Head */
        node->next = list->head; 
        list->head->prev = node;
        list->head = node;
    }
    
    /* Increment Size */
    list->size++;
    
    return ECLECTIC_STATUS_SUCCESS;
}

/*** Push Sorted ***/
eclectic_status_t list_pushSorted(list_list_t * const list, void * const data, const bool Descending)
{
    size_t i;
    list_node_t *newNode, *oldNode;
    
    /*** Error Check ***/
    /* NULL Pointer */
    if(list == NULL)
        return ECLECTIC_STATUS_ERROR_NULL_POINTER;
    
    /* Not Initialized */
    if(list->compareCallback == NULL)
        return ECLECTIC_STATUS_ERROR_NOT_INITIALIZED;
    
    /*** Push Sorted ***/
    if((list->size == 0) || list_pushSortedCompare(list, data, list->head->data, Descending))
    {
        return list_pushHead(list, data);
    }
    else
    {    
        /* Find */
        oldNode = list->head;
        for(i = 0; i < list->size; i++)
        {
            if(list_pushSortedCompare(list, data, oldNode->data, Descending))
                break;
            else
                oldNode = oldNode->next;
        }
        
        /* Adjust */
        if(oldNode == NULL)
        {
            return list_pushTail(list, data);
        }
        else
        {            
            /* Memory Allocation */
            if((newNode = list_createNode(data)) == NULL)
                return ECLECTIC_STATUS_ERROR_MEMORY_ALLOCATION;
            
            /* Adjust */
            newNode->prev = oldNode->prev;
            newNode->prev->next = newNode;
            newNode->next = oldNode;
            oldNode->prev = newNode;
            
            /* Increment Size */
            list->size++;
        }
    }
    
    return ECLECTIC_STATUS_SUCCESS;
}

/*** Push Tail ***/
eclectic_status_t list_pushTail(list_list_t * const list, void * const data)
{
    list_node_t *node;
    
    /*** Error Check ***/
    /* NULL Pointer */
    if(list == NULL)
        return ECLECTIC_STATUS_ERROR_NULL_POINTER;
    
    /* Memory Allocation */
    if((node = list_createNode(data)) == NULL)
        return ECLECTIC_STATUS_ERROR_MEMORY_ALLOCATION;
    
    /*** Push Tail ***/
    /* Push Tail */
    if(list->size == 0)
    {
        /* New Node Is Both Head And Tail */
        list->head = list->tail = node;
    }
    else
    {
        /* New Node Is New Tail */
        list->tail->next = node;
        node->prev = list->tail;
        list->tail = node;
    }
    
    /* Increment Size */
    list->size++;
    
    return ECLECTIC_STATUS_SUCCESS;
}

/*** Size***/
size_t list_size(const list_list_t * const List)
{
    size_t size = 0;
    
    /*** Size ***/
    /* Error Check */
    if(List != NULL)
        size = List->size;
    
    return size;
}

/****************************************************************************************************
 * Function Definition (Private)
 ****************************************************************************************************/

/*** Push Sorted Compare ***/
bool list_pushSortedCompare(const list_list_t * const List, const void * const Data1, const void * const Data2, const bool Descending)
{
    /*** Compare ***/
    if(Descending)
        return (List->compareCallback(Data1, Data2) > 0);
    else
        return (List->compareCallback(Data1, Data2) < 0);
}

/*** Create Node ***/
static list_node_t *list_createNode(void * const data)
{
    list_node_t *node;
    
    /*** Create Node ***/
    /* Allocate Memory */
    node = eclectic_malloc(sizeof(*node));
    
    /* Error Check */
    if(node != NULL)
    {
        /* Initialize */
        node->data = data;
        node->next = node->prev = NULL;
    }
    
    return node;
}
