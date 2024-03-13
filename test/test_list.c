/****************************************************************************************************
 * Define
 ****************************************************************************************************/

#define LIST_TEST_LIST_COUNT (5)

/****************************************************************************************************
 * Include
 ****************************************************************************************************/

#include "list.h"
#include "mock_eclectic.h"
#include <string.h>
#include <time.h>
#include "unity.h"

/****************************************************************************************************
 * Helper
 ****************************************************************************************************/

/*** Compare Callback ***/
int listTest_compareCallback(const void * const Data1, const void * const Data2)
{    
    /*** Compare Callback ***/
    return *(const int *)Data1 - *(const int *)Data2;
}

void test_compareCallback_success(void)
{
    /*** Test Data ***/
    /* Type Definition */
    typedef struct testData_s
    {
        const int Integer1;
        const int Integer2;
        const int ExpectedResult;
    } testData_t;

    /* Test Data */
    const testData_t TestData[] =
    {
        /* Integer1 Smaller Than Integer2 */
        {-2, -1, -1},
        {-2, 1, -3},
        {1, 2, -1},
        
        /* Integer1 Equal To Integer2 */
        {-2, -2, 0},
        {0, 0, 0},
        {1, 1, 0},
        
        /* Integer1 Larger Than Integer2 */
        {2, 1, 1},
        {1, -2, 3},
        {-1, -2, 1},
    };
    const size_t TestDataCount = sizeof(TestData) / sizeof(TestData[0]);

    /* Variable */
    int actualResult;
    uint8_t i;
    
    /*** Compare Callback ***/
    for(i = 0; i < TestDataCount; i++)
    {
        actualResult = listTest_compareCallback(&TestData[i].Integer1, &TestData[i].Integer2);
        TEST_ASSERT_EQUAL_INT(TestData[i].ExpectedResult, actualResult);
    }
}

/****************************************************************************************************
 * Set Up/Tear Down
 ****************************************************************************************************/

void setUp(void)
{
    srand(time(NULL));
}

void tearDown(void)
{
}

/****************************************************************************************************
 * Test (Public)
 ****************************************************************************************************/

/*** Destroy ***/
void test_destroy_error(void)
{
    /*** Destroy ***/
    /* Destroy (NULL Pointer Error) */
    TEST_ASSERT_EQUAL_INT(ECLECTIC_STATUS_ERROR_NULL_POINTER, list_destroy(NULL));
}

void test_destroy_success(void)
{
    /*** Test Data ***/
    /* Test Data */
    const int TestData[] = {24, 63, -59, 64, -85};
    const size_t TestDataCount = sizeof(TestData) / sizeof(TestData[0]);

    /* Variable */
    int *data;
    uint8_t i;
    list_list_t list;
    
    /*** Destroy ***/
    /* Insert */
    TEST_ASSERT_EQUAL_INT(ECLECTIC_STATUS_SUCCESS, list_init(&list, NULL, free));
    
    /* Push Tail */
    for(i = 0; i < TestDataCount; i++)
    {
        /* Set Up */
        data = malloc(sizeof(*data));
        *data = TestData[i];
        eclectic_malloc_ExpectAndReturn(sizeof(list_node_t), malloc(sizeof(list_node_t))); // Mock
        
        /* Push Tail */
        TEST_ASSERT_EQUAL_INT(ECLECTIC_STATUS_SUCCESS, list_pushTail(&list, data));
        
        /* Size */
        TEST_ASSERT_EQUAL_UINT((i + 1), list_size(&list));
    }
    
    /* Destroy */
    for(i = 0; i < TestDataCount; i++)
        eclectic_free_ExpectAnyArgs(); // Mock
    TEST_ASSERT_EQUAL_INT(ECLECTIC_STATUS_SUCCESS, list_destroy(&list));
    
    /* Verify (Same State As When init'd) */
    TEST_ASSERT_EQUAL_PTR(NULL, list.compareCallback);
    TEST_ASSERT_EQUAL_PTR(free, list.destroyCallback);
    TEST_ASSERT_NULL(list.head);
    TEST_ASSERT_NULL(list.tail);
    TEST_ASSERT_EQUAL_UINT(0, list.size);
}

/*** Find ***/
void test_find_error(void)
{
    /*** Test Data ***/
    /* Variable */
    void *data;
    list_list_t list;
    
    /*** Find ***/
    /* Initialize */
    TEST_ASSERT_EQUAL_INT(ECLECTIC_STATUS_SUCCESS, list_init(&list, NULL, NULL));
    
    /* Find (NULL Pointer Error) */
    TEST_ASSERT_EQUAL_UINT(LIST_FIND_NOT_FOUND_INDEX, list_find(NULL, data));
    TEST_ASSERT_EQUAL_UINT(LIST_FIND_NOT_FOUND_INDEX, list_find(&list, data)); // list.compareCallback Is NULL
}

