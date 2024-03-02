/****************************************************************************************************
 * Include
 ****************************************************************************************************/

#include "cli_helper.h"

/****************************************************************************************************
 * Function Definition (Public)
 ****************************************************************************************************/

/*** Get Comma Separated Value List ***/
eclectic_status_t cliHelper_getCommaSeparatedValueList(char * const input, const uint8_t MaxListCount, char *list[], uint8_t * const actualListCount)
{
    bool found = false;
    uint8_t i;
    size_t inputLength;

    /*** Error Check ***/
    /* NULL Pointer */
    if((input == NULL) || (list == NULL) || (actualListCount == NULL))
        return ECLECTIC_STATUS_ERROR_NULL_POINTER;

    /* Length */
    if(strlen(input) == 0)
        return ECLECTIC_STATUS_ERROR_LENGTH;

    /*** Set Up ***/
    for(i = 0; i < MaxListCount; i++)
        list[i] = NULL;
    *actualListCount = 0;

    /*** Get Comma Separated Value List ***/
    inputLength = strlen(input); // Can't Use strlen In for Loop Because '\0's Are Written To input
    for(i = 0; i < inputLength; i++)
    {
        if(input[i] != ',')
        {
            if(!found)
            {
                /* Start Of Comma Separated Value */
                found = true;
                list[(*actualListCount)++] = &input[i];
            }
        }
        else
        {
            /* End Of Comma Separated Value */
            found = false;
            input[i] = '\0'; // Set To NUL So strlen Works On Comma Separated Values
            if(*actualListCount >= MaxListCount)
                break;
        }
    }

    return ECLECTIC_STATUS_SUCCESS;
}

/*** Get Double ***/
eclectic_status_t cliHelper_getDouble(const char * const Input, double * const d)
{
    char *endCharacter;

    /*** Pre-Conversion Error Check ***/
    /* NULL Pointer */
    if((Input == NULL) || (d == NULL))
        return ECLECTIC_STATUS_ERROR_NULL_POINTER;

    /* Length */
    if(strlen(Input) == 0)
        return ECLECTIC_STATUS_ERROR_LENGTH;

    /*** Get Double ***/
    *d = strtod(Input, &endCharacter);

    /*** Post-Conversion Error Check ***/
    if(!isspace(*endCharacter) && isprint(*endCharacter))
        return ECLECTIC_STATUS_ERROR_INVALID;
    else
        return ECLECTIC_STATUS_SUCCESS;
}

/*** Get Float ***/
eclectic_status_t cliHelper_getFloat(const char * const Input, float * const f)
{
    char *endCharacter;

    /*** Pre-Conversion Error Check ***/
    /* NULL Pointer */
    if((Input == NULL) || (f == NULL))
        return ECLECTIC_STATUS_ERROR_NULL_POINTER;

    /* Length */
    if(strlen(Input) == 0)
        return ECLECTIC_STATUS_ERROR_LENGTH;

    /*** Get Float ***/
    *f = strtof(Input, &endCharacter);

    /*** Post-Conversion Error Check ***/
    if(!isspace(*endCharacter) && isprint(*endCharacter))
        return ECLECTIC_STATUS_ERROR_INVALID;
    else
        return ECLECTIC_STATUS_SUCCESS;
}

/*** Get Option/Argument Pair ***/
eclectic_status_t cliHelper_getOptionArgumentPair(char * const input, cliHelper_optionArgumentPair_t * const optionArgumentPair)
{
    uint8_t i;
    eclectic_status_t status = ECLECTIC_STATUS_SUCCESS;

    /*** Error Check ***/
    /* NULL Pointer */
    if((input == NULL) || (optionArgumentPair == NULL))
        return ECLECTIC_STATUS_ERROR_NULL_POINTER;

    /* Length */
    if(strlen(input) < 2)
        return ECLECTIC_STATUS_ERROR_LENGTH;

    /*** Set Up ***/
    optionArgumentPair->option = NULL;
    optionArgumentPair->argument = NULL;

    /*** Get Option/Argument Pair ***/
    if(input[0] == '-')
    {
        if(input[1] == '-')
        {
            /*** Long Option ***/
            if(isprint(input[2]) && (input[2] != '='))
            {
                /* Set Option */
                optionArgumentPair->option = &input[2]; // Move Past "--"

                /* Set Argument, If Present */
                for(i = 3; i < strlen(input); i++)
                {
                    if(input[i] == '=')
                    {
                        input[i] = '\0'; // Overwrite '=' To NUL Terminate Option
                        if(input[i + 1] != '\0')
                            optionArgumentPair->argument = &input[i + 1];
                        break;
                    }
                }
            }
            else
            {
                status = ECLECTIC_STATUS_ERROR_INVALID;
            }
        }
        else
        {
            /*** Short Option ***/
            /* Set Option */
            input[0] = input[1]; // Overwrite '-' With Option
            input[1] = '\0'; // NUL Terminate
            optionArgumentPair->option = &input[0];

            /* Set Argument, If Present */
            if(input[2] != '\0')
                optionArgumentPair->argument = &input[2];
        }
    }
    else
    {
        status = ECLECTIC_STATUS_ERROR_NO_ACTION;
    }

    return status;
}

/*** Get Signed 32-Bit Integer ***/
eclectic_status_t cliHelper_getSigned32BitInteger(const char * const Input, int32_t * const s32)
{
    char *endCharacter;

    /*** Pre-Conversion Error Check ***/
    /* NULL Pointer */
    if((Input == NULL) || (s32 == NULL))
        return ECLECTIC_STATUS_ERROR_NULL_POINTER;

    /* Length */
    if(strlen(Input) == 0)
        return ECLECTIC_STATUS_ERROR_LENGTH;

    /*** Get Signed 32-Bit Integer ***/
    *s32 = (int32_t)strtol(Input, &endCharacter, 0);

    /*** Post-Conversion Error Check ***/
    if(!isspace(*endCharacter) && isprint(*endCharacter))
        return ECLECTIC_STATUS_ERROR_INVALID;
    else
        return ECLECTIC_STATUS_SUCCESS;
}

/*** Get Unsigned 32-Bit Integer ***/
eclectic_status_t cliHelper_getUnsigned32BitInteger(const char * const Input, uint32_t * const u32)
{
    char *endCharacter;

    /*** Pre-Conversion Error Check ***/
    /* NULL Pointer */
    if((Input == NULL) || (u32 == NULL))
        return ECLECTIC_STATUS_ERROR_NULL_POINTER;

    /* Length */
    if(strlen(Input) == 0)
        return ECLECTIC_STATUS_ERROR_LENGTH;

    /*** Get Unsigned 32-Bit Integer ***/
    *u32 = (uint32_t)strtoul(Input, &endCharacter, 0);

    /*** Post-Conversion Error Check ***/
    if(!isspace(*endCharacter) && isprint(*endCharacter))
        return ECLECTIC_STATUS_ERROR_INVALID;
    else
        return ECLECTIC_STATUS_SUCCESS;
}
