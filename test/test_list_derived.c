/****************************************************************************************************
 * Type Definition
 ****************************************************************************************************/

#include "eclectic.h"
#include "list.h"
#include "queue.h"
#include "stack.h"
#include <string.h>
#include "unity.h"

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
    /* Define */
    #define MAX_LIST_COUNT (5)

    /* Type Definition */
    typedef struct testData_s
    {
        const int EnqueueIntegerList[MAX_LIST_COUNT];
        const int ExpectedIntegerList[MAX_LIST_COUNT];
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
            {-24, -63, -59, -64, -85}
        },
        {
            {24, 63, -59, 64, -85},
            {24, 63, -59, 64, -85}
        },
        {
            {24, 63, 59, 64, 85},
            {24, 63, 59, 64, 85}
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
        for(j = 0; j < MAX_LIST_COUNT; j++)
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
        for(j = 0; j < MAX_LIST_COUNT; j++)
        {
            data = malloc(sizeof(*data));
            *data = TestData[i].EnqueueIntegerList[j];
            TEST_ASSERT_EQUAL_INT(ECLECTIC_STATUS_SUCCESS, queue_enqueue(&queue, data));
            TEST_ASSERT_EQUAL_UINT((j + 1), queue_size(&queue));
        }

        /* Dequeue */
        for(j = 0; j < MAX_LIST_COUNT; j++)
        {
            data = queue_dequeue(&queue);
            TEST_ASSERT_EQUAL_INT(TestData[i].ExpectedIntegerList[j], *data);
        }

        /* Clean Up */
        TEST_ASSERT_EQUAL_INT(ECLECTIC_STATUS_SUCCESS, queue_destroy(&queue));
    }

    /*** Clean Up ***/
    #undef MAX_LIST_COUNT
}

/*** Stack ***/
void test_stack(void)
{
   /*** Test Data ***/
    /* Define */
    #define MAX_LIST_COUNT (5)

    /* Type Definition */
    typedef struct testData_s
    {
        const int PushIntegerList[MAX_LIST_COUNT];
        const int ExpectedIntegerList[MAX_LIST_COUNT];
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
            {-85, -64, -59, -63, -24}
        },
        {
            {24, 63, -59, 64, -85},
            {-85, 64, -59, 63, 24}
        },
        {
            {24, 63, 59, 64, 85},
            {85, 64, 59, 63, 24}
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
        for(j = 0; j < MAX_LIST_COUNT; j++)
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
        for(j = 0; j < MAX_LIST_COUNT; j++)
        {
            data = malloc(sizeof(*data));
            *data = TestData[i].PushIntegerList[j];
            TEST_ASSERT_EQUAL_INT(ECLECTIC_STATUS_SUCCESS, stack_push(&stack, data));
            TEST_ASSERT_EQUAL_UINT((j + 1), stack_size(&stack));
        }

        /* Pop */
        for(j = 0; j < MAX_LIST_COUNT; j++)
        {
            data = stack_pop(&stack);
            TEST_ASSERT_EQUAL_INT(TestData[i].ExpectedIntegerList[j], *data);
        }

        /* Clean Up */
        TEST_ASSERT_EQUAL_INT(ECLECTIC_STATUS_SUCCESS, stack_destroy(&stack));
    }

    /*** Clean Up ***/
    #undef MAX_LIST_COUNT
}
