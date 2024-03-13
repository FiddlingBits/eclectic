/****************************************************************************************************
 * Define
 ****************************************************************************************************/

#define HASH_TABLE_TEST_LIST_COUNT (100)

/****************************************************************************************************
 * Include
 ****************************************************************************************************/

#include "hash_table.h"
#include "list.h"
#include "mock_eclectic.h"
#include <string.h>
#include "unity.h"

/****************************************************************************************************
 * Type Definition
 ****************************************************************************************************/

typedef struct hashTableTest_data_s
{
    int id;
    int data;
} hashTableTest_data_t;

/****************************************************************************************************
 * Helper
 ****************************************************************************************************/

/*** Compare Callback ***/
int hashTableTest_compareCallback(const void * const Data1, const void * const Data2)
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
        actualResult = hashTableTest_compareCallback(&TestData[i].Integer1, &TestData[i].Integer2);
        TEST_ASSERT_EQUAL_INT(TestData[i].ExpectedResult, actualResult);
    }
}

/*** Hash Callback ***/
size_t hashTableTest_hashCallback(const void * const Data)
{
    /*** Hash Callback ***/ 
    return (*(const int *)Data + -(INT_MIN + 1)); // Bring Integer Into Unsigned Integer Range
}

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

/*** Destroy ***/
void test_destroy_error(void)
{
    /*** Destroy ***/
    /* Destroy (NULL Pointer Error) */
    TEST_ASSERT_EQUAL_INT(ECLECTIC_STATUS_ERROR_NULL_POINTER, hashTable_destroy(NULL));
}

void test_destroy_success(void)
{
    /*** Test Data ***/    
    /* Test Data */
    const size_t Buckets = 47; // Load Factor Approximately 100 / 47 = 2.127; 2^5 (32) < 47 < 2^6 (64)
    const int DataList[HASH_TABLE_TEST_LIST_COUNT] = // Not Necessarily Unique
    {
        72, 71, 3, 54, -21, 9, 8, -97, 72, 48, -100, -12, -10, 45, 70, 38, 17, 98, -34, 71,
        72, 86, 20, 32, -39, 2, 89, -24, -10, -82, -74, -82, 7, -87, -88, -5, 97, 70, 13, -85,
        -58, 0, 50, -27, -32, 92, -36, 26, 25, 73, 82, -40, 61, 24, -11, -21, 27, 20, -77, -31,
        -34, 85, 64, 37, 38, -47, 77, 39, 1, -11, -87, -27, 79, -7, -65, 24, -3, -36, 0, -77,
        88, -7, 16, -19, -7, -24, -6, 79, -99, -40, 25, -31, -67, -93, -38, 94, -98, 10, 6, 64
    };
    const int IdList[HASH_TABLE_TEST_LIST_COUNT] = // All Unique
    {
        -764, 989, 478, 941, 66, -321, 367, -444, 425, 428, -854, -338, -79, 476, -806, 263, 905, 84, -898, -335,
        -67, 663, -5, 357, -184, 482, -80, -867, 837, -121, -520, 881, 90, 972, -562, 114, -637, 67, 642, -124,
        -996, 880, 135, -535, -462, 231, -811, -169, 697, -788, -356, -634, -850, -49, -645, -973, 457, 757, -998, 977,
        17, -733, 0, -249, 510, 630, 354, -440, -384, 159, 86, -398, 685, -295, 854, 919, -832, 500, 798, 364,
        -142, 257, 646, -431, -216, 363, 586, 171, 770, -416, 91, 582, -621, -881, -872, -181, -413, 662, -166, 252
    };
    
    /* Variable */
    hashTable_hashTable_t hashTable;
    size_t i;
    hashTableTest_data_t testData[HASH_TABLE_TEST_LIST_COUNT];
    
    /* Set Up */
    for(i = 0; i < HASH_TABLE_TEST_LIST_COUNT; i++)
    {
        testData[i].data = DataList[i];
        testData[i].id = IdList[i];
    }
    
    /*** Destroy ***/
    /* Initialize */
    eclectic_malloc_ExpectAndReturn((Buckets * sizeof(*hashTable.list)), malloc((Buckets * sizeof(*hashTable.list)))); // Mock
    TEST_ASSERT_EQUAL_INT(ECLECTIC_STATUS_SUCCESS, hashTable_init(&hashTable, Buckets, hashTableTest_compareCallback, NULL, hashTableTest_hashCallback));
    
    /* Insert */
    for(i = 0; i < HASH_TABLE_TEST_LIST_COUNT; i++)
    {
        eclectic_malloc_ExpectAndReturn(sizeof(*(hashTable.list->head)), malloc(sizeof(*(hashTable.list->head)))); // Mock
        TEST_ASSERT_EQUAL_INT(ECLECTIC_STATUS_SUCCESS, hashTable_insert(&hashTable, &testData[i]));
    }
    
    /* Destroy */
    eclectic_free_ExpectAnyArgs(); // Mock
    for(i = 0; i < HASH_TABLE_TEST_LIST_COUNT; i++)
        eclectic_free_ExpectAnyArgs(); // Mock
    TEST_ASSERT_EQUAL_INT(ECLECTIC_STATUS_SUCCESS, hashTable_destroy(&hashTable));
}

