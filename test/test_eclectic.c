/****************************************************************************************************
 * Type Definition
 ****************************************************************************************************/

#include "eclectic.h"
#include <stdlib.h>
#include "unity.h"

/****************************************************************************************************
 * Variable
 ****************************************************************************************************/

/*** External ***/
extern eclectic_freeCallback_t eclectic_freeCallback;
extern eclectic_mallocCallback_t eclectic_mallocCallback;

/****************************************************************************************************
 * Set Up/Tear Down
 ****************************************************************************************************/

void setUp(void)
{
}

void tearDown(void)
{
}

/****************************************************************************************************
 * Test (Public)
 ****************************************************************************************************/

/*** Free ***/
void test_free_error(void)
{
    /*** Test Data ***/
    /* Variable */
    void *data;
    
    /* Set Up */
    eclectic_init(NULL, malloc);
    
    /*** Free ***/
    /* NULL Pointer */
    eclectic_free(NULL);
    eclectic_free((void **)&data); // eclectic_freeCallback Is NULL
}

void test_free_success(void)
{
    /*** Test Data ***/
    /* Variable */
    int *integer;
    
    /* Set Up */
    eclectic_init(free, malloc);
    integer = eclectic_malloc(sizeof(*integer));
    TEST_ASSERT_NOT_NULL(integer);
    
    /*** Free ***/
    eclectic_free((void **)&integer);
    TEST_ASSERT_NULL(integer);
}

/*** Initialize ***/
void test_init_success(void)
{
    /*** Initialize ***/
    /* Initialize */
    eclectic_init(free, malloc);
    
    /* Verify */
    TEST_ASSERT_EQUAL_PTR(free, eclectic_freeCallback);
    TEST_ASSERT_EQUAL_PTR(malloc, eclectic_mallocCallback);
}

/*** Malloc ***/
void test_malloc_error(void)
{
    /*** Test Data ***/
    /* Variable */
    void *data;
    
    /*** Free ***/
    /* Set Up */
    eclectic_init(free, NULL);
    
    /* NULL Pointer */
    data = eclectic_malloc(1); // eclectic_mallocCallback_t Is NULL
    TEST_ASSERT_NULL(data);
    
    /* Set Up */
    eclectic_init(free, malloc);
    
    /* Length */
    data = eclectic_malloc(0);
    TEST_ASSERT_NULL(data);
}

void test_malloc_success(void)
{
    /*** Test Data ***/
    /* Variable */
    int *integer;
    
    /* Set Up */
    eclectic_init(free, malloc);
    
    /*** Malloc ***/
    /* Malloc */
    integer = eclectic_malloc(sizeof(*integer));
    TEST_ASSERT_NOT_NULL(integer);
    
    /* Clean Up */
    eclectic_free((void **)&integer);
}