void test_find_success(void)
{
    /*** Test Data ***/    
    /* Type Definition */
    typedef struct testData_s
    {
        const int FindInteger;
        const size_t ExpectedResult;
    } testData_t;

    /* Test Data */
    const int PushIntegerList[] = {24, 63, -59, 64, -85};
    const size_t PushIntegerListCount = sizeof(PushIntegerList) / sizeof(PushIntegerList[0]);
    const testData_t TestData[] =
    {
        {24, 0},
        {-85, 4},
        {-24, LIST_FIND_NOT_FOUND_INDEX},
        {58, LIST_FIND_NOT_FOUND_INDEX},
        {80, LIST_FIND_NOT_FOUND_INDEX},
        {64, 3},
        {63, 1},
        {51, LIST_FIND_NOT_FOUND_INDEX},
        {-59, 2},
        {16, LIST_FIND_NOT_FOUND_INDEX}
    };
    const size_t TestDataCount = sizeof(TestData) / sizeof(TestData[0]);

    /* Variable */
    uint8_t i;
    list_list_t list;
    int pushIntegerList[PushIntegerListCount];
    
    /* Set Up */
    memcpy(pushIntegerList, PushIntegerList, sizeof(PushIntegerList));
    
    /*** Find ***/
    /* Initialize */
    TEST_ASSERT_EQUAL_INT(ECLECTIC_STATUS_SUCCESS, list_init(&list, listTest_compareCallback, NULL));
    
    /* Push Tail */
    for(i = 0; i < PushIntegerListCount; i++)
    {
        /* Push Tail */
        eclectic_malloc_ExpectAndReturn(sizeof(list_node_t), malloc(sizeof(list_node_t))); // Mock
        TEST_ASSERT_EQUAL_INT(ECLECTIC_STATUS_SUCCESS, list_pushTail(&list, &pushIntegerList[i]));
        
        /* Size */
        TEST_ASSERT_EQUAL_UINT((i + 1), list_size(&list));
    }
    
    /* Find */
    for(i = 0; i < TestDataCount; i++)
        TEST_ASSERT_EQUAL_UINT(TestData[i].ExpectedResult, list_find(&list, &TestData[i].FindInteger));
    
    /* Destroy */
    for(i = 0; i < PushIntegerListCount; i++)
        eclectic_free_ExpectAnyArgs(); // Mock
    TEST_ASSERT_EQUAL_INT(ECLECTIC_STATUS_SUCCESS, list_destroy(&list));
}

/*** Initialize ***/
void test_init_error(void)
{    
    /*** Initialize ***/
    /* Initialize (NULL Pointer Error) */
    TEST_ASSERT_EQUAL_INT(ECLECTIC_STATUS_ERROR_NULL_POINTER, list_init(NULL, listTest_compareCallback, free));
}

void test_init_success(void)
{
    /*** Set Up ***/
    /* Variable */
    list_list_t list;
    
    /*** Initialize ***/
    /* Initialize */
    TEST_ASSERT_EQUAL_INT(ECLECTIC_STATUS_SUCCESS, list_init(&list, listTest_compareCallback, free));
    
    /* Verify */
    TEST_ASSERT_EQUAL_PTR(listTest_compareCallback, list.compareCallback);
    TEST_ASSERT_EQUAL_PTR(free, list.destroyCallback);
    TEST_ASSERT_NULL(list.head);
    TEST_ASSERT_NULL(list.tail);
    TEST_ASSERT_EQUAL_UINT(0, list.size);
}

/*** Peek At ***/
void test_list_peekAt_error(void)
{
    /*** Test Data ***/
    /* Variable */
    void *data;
    list_list_t list;
    
    /*** Peek At ***/
    /* Initialize */
    TEST_ASSERT_EQUAL_INT(ECLECTIC_STATUS_SUCCESS, list_init(&list, NULL, NULL));
    
    /* Peek At (NULL Pointer Error) */
    TEST_ASSERT_FALSE(list_peekAt(NULL, 0));
    
    /* Peek At (Length Error) */
    TEST_ASSERT_FALSE(list_peekAt(&list, 0));
}