/*** Find ***/
void test_find_error(void)
{
    /*** Test Data ***/
    /* Variable */
    void *data;
    hashTable_hashTable_t hashTable;
    
    /*** Find ***/
    /* Find (NULL Pointer Error) */
    TEST_ASSERT_FALSE(hashTable_find(NULL, data));
    
    /* Initialize */
    eclectic_malloc_ExpectAndReturn((1 * sizeof(*hashTable.list)), malloc((1 * sizeof(*hashTable.list)))); // Mock
    TEST_ASSERT_EQUAL_INT(ECLECTIC_STATUS_SUCCESS, hashTable_init(&hashTable, 1, hashTableTest_compareCallback, free, hashTableTest_hashCallback));
    
    /* Find (Not Initialized Error) */
    hashTable.hashCallback = NULL;
    TEST_ASSERT_FALSE(hashTable_find(&hashTable, data));
    
    /* Destroy */
    eclectic_free_ExpectAnyArgs(); // Mock
    TEST_ASSERT_EQUAL_INT(ECLECTIC_STATUS_SUCCESS, hashTable_destroy(&hashTable));
}

void test_find_success(void)
{    
    /*** Test Data ***/    
    /* Test Data */
    const size_t Buckets = 47; // Load Factor Approximately 100 / 47 = 2.127; 2^5 (32) < 47 < 2^6 (64)
    const int DataList[HASH_TABLE_TEST_LIST_COUNT] = // Not Necessarily Unique
    {
        72, 71, 3, 54, -21, 9, 8, -97, 72, 48, -100, -12, -10, 45, 70, 38, 17, 98, -34, 71,
        72, 86, 20, 32, -39, 2, 89, -24, -10, -82, -74, -82, 7, -87, -88, -5, 97, 70, 13, -85,
        -58, 0, 50, -27, -32, 92, -36, 26, 25, 73, 82, -40, 61, 24, -11, -21, 27, 20, -77, -31,
        -34, 85, 64, 37, 38, -47, 77, 39, 1, -11, -87, -27, 79, -7, -65, 24, -3, -36, 0, -77,
        88, -7, 16, -19, -7, -24, -6, 79, -99, -40, 25, -31, -67, -93, -38, 94, -98, 10, 6, 64
    };
    const int FindIdList[HASH_TABLE_TEST_LIST_COUNT] = // All Different Than IdList
    {
        228, -913, 962, 302, -995, -414, 621, 912, -200, 25, 195, -506, 55, -947, -852, 829, -633, 897, 464, 778,
        550, 355, 399, 413, 19, 935, -461, 896, 172, 818, -992, -555, 900, -964, 489, 485, 83, -373, -571, 668,
        -769, -843, 619, 576, -182, 844, 647, -490, -81, 562, -497, 648, 7, 541, -636, 326, 986, 399, 68, 584,
        -283, -366, -307, -541, -9, -302, -892, 379, 334, -226, -862, -307, 196, -403, -210, -571, -804, 122, -106, 92,
        85, -519, 32, 268, 835, -217, -72, -463, 3, -177, 30, 440, -496, 799, 764, 448, -215, -355, -745, 210
    };
    const int IdList[HASH_TABLE_TEST_LIST_COUNT] = // All Unique
    {
        -764, 989, 478, 941, 66, -321, 367, -444, 425, 428, -854, -338, -79, 476, -806, 263, 905, 84, -898, -335,
        -67, 663, -5, 357, -184, 482, -80, -867, 837, -121, -520, 881, 90, 972, -562, 114, -637, 67, 642, -124,
        -996, 880, 135, -535, -462, 231, -811, -169, 697, -788, -356, -634, -850, -49, -645, -973, 457, 757, -998, 977,
        17, -733, 0, -249, 510, 630, 354, -440, -384, 159, 86, -398, 685, -295, 854, 919, -832, 500, 798, 364,
        -142, 257, 646, -431, -216, 363, 586, 171, 770, -416, 91, 582, -621, -881, -872, -181, -413, 662, -166, 252
    };
    
    /* Variable */
    hashTable_hashTable_t hashTable;
    size_t i;
    hashTableTest_data_t testData[HASH_TABLE_TEST_LIST_COUNT];
    
    /* Set Up */
    for(i = 0; i < HASH_TABLE_TEST_LIST_COUNT; i++)
    {
        testData[i].data = DataList[i];
        testData[i].id = IdList[i];
    }
    
    /*** Find ***/
    /* Initialize */
    eclectic_malloc_ExpectAndReturn((Buckets * sizeof(*hashTable.list)), malloc((Buckets * sizeof(*hashTable.list)))); // Mock
    TEST_ASSERT_EQUAL_INT(ECLECTIC_STATUS_SUCCESS, hashTable_init(&hashTable, Buckets, hashTableTest_compareCallback, NULL, hashTableTest_hashCallback));
    
    /* Insert */
    for(i = 0; i < HASH_TABLE_TEST_LIST_COUNT; i++)
    {
        eclectic_malloc_ExpectAndReturn(sizeof(*(hashTable.list->head)), malloc(sizeof(*(hashTable.list->head)))); // Mock
        TEST_ASSERT_EQUAL_INT(ECLECTIC_STATUS_SUCCESS, hashTable_insert(&hashTable, &testData[i]));
    }
    
    /* Found */
    for(i = 0; i < HASH_TABLE_TEST_LIST_COUNT; i++)
        TEST_ASSERT_TRUE(hashTable_find(&hashTable, &IdList[i])); // For This To Work Correctly, ID Must Be At Front Of hashTableTest_data_t Because hashTableTest_compareCallback Assumes Data1 And Data2 Point To int
    
    /* Not Found */
    for(i = 0; i < HASH_TABLE_TEST_LIST_COUNT; i++)
        TEST_ASSERT_FALSE(hashTable_find(&hashTable, &FindIdList[i]));
    
    /* Destroy */
    eclectic_free_ExpectAnyArgs(); // Mock
    for(i = 0; i < HASH_TABLE_TEST_LIST_COUNT; i++)
        eclectic_free_ExpectAnyArgs(); // Mock
    TEST_ASSERT_EQUAL_INT(ECLECTIC_STATUS_SUCCESS, hashTable_destroy(&hashTable));
}

