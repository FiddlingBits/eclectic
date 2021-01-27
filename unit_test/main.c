/****************************************************************************************************
 * FILE:    main.c
 * BRIEF:   Unit Test Main Source File
 ****************************************************************************************************/

/****************************************************************************************************
 * Includes
 ****************************************************************************************************/

#include <stdio.h>
#include "unity.h"
#include "unity_fixture.h"

/****************************************************************************************************
 * Function Prototypes
 ****************************************************************************************************/

static void main_runTestGroups(void);

/****************************************************************************************************
 * Function Definitions (Public)
 ****************************************************************************************************/

/****************************************************************************************************
 * FUNCT:   main
 * BRIEF:   Unit Test Main Entry
 * RETURN:  int: Success Or Failure Status
 * ARG:     argc: Number Of Arguments
 * ARG:     argv: Argument String List
 * NOTE:    Unity Command Line Options:
 *              -g [group]: Run Tests With Substring [group] In Group Name
 *              -n [test]: Run Tests With Substring [test] In Test Name
 *              -r [count]: Repeat Test Run [count] Number Of Times
 *              -v: Verbose Output (Announce Each Test Before It Runs)
 ****************************************************************************************************/
int main(int argc, const char *argv[])
{
    /*** Start Unity Unit Test Framework ***/
    printf("Unity Version: %u.%u.%u\n", UNITY_VERSION_MAJOR, UNITY_VERSION_MINOR, UNITY_VERSION_BUILD);
    return UnityMain(argc, argv, main_runTestGroups);
}

/****************************************************************************************************
 * Function Definitions (Private)
 ****************************************************************************************************/

/****************************************************************************************************
 * FUNCT:   main_runTestGroups
 * BRIEF:   Run Test Groups
 * RETURN:  void: Returns Nothing
 * ARG:     void: No Arguments
 ****************************************************************************************************/
static void main_runTestGroups(void)
{
    /*** Run Test Groups ***/
    RUN_TEST_GROUP(template_test)
}