void test_list_peekAt_success(void)
{
    /*** Test Data ***/
    /* Test Data */
    const int TestData[] = {24, 63, -59, 64, -85};
    const size_t TestDataCount = sizeof(TestData) / sizeof(TestData[0]);

    /* Variable */
    int *data;
    uint8_t i;
    list_list_t list;
    int pushIntegerList[TestDataCount];
    
    /* Set Up */
    memcpy(pushIntegerList, TestData, sizeof(TestData));
    
    /*** Peek At ***/
    /* Initialize */
    TEST_ASSERT_EQUAL_INT(ECLECTIC_STATUS_SUCCESS, list_init(&list, NULL, NULL));
    
    /* Push Tail */
    for(i = 0; i < TestDataCount; i++)
    {
        eclectic_malloc_ExpectAndReturn(sizeof(list_node_t), malloc(sizeof(list_node_t))); // Mock
        TEST_ASSERT_EQUAL_INT(ECLECTIC_STATUS_SUCCESS, list_pushTail(&list, &pushIntegerList[i]));
        
        /* Size */
        TEST_ASSERT_EQUAL_UINT((i + 1), list_size(&list));
    }
    
    /* Peek At */
    for(i = 0; i < TestDataCount; i++)
    {
        data = list_peekAt(&list, i);
        TEST_ASSERT_EQUAL_INT(pushIntegerList[i], *data);
        TEST_ASSERT_EQUAL_PTR(&pushIntegerList[i], data);
    }
    
    /* Destroy */
    for(i = 0; i < TestDataCount; i++)
        eclectic_free_ExpectAnyArgs(); // Mock
    TEST_ASSERT_EQUAL_INT(ECLECTIC_STATUS_SUCCESS, list_destroy(&list));
}

/*** Pop At ***/
void test_list_popAt_error(void)
{
    /*** Test Data ***/
    /* Variable */
    void *data;
    list_list_t list;
    
    /*** Pop At ***/
    /* Initialize */
    TEST_ASSERT_EQUAL_INT(ECLECTIC_STATUS_SUCCESS, list_init(&list, NULL, NULL));
    
    /* Pop At (NULL Pointer Error) */
    TEST_ASSERT_FALSE(list_popAt(NULL, 0));
    
    /* Pop At (Length Error) */
    TEST_ASSERT_FALSE(list_popAt(&list, 0));
}

void test_list_popAt_success(void)
{
    /*** Test Data ***/
    /* Type Definition */
    typedef struct testData_s
    {
        const size_t PopIndex;
        const int ExpectedPopInteger;
        const int ExpectedIntegerList[LIST_TEST_LIST_COUNT - 1];
    } testData_t;

    /* Test Data */
    const int PushIntegerList[LIST_TEST_LIST_COUNT] = {24, 63, -59, 64, -85};
    const testData_t TestData[] =
    {
        {
            0,
            24,
            {63, -59, 64, -85}
        },
        {
            1,
            63,
            {24, -59, 64, -85}
        },
        {
            2,
            -59,
            {24, 63, 64, -85}
        },
        {
            3,
            64,
            {24, 63, -59, -85}
        },
        {
            4,
            -85,
            {24, 63, -59, 64}
        }
    };
    const size_t TestDataCount = sizeof(TestData) / sizeof(TestData[0]);

    /* Variable */
    int *data;
    uint8_t i, j;
    list_list_t list;
    int pushIntegerList[LIST_TEST_LIST_COUNT];
    
    /*** Pop At ***/
    for(i = 0; i < TestDataCount; i++)
    {
        /* Set Up */
        memcpy(pushIntegerList, PushIntegerList, sizeof(PushIntegerList));
        
        /* Initialize */
        TEST_ASSERT_EQUAL_INT(ECLECTIC_STATUS_SUCCESS, list_init(&list, NULL, NULL));
        
        /* Push Tail */
        for(j = 0; j < LIST_TEST_LIST_COUNT; j++)
        {
            /* Push Tail */
            eclectic_malloc_ExpectAndReturn(sizeof(list_node_t), malloc(sizeof(list_node_t))); // Mock
            TEST_ASSERT_EQUAL_INT(ECLECTIC_STATUS_SUCCESS, list_pushTail(&list, &pushIntegerList[j]));
            
            /* Size */
            TEST_ASSERT_EQUAL_UINT((j + 1), list_size(&list));
        }
        
        /* Pop At */
        eclectic_free_ExpectAnyArgs(); // Mock
        data = list_popAt(&list, TestData[i].PopIndex);
        TEST_ASSERT_NOT_NULL(data);
        TEST_ASSERT_EQUAL_INT(TestData[i].ExpectedPopInteger, *data);
        TEST_ASSERT_EQUAL_UINT((LIST_TEST_LIST_COUNT - 1), list_size(&list));
        
        /* Peek At */
        for(j = 0; j < (LIST_TEST_LIST_COUNT - 1); j++)
        {
            data = list_peekAt(&list, j);
            TEST_ASSERT_EQUAL_INT(TestData[i].ExpectedIntegerList[j], *data);
        }
        
        /* Pop Until Empty (In Random Order) */
        for(j = 0; j < (LIST_TEST_LIST_COUNT - 1); j++)
        {
            eclectic_free_ExpectAnyArgs(); // Mock
            (void)list_popAt(&list, (rand() % list_size(&list)));
        }
    }
}