/*** Initialize ***/
void test_init_error(void)
{
    /*** Test Data ***/
    /* Variable */
    hashTable_hashTable_t hashTable;
    
    /*** Initialize ***/
    /* Initialize (NULL Pointer Error) */
    TEST_ASSERT_EQUAL_INT(ECLECTIC_STATUS_ERROR_NULL_POINTER, hashTable_init(NULL, 1, hashTableTest_compareCallback, free, hashTableTest_hashCallback));
    TEST_ASSERT_EQUAL_INT(ECLECTIC_STATUS_ERROR_NULL_POINTER, hashTable_init(&hashTable, 1, NULL, free, hashTableTest_hashCallback));
    TEST_ASSERT_EQUAL_INT(ECLECTIC_STATUS_ERROR_NULL_POINTER, hashTable_init(&hashTable, 1, hashTableTest_compareCallback, free, NULL));
    
    /* Initialize (Length Error) */
    TEST_ASSERT_EQUAL_INT(ECLECTIC_STATUS_ERROR_LENGTH, hashTable_init(&hashTable, 0, hashTableTest_compareCallback, free, hashTableTest_hashCallback));
    
    /* Initialize (Memory Allocation Error) */
    eclectic_malloc_ExpectAndReturn((1 * sizeof(*hashTable.list)), NULL); // Mock
    TEST_ASSERT_EQUAL_INT(ECLECTIC_STATUS_ERROR_MEMORY_ALLOCATION, hashTable_init(&hashTable, 1, hashTableTest_compareCallback, free, hashTableTest_hashCallback));
    
    /* Destroy */
    eclectic_free_ExpectAnyArgs(); // Mock
    TEST_ASSERT_EQUAL_INT(ECLECTIC_STATUS_SUCCESS, hashTable_destroy(&hashTable)); // Isn't Necessary As hashTable_init Failed, But Shouldn't Cause Any Problems To Call Anyway
}

