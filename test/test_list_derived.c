/****************************************************************************************************
 * Define
 ****************************************************************************************************/

#define LIST_DERIVED_TEST_LIST_COUNT (10)

/****************************************************************************************************
 * Include
 ****************************************************************************************************/

#include "eclectic.h"
#include <limits.h>
#include "list.h"
#include "queue.h"
#include "set.h"
#include "stack.h"
#include <string.h>
#include "unity.h"

/****************************************************************************************************
 * Helper
 ****************************************************************************************************/

/*** Compare Callback ***/
int listDerivedTest_compareCallback(const void * const Data1, const void * const Data2)
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
        actualResult = listDerivedTest_compareCallback(&TestData[i].Integer1, &TestData[i].Integer2);
        TEST_ASSERT_EQUAL_INT(TestData[i].ExpectedResult, actualResult);
    }
}

/****************************************************************************************************
 * Set Up/Tear Down
 ****************************************************************************************************/

void setUp(void)
{
    eclectic_init(free, malloc);
}

void tearDown(void)
{
}

/****************************************************************************************************
 * Test (Public)
 ****************************************************************************************************/

/*** Queue ***/
void test_queue(void)
{
    /*** Test Data ***/
    /* Type Definition */
    typedef struct testData_s
    {
        const int EnqueueIntegerList[LIST_DERIVED_TEST_LIST_COUNT];
        const int ExpectedIntegerList[LIST_DERIVED_TEST_LIST_COUNT];
    } testData_t;

    /* Test Data */
    const testData_t TestData[] =
    {
        /* Same Value */
        {
            {-85, -85, -85, -85, -85, -85, -85, -85, -85, -85},
            {-85, -85, -85, -85, -85, -85, -85, -85, -85, -85}
        },
        {
            {0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
            {0, 0, 0, 0, 0, 0, 0, 0, 0, 0}
        },
        {
            {64, 64, 64, 64, 64, 64, 64, 64, 64, 64},
            {64, 64, 64, 64, 64, 64, 64, 64, 64, 64}
        },

        /* Different Values */
        {
            {-82, -78, -11, -43, -79, -95, -50, -72, -98, -6},
            {-82, -78, -11, -43, -79, -95, -50, -72, -98, -6}
        },
        {
            {-82, 78, -11, 43, -79, 95, -50, 72, -98, 6},
            {-82, 78, -11, 43, -79, 95, -50, 72, -98, 6}
        },
        {
            {82, 78, 11, 43, 79, 95, 50, 72, 98, 6},
            {82, 78, 11, 43, 79, 95, 50, 72, 98, 6}
        }
    };
    const size_t TestDataCount = sizeof(TestData) / sizeof(TestData[0]);

    /* Variable */
    int *data;
    uint8_t i, j;
    queue_queue_t queue;

    /*** Queue ***/
    for(i = 0; i < TestDataCount; i++)
    {
        /* Initialize */
        TEST_ASSERT_EQUAL_INT(ECLECTIC_STATUS_SUCCESS, queue_init(&queue, free));

        /* Enqueue */
        for(j = 0; j < LIST_DERIVED_TEST_LIST_COUNT; j++)
        {
            data = malloc(sizeof(*data));
            *data = TestData[i].EnqueueIntegerList[j];
            TEST_ASSERT_EQUAL_INT(ECLECTIC_STATUS_SUCCESS, queue_enqueue(&queue, data));
            TEST_ASSERT_EQUAL_UINT((j + 1), queue_size(&queue));
        }

        /* Destroy */
        TEST_ASSERT_EQUAL_INT(ECLECTIC_STATUS_SUCCESS, queue_destroy(&queue));
        TEST_ASSERT_EQUAL_UINT(0, queue_size(&queue));

        /* Enqueue */
        for(j = 0; j < LIST_DERIVED_TEST_LIST_COUNT; j++)
        {
            data = malloc(sizeof(*data));
            *data = TestData[i].EnqueueIntegerList[j];
            TEST_ASSERT_EQUAL_INT(ECLECTIC_STATUS_SUCCESS, queue_enqueue(&queue, data));
            TEST_ASSERT_EQUAL_UINT((j + 1), queue_size(&queue));
        }

        /* Dequeue */
        for(j = 0; j < LIST_DERIVED_TEST_LIST_COUNT; j++)
        {
            data = queue_dequeue(&queue);
            TEST_ASSERT_EQUAL_INT(TestData[i].ExpectedIntegerList[j], *data);
        }

        /* Destroy */
        TEST_ASSERT_EQUAL_INT(ECLECTIC_STATUS_SUCCESS, queue_destroy(&queue));
    }
}

/*** Set ***/
void test_set(void)
{
    /*** Test Data ***/
    /* Type Definition */
    typedef struct testData_s
    {
        const int InsertIntegerList[LIST_DERIVED_TEST_LIST_COUNT];
        const eclectic_status_t ExpectedState[LIST_DERIVED_TEST_LIST_COUNT];
        const int ExpectedIntegerList[LIST_DERIVED_TEST_LIST_COUNT];
        const size_t ExpectedIntegerListCount;
    } testData_t;

    /* Test Data */
    const testData_t TestData[] =
    {
        {
            {-82, 78, -11, 43, -79, 95, -50, 72, -98, 6},
            {
                ECLECTIC_STATUS_SUCCESS,
                ECLECTIC_STATUS_SUCCESS,
                ECLECTIC_STATUS_SUCCESS,
                ECLECTIC_STATUS_SUCCESS,
                ECLECTIC_STATUS_SUCCESS,
                ECLECTIC_STATUS_SUCCESS,
                ECLECTIC_STATUS_SUCCESS,
                ECLECTIC_STATUS_SUCCESS,
                ECLECTIC_STATUS_SUCCESS,
                ECLECTIC_STATUS_SUCCESS
            },
            {-98, -82, -79, -50, -11, 6, 43, 72, 78, 95},
            10
        },
        {
            {-82, 78, -11, 43, -79, 95, -50, 78, -98, 6},
            {
                ECLECTIC_STATUS_SUCCESS,
                ECLECTIC_STATUS_SUCCESS,
                ECLECTIC_STATUS_SUCCESS,
                ECLECTIC_STATUS_SUCCESS,
                ECLECTIC_STATUS_SUCCESS,
                ECLECTIC_STATUS_SUCCESS,
                ECLECTIC_STATUS_SUCCESS,
                ECLECTIC_STATUS_ERROR_DUPLICATE,
                ECLECTIC_STATUS_SUCCESS,
                ECLECTIC_STATUS_SUCCESS
            },
            {-98, -82, -79, -50, -11, 6, 43, 78, 95, INT_MIN},
            9
        },
        {
            {-82, 78, 78, 78, -82, -82, 78, -82, 78, 78},
            {
                ECLECTIC_STATUS_SUCCESS,
                ECLECTIC_STATUS_SUCCESS,
                ECLECTIC_STATUS_ERROR_DUPLICATE,
                ECLECTIC_STATUS_ERROR_DUPLICATE,
                ECLECTIC_STATUS_ERROR_DUPLICATE,
                ECLECTIC_STATUS_ERROR_DUPLICATE,
                ECLECTIC_STATUS_ERROR_DUPLICATE,
                ECLECTIC_STATUS_ERROR_DUPLICATE,
                ECLECTIC_STATUS_ERROR_DUPLICATE,
                ECLECTIC_STATUS_ERROR_DUPLICATE
            },
            {-82, 78, INT_MIN, INT_MIN, INT_MIN, INT_MIN, INT_MIN, INT_MIN, INT_MIN, INT_MIN},
            2
        },
        {
            {-82, -82, -82, -82, -82, -82, -82, -82, -82, -82},
            {
                ECLECTIC_STATUS_SUCCESS,
                ECLECTIC_STATUS_ERROR_DUPLICATE,
                ECLECTIC_STATUS_ERROR_DUPLICATE,
                ECLECTIC_STATUS_ERROR_DUPLICATE,
                ECLECTIC_STATUS_ERROR_DUPLICATE,
                ECLECTIC_STATUS_ERROR_DUPLICATE,
                ECLECTIC_STATUS_ERROR_DUPLICATE,
                ECLECTIC_STATUS_ERROR_DUPLICATE,
                ECLECTIC_STATUS_ERROR_DUPLICATE,
                ECLECTIC_STATUS_ERROR_DUPLICATE
            },
            {-82, INT_MIN, INT_MIN, INT_MIN, INT_MIN, INT_MIN, INT_MIN, INT_MIN, INT_MIN, INT_MIN},
            1
        }
    };
    const size_t TestDataCount = sizeof(TestData) / sizeof(TestData[0]);

    /* Variable */
    int *data;
    size_t expectedSize;
    uint8_t i, j;
    set_set_t set;
    
   /*** Set ***/
   for(i = 0; i < TestDataCount; i++)
    {
        /* Initialize */
        TEST_ASSERT_EQUAL_INT(ECLECTIC_STATUS_SUCCESS, set_init(&set, listDerivedTest_compareCallback, free));

        /* Insert */
        for(j = 0; j < LIST_DERIVED_TEST_LIST_COUNT; j++)
        {
            data = malloc(sizeof(*data));
            *data = TestData[i].InsertIntegerList[j];
            TEST_ASSERT_EQUAL_INT(TestData[i].ExpectedState[j], set_insert(&set, data));
        }
        TEST_ASSERT_EQUAL_UINT(TestData[i].ExpectedIntegerListCount, set_size(&set));

        /* Destroy */
        TEST_ASSERT_EQUAL_INT(ECLECTIC_STATUS_SUCCESS, set_destroy(&set));
        TEST_ASSERT_EQUAL_UINT(0, set_size(&set));

        /* Insert */
        for(j = 0; j < LIST_DERIVED_TEST_LIST_COUNT; j++)
        {
            data = malloc(sizeof(*data));
            *data = TestData[i].InsertIntegerList[j];
            TEST_ASSERT_EQUAL_INT(TestData[i].ExpectedState[j], set_insert(&set, data));
        }
        TEST_ASSERT_EQUAL_UINT(TestData[i].ExpectedIntegerListCount, set_size(&set));

        /* Peek And Remove */
        expectedSize = TestData[i].ExpectedIntegerListCount;
        for(j = 0; j < TestData[i].ExpectedIntegerListCount; j++)
        {
            /* Peek */
            data = set_peek(&set);
            TEST_ASSERT_EQUAL_INT(TestData[i].ExpectedIntegerList[j], *data);
            TEST_ASSERT_EQUAL_UINT(expectedSize, set_size(&set));
            
            /* Remove */
            data = set_remove(&set);
            TEST_ASSERT_EQUAL_INT(TestData[i].ExpectedIntegerList[j], *data);
            TEST_ASSERT_EQUAL_UINT(--expectedSize, set_size(&set));
        }

        /* Destroy */
        TEST_ASSERT_EQUAL_INT(ECLECTIC_STATUS_SUCCESS, set_destroy(&set));
    }
}

/*** Stack ***/
void test_stack(void)
{
    /*** Test Data ***/
    /* Type Definition */
    typedef struct testData_s
    {
        const int PushIntegerList[LIST_DERIVED_TEST_LIST_COUNT];
        const int ExpectedIntegerList[LIST_DERIVED_TEST_LIST_COUNT];
    } testData_t;

    /* Test Data */
    const testData_t TestData[] =
    {
        /* Same Value */
        {
            {-85, -85, -85, -85, -85, -85, -85, -85, -85, -85},
            {-85, -85, -85, -85, -85, -85, -85, -85, -85, -85}
        },
        {
            {0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
            {0, 0, 0, 0, 0, 0, 0, 0, 0, 0}
        },
        {
            {64, 64, 64, 64, 64, 64, 64, 64, 64, 64},
            {64, 64, 64, 64, 64, 64, 64, 64, 64, 64}
        },

        /* Different Values */
        {
            {-82, -78, -11, -43, -79, -95, -50, -72, -98, -6},
            {-6, -98, -72, -50, -95, -79, -43, -11, -78, -82}
        },
        {
            {-82, 78, -11, 43, -79, 95, -50, 72, -98, 6},
            {6, -98, 72, -50, 95, -79, 43, -11, 78, -82}
        },
        {
            {82, 78, 11, 43, 79, 95, 50, 72, 98, 6},
            {6, 98, 72, 50, 95, 79, 43, 11, 78, 82}
        }
    };
    const size_t TestDataCount = sizeof(TestData) / sizeof(TestData[0]);

    /* Variable */
    int *data;
    uint8_t i, j;
    stack_stack_t stack;

    /*** Stack ***/
    for(i = 0; i < TestDataCount; i++)
    {
        /* Initialize */
        TEST_ASSERT_EQUAL_INT(ECLECTIC_STATUS_SUCCESS, stack_init(&stack, free));

        /* Push */
        for(j = 0; j < LIST_DERIVED_TEST_LIST_COUNT; j++)
        {
            data = malloc(sizeof(*data));
            *data = TestData[i].PushIntegerList[j];
            TEST_ASSERT_EQUAL_INT(ECLECTIC_STATUS_SUCCESS, stack_push(&stack, data));
            TEST_ASSERT_EQUAL_UINT((j + 1), stack_size(&stack));
        }

        /* Destroy */
        TEST_ASSERT_EQUAL_INT(ECLECTIC_STATUS_SUCCESS, stack_destroy(&stack));
        TEST_ASSERT_EQUAL_UINT(0, stack_size(&stack));

        /* Push */
        for(j = 0; j < LIST_DERIVED_TEST_LIST_COUNT; j++)
        {
            data = malloc(sizeof(*data));
            *data = TestData[i].PushIntegerList[j];
            TEST_ASSERT_EQUAL_INT(ECLECTIC_STATUS_SUCCESS, stack_push(&stack, data));
            TEST_ASSERT_EQUAL_UINT((j + 1), stack_size(&stack));
        }

        /* Pop */
        for(j = 0; j < LIST_DERIVED_TEST_LIST_COUNT; j++)
        {
            data = stack_pop(&stack);
            TEST_ASSERT_EQUAL_INT(TestData[i].ExpectedIntegerList[j], *data);
        }

        /* Destroy */
        TEST_ASSERT_EQUAL_INT(ECLECTIC_STATUS_SUCCESS, stack_destroy(&stack));
    }
}