/*** Pop Head ***/
void test_list_popHead_error(void)
{
    /*** Test Data ***/
    /* Variable */
    void *data;
    list_list_t list;
    
    /*** Pop Head ***/
    /* Initialize */
    TEST_ASSERT_EQUAL_INT(ECLECTIC_STATUS_SUCCESS, list_init(&list, NULL, NULL));
    
    /* Pop Head  (NULL Pointer Error) */
    data = list_popHead(NULL);
    TEST_ASSERT_NULL(data);
    
    /* Pop Head  (Length Error) */
    data = list_popHead(&list);
    TEST_ASSERT_NULL(data);
}

void test_list_popHead_success(void)
{
    /*** Test Data ***/
    /* Test Data */
    const int TestData[] = {24, 63, -59, 64, -85};
    const size_t TestDataCount = sizeof(TestData) / sizeof(TestData[0]);

    /* Variable */
    int *data;
    uint8_t i;
    list_list_t list;
    int pushIntegerList[TestDataCount];
    
    /* Set Up */
    memcpy(pushIntegerList, TestData, sizeof(TestData));
    
    /*** Pop Head ***/
    /* Initialize */
    TEST_ASSERT_EQUAL_INT(ECLECTIC_STATUS_SUCCESS, list_init(&list, NULL, NULL));
    
    /* Push Tail */
    for(i = 0; i < TestDataCount; i++)
    {
        /* Push Tail */
        eclectic_malloc_ExpectAndReturn(sizeof(list_node_t), malloc(sizeof(list_node_t))); // Mock
        TEST_ASSERT_EQUAL_INT(ECLECTIC_STATUS_SUCCESS, list_pushTail(&list, &pushIntegerList[i]));
        
        /* Size */
        TEST_ASSERT_EQUAL_UINT((i + 1), list_size(&list));
    }
    
    /* Pop Head */
    for(i = 0; i < TestDataCount; i++)
    {
        eclectic_free_ExpectAnyArgs(); // Mock
        data = list_popHead(&list);
        TEST_ASSERT_EQUAL_INT(pushIntegerList[i], *data);
        TEST_ASSERT_EQUAL_PTR(&pushIntegerList[i], data);
        TEST_ASSERT_EQUAL_UINT((TestDataCount - (i + 1)), list_size(&list));
    }
    
    /* Destroy */
    TEST_ASSERT_EQUAL_INT(ECLECTIC_STATUS_SUCCESS, list_destroy(&list));
}

/*** Pop Tail ***/
void test_list_popTail_error(void)
{
    /*** Test Data ***/
    /* Variable */
    void *data;
    list_list_t list;
    
    /*** Pop Tail ***/
    /* Initialize */
    TEST_ASSERT_EQUAL_INT(ECLECTIC_STATUS_SUCCESS, list_init(&list, NULL, NULL));
    
    /* Pop Tail (NULL Pointer Error) */
    data = list_popTail(NULL);
    TEST_ASSERT_NULL(data);
    
    /* Pop Tail (Length Error) */
    data = list_popTail(&list);
    TEST_ASSERT_NULL(data);
}

void test_list_popTail_success(void)
{
    /*** Test Data ***/
    /* Test Data */
    const int TestData[] = {24, 63, -59, 64, -85};
    const size_t TestDataCount = sizeof(TestData) / sizeof(TestData[0]);

    /* Variable */
    int *data;
    uint8_t i;
    list_list_t list;
    int pushIntegerList[TestDataCount];
    
    /* Set Up */
    memcpy(pushIntegerList, TestData, sizeof(TestData));
    
    /*** Pop Tail ***/
    /* Initialize */
    TEST_ASSERT_EQUAL_INT(ECLECTIC_STATUS_SUCCESS, list_init(&list, NULL, NULL));
    
    /* Push Head */
    for(i = 0; i < TestDataCount; i++)
    {
        /* Push Head */
        eclectic_malloc_ExpectAndReturn(sizeof(list_node_t), malloc(sizeof(list_node_t))); // Mock
        TEST_ASSERT_EQUAL_INT(ECLECTIC_STATUS_SUCCESS, list_pushHead(&list, &pushIntegerList[i]));
        
        /* Size */
        TEST_ASSERT_EQUAL_UINT((i + 1), list_size(&list));
    }
    
    /* Pop Tail */
    for(i = 0; i < TestDataCount; i++)
    {
        eclectic_free_ExpectAnyArgs(); // Mock
        data = list_popTail(&list);
        TEST_ASSERT_EQUAL_INT(pushIntegerList[i], *data);
        TEST_ASSERT_EQUAL_PTR(&pushIntegerList[i], data);
        TEST_ASSERT_EQUAL_UINT((TestDataCount - (i + 1)), list_size(&list));
    }
    
    /* Destroy */
    TEST_ASSERT_EQUAL_INT(ECLECTIC_STATUS_SUCCESS, list_destroy(&list));
}