void test_init_success(void)
{
    /*** Test Data ***/    
    /* Test Data */
    const size_t Buckets = 47;
    
    /* Variable */
    hashTable_hashTable_t hashTable;
    size_t i;
    void *memory;
    
    /*** Initialize ***/
    /* Initialize */
    memory = malloc(Buckets * sizeof(*hashTable.list));
    eclectic_malloc_ExpectAndReturn((Buckets * sizeof(*hashTable.list)), memory); // Mock
    TEST_ASSERT_EQUAL_INT(ECLECTIC_STATUS_SUCCESS, hashTable_init(&hashTable, Buckets, hashTableTest_compareCallback, NULL, hashTableTest_hashCallback));
    
    /* Verify */
    TEST_ASSERT_EQUAL_UINT(Buckets, hashTable.buckets);
    TEST_ASSERT_EQUAL_PTR(hashTableTest_hashCallback, hashTable.hashCallback);
    TEST_ASSERT_EQUAL_PTR(memory, hashTable.list);
    
    /* Destroy */
    eclectic_free_ExpectAnyArgs(); // Mock
    TEST_ASSERT_EQUAL_INT(ECLECTIC_STATUS_SUCCESS, hashTable_destroy(&hashTable));
}

/*** Insert ***/
void test_insert_error(void)
{
    /*** Test Data ***/
    /* Test Data */
    const size_t Buckets = 1;
    const int TestData = 1234;
    
    /* Variable */
    int data[2];
    hashTable_hashTable_t hashTable;
    
    /* Set Up */
    data[0] = TestData;
    data[1] = TestData;
    
    /*** Insert ***/
    /* Insert (NULL Pointer Error) */
    TEST_ASSERT_EQUAL_INT(ECLECTIC_STATUS_ERROR_NULL_POINTER, hashTable_insert(NULL, &data[0]));
    
    /* Initialize */
    eclectic_malloc_ExpectAndReturn((Buckets * sizeof(*hashTable.list)), malloc((Buckets * sizeof(*hashTable.list)))); // Mock
    TEST_ASSERT_EQUAL_INT(ECLECTIC_STATUS_SUCCESS, hashTable_init(&hashTable, Buckets, hashTableTest_compareCallback, NULL, hashTableTest_hashCallback));
    
    /* Insert (Not Initialized Error) */
    hashTable.hashCallback = NULL;
    TEST_ASSERT_EQUAL_INT(ECLECTIC_STATUS_ERROR_NOT_INITIALIZED, hashTable_insert(&hashTable, &data[0]));
    
    /* Destroy */
    eclectic_free_ExpectAnyArgs(); // Mock
    TEST_ASSERT_EQUAL_INT(ECLECTIC_STATUS_SUCCESS, hashTable_destroy(&hashTable));
    
    /* Initialize */
    eclectic_malloc_ExpectAndReturn((Buckets * sizeof(*hashTable.list)), malloc((Buckets * sizeof(*hashTable.list)))); // Mock
    TEST_ASSERT_EQUAL_INT(ECLECTIC_STATUS_SUCCESS, hashTable_init(&hashTable, Buckets, hashTableTest_compareCallback, NULL, hashTableTest_hashCallback));
    
    /* Insert (Duplicate Error) */
    eclectic_malloc_ExpectAndReturn(sizeof(*(hashTable.list->head)), malloc(sizeof(*(hashTable.list->head)))); // Mock
    TEST_ASSERT_EQUAL_INT(ECLECTIC_STATUS_SUCCESS, hashTable_insert(&hashTable, &data[0]));
    TEST_ASSERT_EQUAL_INT(ECLECTIC_STATUS_ERROR_DUPLICATE, hashTable_insert(&hashTable, &data[1]));
    
    /* Destroy */
    eclectic_free_ExpectAnyArgs(); // Mock
    eclectic_free_ExpectAnyArgs(); // Mock
    TEST_ASSERT_EQUAL_INT(ECLECTIC_STATUS_SUCCESS, hashTable_destroy(&hashTable));
    
    /* Initialize */
    eclectic_malloc_ExpectAndReturn((Buckets * sizeof(*hashTable.list)), malloc((Buckets * sizeof(*hashTable.list)))); // Mock
    TEST_ASSERT_EQUAL_INT(ECLECTIC_STATUS_SUCCESS, hashTable_init(&hashTable, Buckets, hashTableTest_compareCallback, NULL, hashTableTest_hashCallback));
    
    /* Insert (Memory Allocation Error) */
    eclectic_malloc_ExpectAndReturn(sizeof(*(hashTable.list->head)), NULL); // Mock
    TEST_ASSERT_EQUAL_INT(ECLECTIC_STATUS_ERROR_MEMORY_ALLOCATION, hashTable_insert(&hashTable, &data[0]));
    
    /* Destroy */
    eclectic_free_ExpectAnyArgs(); // Mock
    TEST_ASSERT_EQUAL_INT(ECLECTIC_STATUS_SUCCESS, hashTable_destroy(&hashTable));
}