/*** Push At ***/
void test_list_pushAt_error(void)
{
    /*** Test Data ***/    
    /* Variable */
    int data[3];
    list_list_t list;
    
    /*** Push At ***/
    /* Initialize */
    TEST_ASSERT_EQUAL_INT(ECLECTIC_STATUS_SUCCESS, list_init(&list, NULL, NULL));
    
    /* Push Head */
    eclectic_malloc_ExpectAndReturn(sizeof(list_node_t), malloc(sizeof(list_node_t))); // Mock
    TEST_ASSERT_EQUAL_INT(ECLECTIC_STATUS_SUCCESS, list_pushHead(&list, &data[0]));
    eclectic_malloc_ExpectAndReturn(sizeof(list_node_t), malloc(sizeof(list_node_t))); // Mock
    TEST_ASSERT_EQUAL_INT(ECLECTIC_STATUS_SUCCESS, list_pushHead(&list, &data[1]));
    
    /* Size */
    TEST_ASSERT_EQUAL_UINT(2, list_size(&list));
    
    /* Push At (NULL Pointer Error) */
    TEST_ASSERT_EQUAL_INT(ECLECTIC_STATUS_ERROR_NULL_POINTER, list_pushAt(NULL, &data[2], 0));
    
    /* Push At (Invalid Error) */
    TEST_ASSERT_EQUAL_INT(ECLECTIC_STATUS_ERROR_INVALID, list_pushAt(&list, &data[2], 2));
    
    /* Push At (Memory Allocation Error) */
    eclectic_malloc_ExpectAndReturn(sizeof(list_node_t), NULL); // Mock
    TEST_ASSERT_EQUAL_INT(ECLECTIC_STATUS_ERROR_MEMORY_ALLOCATION, list_pushAt(&list, &data[2], 1));
    TEST_ASSERT_EQUAL_UINT(2, list_size(&list));
}

void test_list_pushAt_success(void)
{
    /*** Test Data ***/
    /* Type Definition */
    typedef struct testData_s
    {
        const size_t PushIndex;
        const int PushInteger;
        const int ExpectedIntegerList[LIST_TEST_LIST_COUNT];
    } testData_t;

    /* Test Data */
    const int PushIntegerList[LIST_TEST_LIST_COUNT - 1] = {24, 63, -59, 64};
    const testData_t TestData[] =
    {
        {
            0,
            -85,
            {-85, 24, 63, -59, 64}
        },
        {
            1,
            -85,
            {24, -85, 63, -59, 64}
        },
        {
            2,
            -85,
            {24, 63, -85, -59, 64}
        },
        {
            3,
            -85,
            {24, 63, -59, -85, 64}
        }
    };
    const size_t TestDataCount = sizeof(TestData) / sizeof(TestData[0]);

    /* Variable */
    int *peekedData, pushedData;
    uint8_t i, j;
    list_list_t list;
    int pushIntegerList[LIST_TEST_LIST_COUNT];
    
    /*** Push At ***/
    for(i = 0; i < TestDataCount; i++)
    {
        /* Set Up */
        memcpy(pushIntegerList, PushIntegerList, sizeof(PushIntegerList));
        
        /* Initialize */
        TEST_ASSERT_EQUAL_INT(ECLECTIC_STATUS_SUCCESS, list_init(&list, NULL, NULL));
        
        /* Push Tail */
        for(j = 0; j < (LIST_TEST_LIST_COUNT - 1); j++)
        {
            /* Push Tail */
            eclectic_malloc_ExpectAndReturn(sizeof(list_node_t), malloc(sizeof(list_node_t))); // Mock
            TEST_ASSERT_EQUAL_INT(ECLECTIC_STATUS_SUCCESS, list_pushTail(&list, &pushIntegerList[j]));
            
            /* Size */
            TEST_ASSERT_EQUAL_UINT((j + 1), list_size(&list));
        }
        
        /* Push At */
        pushedData = TestData[i].PushInteger;
        eclectic_malloc_ExpectAndReturn(sizeof(list_node_t), malloc(sizeof(list_node_t))); // Mock
        TEST_ASSERT_EQUAL_INT(ECLECTIC_STATUS_SUCCESS, list_pushAt(&list, &pushedData, TestData[i].PushIndex));
        
        /* Size */
        TEST_ASSERT_EQUAL_UINT(LIST_TEST_LIST_COUNT, list_size(&list));
        
        /* Peek At */
        for(j = 0; j < LIST_TEST_LIST_COUNT; j++)
        {
            peekedData = list_peekAt(&list, j);
            TEST_ASSERT_EQUAL_INT(TestData[i].ExpectedIntegerList[j], *peekedData);
        }
        
        /* Destroy */
        for(j = 0; j < LIST_TEST_LIST_COUNT; j++)
            eclectic_free_ExpectAnyArgs(); // Mock
        TEST_ASSERT_EQUAL_INT(ECLECTIC_STATUS_SUCCESS, list_destroy(&list));
    }
}

/*** Push Head ***/
void test_list_pushHead_error(void)
{
    /*** Test Data ***/
    /* Variable */
    void *data;
    list_list_t list;
    
    /*** Push Head ***/
    /* Initialize */
    TEST_ASSERT_EQUAL_INT(ECLECTIC_STATUS_SUCCESS, list_init(&list, NULL, NULL));
    
    /* Push Head (NULL Pointer Error) */
    TEST_ASSERT_EQUAL_INT(ECLECTIC_STATUS_ERROR_NULL_POINTER, list_pushHead(NULL, data));
    
    /* Push Head (Memory Allocation Error) */
    eclectic_malloc_ExpectAndReturn(sizeof(list_node_t), NULL); // Mock
    TEST_ASSERT_EQUAL_INT(ECLECTIC_STATUS_ERROR_MEMORY_ALLOCATION, list_pushHead(&list, data));
    TEST_ASSERT_EQUAL_UINT(0, list_size(&list));
}

void test_list_pushHead_success(void)
{
    /*** Test Data ***/
    /* Test Data */
    const int TestInteger = 24;

    /* Variable */
    int *data;
    list_list_t list;
    int pushInteger;
    
    /* Set Up */
    pushInteger = TestInteger;
    
    /*** Push Head ***/
    /* Initialize */
    TEST_ASSERT_EQUAL_INT(ECLECTIC_STATUS_SUCCESS, list_init(&list, NULL, NULL));
    
    /* Push Head */
    eclectic_malloc_ExpectAndReturn(sizeof(list_node_t), malloc(sizeof(list_node_t))); // Mock
    TEST_ASSERT_EQUAL_INT(ECLECTIC_STATUS_SUCCESS, list_pushHead(&list, &pushInteger));
    
    /* Size */
    TEST_ASSERT_EQUAL_UINT(1, list_size(&list));
    
    /* Peek At */
    data = list_peekAt(&list, 0);
    TEST_ASSERT_EQUAL_INT(pushInteger, *data);
    TEST_ASSERT_EQUAL_PTR(&pushInteger, data);
    
    /* Destroy */
    eclectic_free_ExpectAnyArgs(); // Mock
    TEST_ASSERT_EQUAL_INT(ECLECTIC_STATUS_SUCCESS, list_destroy(&list));
}

/*** Push Sorted ***/
void test_list_pushSorted_error(void)
{
    /*** Test Data ***/
    /* Test Data */
    const int TestData[3] = {-1, 1, 0};
    
    /* Variable */
    size_t i;
    list_list_t list;
    int pushIntegerList[3];
    
    /* Set Up */
    memcpy(pushIntegerList, TestData, sizeof(TestData));
    
    /*** Push Sorted ***/
    /* Push Sorted (NULL Pointer Error) */
    TEST_ASSERT_EQUAL_INT(ECLECTIC_STATUS_ERROR_NULL_POINTER, list_pushSorted(NULL, &pushIntegerList[0], false));
    
    /* Initialize */
    TEST_ASSERT_EQUAL_INT(ECLECTIC_STATUS_SUCCESS, list_init(&list, NULL, NULL));
    
    /* Push Sorted (Not Initialized Error) */
    TEST_ASSERT_EQUAL_INT(ECLECTIC_STATUS_SUCCESS, list_init(&list, NULL, NULL));
    TEST_ASSERT_EQUAL_INT(ECLECTIC_STATUS_ERROR_NOT_INITIALIZED, list_pushSorted(&list, &pushIntegerList[0], false)); // list.compareCallback Is NULL
    
    /* Destroy */
    TEST_ASSERT_EQUAL_INT(ECLECTIC_STATUS_SUCCESS, list_destroy(&list));
    
    /* Initialize */
    TEST_ASSERT_EQUAL_INT(ECLECTIC_STATUS_SUCCESS, list_init(&list, listTest_compareCallback, NULL));
    
    /* Push Sorted */
    eclectic_malloc_ExpectAndReturn(sizeof(list_node_t), malloc(sizeof(list_node_t))); // Mock
    TEST_ASSERT_EQUAL_INT(ECLECTIC_STATUS_SUCCESS, list_pushSorted(&list, &pushIntegerList[0], false));
    eclectic_malloc_ExpectAndReturn(sizeof(list_node_t), malloc(sizeof(list_node_t))); // Mock
    TEST_ASSERT_EQUAL_INT(ECLECTIC_STATUS_SUCCESS, list_pushSorted(&list, &pushIntegerList[1], false));
    
    /* Size */
    TEST_ASSERT_EQUAL_UINT(2, list_size(&list));
    
    /* Push Sorted (Memory Allocation Error) */
    eclectic_malloc_ExpectAndReturn(sizeof(list_node_t), NULL); // Mock
    TEST_ASSERT_EQUAL_INT(ECLECTIC_STATUS_ERROR_MEMORY_ALLOCATION, list_pushSorted(&list, &pushIntegerList[2], false));
    TEST_ASSERT_EQUAL_UINT(2, list_size(&list));
    
    /* Destroy */
    for(i = 0; i < 2; i++)
        eclectic_free_ExpectAnyArgs(); // Mock
    TEST_ASSERT_EQUAL_INT(ECLECTIC_STATUS_SUCCESS, list_destroy(&list));
}