void test_insert_success(void)
{
    /*** Test Data ***/
    /* Test Data */
    const int TestData = 1234;
    
    /* Variable */
    int data, *dataPointer;
    hashTable_hashTable_t hashTable;
    
    /*** Remove ***/
    /* Initialize */
    eclectic_malloc_ExpectAndReturn((1 * sizeof(*hashTable.list)), malloc((1 * sizeof(*hashTable.list)))); // Mock
    TEST_ASSERT_EQUAL_INT(ECLECTIC_STATUS_SUCCESS, hashTable_init(&hashTable, 1, hashTableTest_compareCallback, NULL, hashTableTest_hashCallback));
    
    /* Insert */
    eclectic_malloc_ExpectAndReturn(sizeof(*(hashTable.list->head)), malloc(sizeof(*(hashTable.list->head)))); // Mock
    data = TestData;
    TEST_ASSERT_EQUAL_INT(ECLECTIC_STATUS_SUCCESS, hashTable_insert(&hashTable, &data));
    
    /* Size */
    TEST_ASSERT_EQUAL_UINT(1, hashTable_size(&hashTable));
    
    /* Remove */
    eclectic_free_ExpectAnyArgs(); // Mock
    dataPointer = &data;
    TEST_ASSERT_EQUAL_INT(ECLECTIC_STATUS_SUCCESS, hashTable_remove(&hashTable, (void **)&dataPointer));
    TEST_ASSERT_EQUAL_UINT(0, hashTable_size(&hashTable));
    TEST_ASSERT_EQUAL_PTR(&data, dataPointer);
    TEST_ASSERT_EQUAL_INT(TestData, *dataPointer);
    
    /* Destroy */
    eclectic_free_ExpectAnyArgs(); // Mock
    TEST_ASSERT_EQUAL_INT(ECLECTIC_STATUS_SUCCESS, hashTable_destroy(&hashTable));
}