void test_list_pushSorted_success_ascending(void)
{
    /*** Test Data ***/
    /* Type Definition */
    typedef struct testData_s
    {
        const int PushIntegerList[LIST_TEST_LIST_COUNT];
        const int ExpectedIntegerList[LIST_TEST_LIST_COUNT];
    } testData_t;

    /* Test Data */
    const testData_t TestData[] =
    {
        /* Same Value */
        {
            {-85, -85, -85, -85, -85},
            {-85, -85, -85, -85, -85}
        },
        {
            {0, 0, 0, 0, 0},
            {0, 0, 0, 0, 0}
        },
        {
            {64, 64, 64, 64, 64},
            {64, 64, 64, 64, 64}
        },
        
        /* Different Values */
        {
            {-24, -63, -59, -64, -85},
            {-85, -64, -63, -59, -24}
        },
        {
            {24, 63, -59, 64, -85},
            {-85, -59, 24, 63, 64}
        },
        {
            {24, 63, 59, 64, 85},
            {24, 59, 63, 64, 85}
        }
    };
    const size_t TestDataCount = sizeof(TestData) / sizeof(TestData[0]);

    /* Variable */
    int *data;
    uint8_t i, j;
    list_list_t list;
    int pushIntegerList[LIST_TEST_LIST_COUNT];
    
    /*** Push Sorted ***/
    for(i = 0; i < TestDataCount; i++)
    {        
        /* Set Up */
        memcpy(pushIntegerList, TestData[i].PushIntegerList, sizeof(TestData[i].PushIntegerList));
        TEST_ASSERT_EQUAL_INT(ECLECTIC_STATUS_SUCCESS, list_init(&list, listTest_compareCallback, NULL));
        for(j = 0; j < LIST_TEST_LIST_COUNT; j++)
        {
            eclectic_malloc_ExpectAndReturn(sizeof(list_node_t), malloc(sizeof(list_node_t))); // Mock
            TEST_ASSERT_EQUAL_INT(ECLECTIC_STATUS_SUCCESS, list_pushSorted(&list, &pushIntegerList[j], false));
            TEST_ASSERT_EQUAL_UINT((j + 1), list_size(&list));
        }
        
        /* Verify */
        for(j = 0; j < LIST_TEST_LIST_COUNT; j++)
        {
            data = list_peekAt(&list, j);
            TEST_ASSERT_EQUAL_INT(TestData[i].ExpectedIntegerList[j], *data);
        }
        
        /* Destroy */
        for(j = 0; j < LIST_TEST_LIST_COUNT; j++)
            eclectic_free_ExpectAnyArgs(); // Mock
        TEST_ASSERT_EQUAL_INT(ECLECTIC_STATUS_SUCCESS, list_destroy(&list));
    }
}

void test_list_pushSorted_success_descending(void)
{
    /*** Test Data ***/
    /* Type Definition */
    typedef struct testData_s
    {
        const int PushIntegerList[LIST_TEST_LIST_COUNT];
        const int ExpectedIntegerList[LIST_TEST_LIST_COUNT];
    } testData_t;

    /* Test Data */
    const testData_t TestData[] =
    {
        /* Same Value */
        {
            {-85, -85, -85, -85, -85},
            {-85, -85, -85, -85, -85}
        },
        {
            {0, 0, 0, 0, 0},
            {0, 0, 0, 0, 0}
        },
        {
            {64, 64, 64, 64, 64},
            {64, 64, 64, 64, 64}
        },
        
        /* Different Values */
        {
            {-24, -63, -59, -64, -85},
            {-24, -59, -63, -64, -85}
        },
        {
            {24, 63, -59, 64, -85},
            {64, 63, 24, -59, -85}
        },
        {
            {24, 63, 59, 64, 85},
            {85, 64, 63, 59, 24}
        }
    };
    const size_t TestDataCount = sizeof(TestData) / sizeof(TestData[0]);

    /* Variable */
    int *data;
    uint8_t i, j;
    list_list_t list;
    int pushIntegerList[LIST_TEST_LIST_COUNT];
    
    /*** Push Sorted ***/
    for(i = 0; i < TestDataCount; i++)
    {        
        /* Set Up */
        memcpy(pushIntegerList, TestData[i].PushIntegerList, sizeof(TestData[i].PushIntegerList));
        TEST_ASSERT_EQUAL_INT(ECLECTIC_STATUS_SUCCESS, list_init(&list, listTest_compareCallback, NULL));
        for(j = 0; j < LIST_TEST_LIST_COUNT; j++)
        {
            eclectic_malloc_ExpectAndReturn(sizeof(list_node_t), malloc(sizeof(list_node_t))); // Mock
            TEST_ASSERT_EQUAL_INT(ECLECTIC_STATUS_SUCCESS, list_pushSorted(&list, &pushIntegerList[j], true));
            TEST_ASSERT_EQUAL_UINT((j + 1), list_size(&list));
        }
        
        /* Verify */
        for(j = 0; j < LIST_TEST_LIST_COUNT; j++)
        {
            data = list_peekAt(&list, j);
            TEST_ASSERT_EQUAL_INT(TestData[i].ExpectedIntegerList[j], *data);
        }
        
        /* Destroy */
        for(j = 0; j < LIST_TEST_LIST_COUNT; j++)
            eclectic_free_ExpectAnyArgs(); // Mock
        TEST_ASSERT_EQUAL_INT(ECLECTIC_STATUS_SUCCESS, list_destroy(&list));
    }
}

/*** Push Tail ***/
void test_list_pushTail_error(void)
{
    /*** Test Data ***/
    /* Variable */
    void *data;
    list_list_t list;
    
    /* Set Up */
    TEST_ASSERT_EQUAL_INT(ECLECTIC_STATUS_SUCCESS, list_init(&list, NULL, NULL));
    
    /*** Push Tail ***/
    /* Push Tail (NULL Pointer Error) */
    TEST_ASSERT_EQUAL_INT(ECLECTIC_STATUS_ERROR_NULL_POINTER, list_pushTail(NULL, data));
    
    /* Push Tail (Memory Allocation Error) */
    eclectic_malloc_ExpectAndReturn(sizeof(list_node_t), NULL); // Mock
    TEST_ASSERT_EQUAL_INT(ECLECTIC_STATUS_ERROR_MEMORY_ALLOCATION, list_pushTail(&list, data));
    TEST_ASSERT_EQUAL_UINT(0, list_size(&list));
}

void test_list_pushTail_success(void)
{
    /*** Test Data ***/
    /* Test Data */
    const int TestInteger = 24;

    /* Variable */
    int *data;
    list_list_t list;
    int pushInteger;
    
    /* Set Up */
    TEST_ASSERT_EQUAL_INT(ECLECTIC_STATUS_SUCCESS, list_init(&list, NULL, NULL));
    pushInteger = TestInteger;
    
    /* Mock */
    eclectic_malloc_ExpectAndReturn(sizeof(list_node_t), malloc(sizeof(list_node_t)));
    
    /*** Push Tail ***/
    /* Push Tail */
    TEST_ASSERT_EQUAL_INT(ECLECTIC_STATUS_SUCCESS, list_pushTail(&list, &pushInteger));
    TEST_ASSERT_EQUAL_UINT(1, list_size(&list));
    data = list_peekAt(&list, 0);
    TEST_ASSERT_EQUAL_INT(pushInteger, *data);
    TEST_ASSERT_EQUAL_PTR(&pushInteger, data);
    
    /* Destroy */
    eclectic_free_ExpectAnyArgs(); // Mock
    TEST_ASSERT_EQUAL_INT(ECLECTIC_STATUS_SUCCESS, list_destroy(&list));
}

/*** Size ***/
void test_list_size_error(void)
{
    /*** Size ***/
    /* Size (NULL Pointer Error) */
    TEST_ASSERT_EQUAL_UINT(0, list_size(NULL));
}

void test_list_size_success(void)
{
    /*** Test Data ***/
    /* Test Data */
    const int TestData[] = {24, 63, 64, -69, -85};
    const size_t TestDataCount = sizeof(TestData) / sizeof(TestData[0]);

    /* Variable */
    int *data;
    uint8_t i;
    list_list_t list;
    int pushIntegerList[TestDataCount];
    
    /* Set Up */
    memcpy(pushIntegerList, TestData, sizeof(TestData));
    TEST_ASSERT_EQUAL_INT(ECLECTIC_STATUS_SUCCESS, list_init(&list, NULL, NULL));
    
    /*** Size ***/
    for(i = 0; i < TestDataCount; i++)
    {
        eclectic_malloc_ExpectAndReturn(sizeof(list_node_t), malloc(sizeof(list_node_t))); // Mock
        TEST_ASSERT_EQUAL_INT(ECLECTIC_STATUS_SUCCESS, list_pushTail(&list, &pushIntegerList[i]));
        TEST_ASSERT_EQUAL_UINT((i + 1), list_size(&list));
    }
    
    /* Destroy */
    for(i = 0; i < TestDataCount; i++)
        eclectic_free_ExpectAnyArgs(); // Mock
    TEST_ASSERT_EQUAL_INT(ECLECTIC_STATUS_SUCCESS, list_destroy(&list));
}