/*** Remove ***/
void test_remove_error(void)
{
    /*** Test Data ***/
    /* Test Data */
    const int TestData = 1234;
    
    /* Variable */
    int data[2], *dataPointer;
    hashTable_hashTable_t hashTable;
    
    /*** Remove ***/
    /* Remove (NULL Pointer Error) */
    TEST_ASSERT_EQUAL_INT(ECLECTIC_STATUS_ERROR_NULL_POINTER, hashTable_remove(NULL, (void **)&dataPointer));
    TEST_ASSERT_EQUAL_INT(ECLECTIC_STATUS_ERROR_NULL_POINTER, hashTable_remove(&hashTable, NULL));
    
    /* Initialize */
    eclectic_malloc_ExpectAndReturn((1 * sizeof(*hashTable.list)), malloc((1 * sizeof(*hashTable.list)))); // Mock
    TEST_ASSERT_EQUAL_INT(ECLECTIC_STATUS_SUCCESS, hashTable_init(&hashTable, 1, hashTableTest_compareCallback, NULL, hashTableTest_hashCallback));
    
    /* Remove (Not Initialized) */
    hashTable.hashCallback = NULL;
    TEST_ASSERT_EQUAL_INT(ECLECTIC_STATUS_ERROR_NOT_INITIALIZED, hashTable_remove(&hashTable, (void **)&dataPointer));
    eclectic_free_ExpectAnyArgs(); // Mock
    
    /* Destroy */
    TEST_ASSERT_EQUAL_INT(ECLECTIC_STATUS_SUCCESS, hashTable_destroy(&hashTable));
    
    /* Initialize */
    eclectic_malloc_ExpectAndReturn((1 * sizeof(*hashTable.list)), malloc((1 * sizeof(*hashTable.list)))); // Mock
    TEST_ASSERT_EQUAL_INT(ECLECTIC_STATUS_SUCCESS, hashTable_init(&hashTable, 1, hashTableTest_compareCallback, NULL, hashTableTest_hashCallback));
    eclectic_malloc_ExpectAndReturn(sizeof(*(hashTable.list->head)), malloc(sizeof(*(hashTable.list->head)))); // Mock
    
    /* Insert */
    data[0] = TestData;
    TEST_ASSERT_EQUAL_INT(ECLECTIC_STATUS_SUCCESS, hashTable_insert(&hashTable, &data[0]));
    
    /* Remove (Not Found Error) */
    data[1] = TestData + 1;
    dataPointer = &data[1];
    TEST_ASSERT_EQUAL_INT(ECLECTIC_STATUS_ERROR_NOT_FOUND, hashTable_remove(&hashTable, (void **)&dataPointer));
    
    /* Destroy */
    eclectic_free_ExpectAnyArgs(); // Mock
    eclectic_free_ExpectAnyArgs(); // Mock
    TEST_ASSERT_EQUAL_INT(ECLECTIC_STATUS_SUCCESS, hashTable_destroy(&hashTable));
}

void test_remove_success(void)
{
    /*** Test Data ***/
    /* Test Data */
    const int TestData = 1234;
    
    /* Variable */
    int data, *dataPointer;
    hashTable_hashTable_t hashTable;
    
    /*** Remove ***/
    /* Initialize */
    eclectic_malloc_ExpectAndReturn((1 * sizeof(*hashTable.list)), malloc((1 * sizeof(*hashTable.list)))); // Mock
    TEST_ASSERT_EQUAL_INT(ECLECTIC_STATUS_SUCCESS, hashTable_init(&hashTable, 1, hashTableTest_compareCallback, NULL, hashTableTest_hashCallback));
    
    /* Insert */
    eclectic_malloc_ExpectAndReturn(sizeof(*(hashTable.list->head)), malloc(sizeof(*(hashTable.list->head)))); // Mock
    data = TestData;
    dataPointer = &data;
    TEST_ASSERT_EQUAL_INT(ECLECTIC_STATUS_SUCCESS, hashTable_insert(&hashTable, &data));
    
    /* Size */
    TEST_ASSERT_EQUAL_UINT(1, hashTable_size(&hashTable));
    
    /* Remove */
    eclectic_free_ExpectAnyArgs(); // Mock
    TEST_ASSERT_EQUAL_INT(ECLECTIC_STATUS_SUCCESS, hashTable_remove(&hashTable, (void **)&dataPointer));
    TEST_ASSERT_EQUAL_UINT(0, hashTable_size(&hashTable));
    TEST_ASSERT_EQUAL_PTR(&data, dataPointer);
    TEST_ASSERT_EQUAL_INT(TestData, *dataPointer);
    
    /* Destroy */
    eclectic_free_ExpectAnyArgs(); // Mock
    TEST_ASSERT_EQUAL_INT(ECLECTIC_STATUS_SUCCESS, hashTable_destroy(&hashTable));
}

/*** Size ***/
void test_size_error(void)
{
    /*** Size ***/
    /* Size (NULL Pointer Error) */
    TEST_ASSERT_EQUAL_UINT(0, hashTable_size(NULL));
}

void test_size_success(void)
{
    /*** Test Data ***/    
    /* Test Data */
    const size_t Buckets = 47; // Load Factor Approximately 100 / 47 = 2.127; 2^5 (32) < 47 < 2^6 (64)
    const int DataList[HASH_TABLE_TEST_LIST_COUNT] = // Not Necessarily Unique
    {
        72, 71, 3, 54, -21, 9, 8, -97, 72, 48, -100, -12, -10, 45, 70, 38, 17, 98, -34, 71,
        72, 86, 20, 32, -39, 2, 89, -24, -10, -82, -74, -82, 7, -87, -88, -5, 97, 70, 13, -85,
        -58, 0, 50, -27, -32, 92, -36, 26, 25, 73, 82, -40, 61, 24, -11, -21, 27, 20, -77, -31,
        -34, 85, 64, 37, 38, -47, 77, 39, 1, -11, -87, -27, 79, -7, -65, 24, -3, -36, 0, -77,
        88, -7, 16, -19, -7, -24, -6, 79, -99, -40, 25, -31, -67, -93, -38, 94, -98, 10, 6, 64
    };
    const int IdList[HASH_TABLE_TEST_LIST_COUNT] = // All Unique
    {
        -764, 989, 478, 941, 66, -321, 367, -444, 425, 428, -854, -338, -79, 476, -806, 263, 905, 84, -898, -335,
        -67, 663, -5, 357, -184, 482, -80, -867, 837, -121, -520, 881, 90, 972, -562, 114, -637, 67, 642, -124,
        -996, 880, 135, -535, -462, 231, -811, -169, 697, -788, -356, -634, -850, -49, -645, -973, 457, 757, -998, 977,
        17, -733, 0, -249, 510, 630, 354, -440, -384, 159, 86, -398, 685, -295, 854, 919, -832, 500, 798, 364,
        -142, 257, 646, -431, -216, 363, 586, 171, 770, -416, 91, 582, -621, -881, -872, -181, -413, 662, -166, 252
    };
    
    /* Variable */
    hashTable_hashTable_t hashTable;
    size_t i;
    hashTableTest_data_t testData[HASH_TABLE_TEST_LIST_COUNT];
    
    /* Set Up */
    for(i = 0; i < HASH_TABLE_TEST_LIST_COUNT; i++)
    {
        testData[i].data = DataList[i];
        testData[i].id = IdList[i];
    }
    
    /*** Size ***/
    /* Initialize */
    eclectic_malloc_ExpectAndReturn((Buckets * sizeof(*hashTable.list)), malloc((Buckets * sizeof(*hashTable.list)))); // Mock
    TEST_ASSERT_EQUAL_INT(ECLECTIC_STATUS_SUCCESS, hashTable_init(&hashTable, Buckets, hashTableTest_compareCallback, NULL, hashTableTest_hashCallback));
    
    /* Insert */
    for(i = 0; i < HASH_TABLE_TEST_LIST_COUNT; i++)
    {
        /* Insert */
        eclectic_malloc_ExpectAndReturn(sizeof(*(hashTable.list->head)), malloc(sizeof(*(hashTable.list->head)))); // Mock
        TEST_ASSERT_EQUAL_INT(ECLECTIC_STATUS_SUCCESS, hashTable_insert(&hashTable, &testData[i]));
        
        /* Size */
        TEST_ASSERT_EQUAL_UINT((i + 1), hashTable_size(&hashTable));
    }
    TEST_ASSERT_EQUAL_UINT(HASH_TABLE_TEST_LIST_COUNT, hashTable_size(&hashTable)); // Already Tested Above, But Added So Final Count Is Explicit
    
    /* Destroy */
    eclectic_free_ExpectAnyArgs(); // Mock
    for(i = 0; i < HASH_TABLE_TEST_LIST_COUNT; i++)
        eclectic_free_ExpectAnyArgs(); // Mock
    TEST_ASSERT_EQUAL_INT(ECLECTIC_STATUS_SUCCESS, hashTable_destroy(&hashTable));
}
