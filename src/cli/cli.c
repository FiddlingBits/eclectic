/****************************************************************************************************
 * Include
 ****************************************************************************************************/

#include "cli.h"

/****************************************************************************************************
 * Variable
 ****************************************************************************************************/

ECLECTIC_STATIC(cli_alertProcessInputCallback_t cli_alertProcessInputCallback);
ECLECTIC_STATIC(const char *cli_commandPrompt);
static cli_record_t cli_helpCommandRecord;
ECLECTIC_STATIC(char cli_inputBuffer[CLI_CONFIG_MAXIMUM_INPUT_BUFFER_LENGTH]);
ECLECTIC_STATIC(uint16_t cli_inputBufferIndex);
static cli_record_t cli_listCommandRecord;
ECLECTIC_STATIC(cli_printfCallback_t cli_printfCallback);
ECLECTIC_STATIC(cli_record_t *cli_recordList);

/****************************************************************************************************
 * Function Prototype
 ****************************************************************************************************/

ECLECTIC_STATIC(void cli_addRecord(cli_record_t * const newRecord));
ECLECTIC_STATIC(void cli_helpCommandHandlerCallback(uint8_t argc, char *argv[]));
ECLECTIC_STATIC(void cli_listCommandHandlerCallback(uint8_t argc, char *argv[]));
ECLECTIC_STATIC(eclectic_status_t cli_getCommandAndArgumentList(char * const input, const uint8_t MaxArgumentListCount, char **command, uint8_t * const argumentListCount, char *argumentList[]));
ECLECTIC_STATIC(eclectic_status_t cli_verifyRecordNameAcceptable(const char * const Name));

/****************************************************************************************************
 * Function Definition (Public)
 ****************************************************************************************************/

/*** Add Input Character ***/
eclectic_status_t cli_addInputCharacter(const char Character)
{
    /*** Error Check ***/
    if(cli_alertProcessInputCallback == NULL)
        return ECLECTIC_STATUS_ERROR_NOT_INITIALIZED;
    
    /*** Add Input Character ***/
    if(Character == '\b')
    {
        /* Backspace Character */
        if(cli_inputBufferIndex != 0)
            cli_inputBufferIndex--;
    }
    else if((Character == '\n') || (cli_inputBufferIndex == (sizeof(cli_inputBuffer) - 1)))
    {
        /* New Line Character Or Input Buffer Full */
        cli_inputBuffer[cli_inputBufferIndex++] = '\0'; // Replace Newline With NUL Character
    }
    else
    {
        /* Other Characters */
        cli_inputBuffer[cli_inputBufferIndex++] = Character;
    }

    /*** Send Alert To Process Input If Newline Character ('\n') Inputted Or Input Buffer Full ***/
    if((Character == '\n') || (cli_inputBufferIndex == sizeof(cli_inputBuffer)))
        cli_alertProcessInputCallback();
    
    return ECLECTIC_STATUS_SUCCESS;
}

/*** Initialize ***/
eclectic_status_t cli_init(const cli_alertProcessInputCallback_t AlertProcessInputCallback, const cli_printfCallback_t PrintfCallback, const char * const CommandPrompt)
{
    /*** Error Check ***/
    if((AlertProcessInputCallback == NULL) || (PrintfCallback == NULL) || (CommandPrompt == NULL))
        return ECLECTIC_STATUS_ERROR_NULL_POINTER;
    
    /*** Initialize ***/
    /* Variables */
    cli_alertProcessInputCallback = AlertProcessInputCallback;
    cli_commandPrompt = CommandPrompt;
    cli_inputBufferIndex = 0;
    cli_printfCallback = PrintfCallback;
    cli_recordList = NULL;
    
    /* Built In Commands */
    (void)cli_registerCommand(&cli_helpCommandRecord, CLI_CONFIG_BUILT_IN_COMMAND_HELP_NAME, cli_helpCommandHandlerCallback);
    (void)cli_registerCommand(&cli_listCommandRecord, CLI_CONFIG_BUILT_IN_COMMAND_LIST_NAME, cli_listCommandHandlerCallback);
    
    /*** Print Command Prompt ***/
    cli_printfCallback(true, "%s", cli_commandPrompt);
    
    return ECLECTIC_STATUS_SUCCESS;
}

/*** Process Input ***/
eclectic_status_t cli_processInput(void)
{
    char *argumentList[CLI_CONFIG_MAXIMUM_ARGUMENT_LIST_COUNT], *command;
    uint8_t argumentListCount = 0;
    cli_record_t *record;
    
    /*** Error Check ***/
    if(cli_printfCallback == NULL)
        return ECLECTIC_STATUS_ERROR_NOT_INITIALIZED;

    /*** Process Input ***/
    /* Get Command And Argument List */
    if(cli_getCommandAndArgumentList(cli_inputBuffer, CLI_CONFIG_MAXIMUM_ARGUMENT_LIST_COUNT, &command, &argumentListCount, argumentList) == ECLECTIC_STATUS_SUCCESS)
    {
        /* Find Command Record */
        record = cli_recordList;
        while(record != NULL)
        {
            if(strcmp(command, record->name) == 0)
                break;
            record = record->next;
        }
        
        /* Process Command */
        if(record != NULL)
            record->commandHandlerCallback(argumentListCount, argumentList);
    }
    
    /*** Reset ***/
    cli_inputBufferIndex = 0;

    /*** Print Command Prompt ***/
    cli_printfCallback(true, "%s", cli_commandPrompt);

    return ECLECTIC_STATUS_SUCCESS;
}

/*** Register Command ***/
eclectic_status_t cli_registerCommand(cli_record_t * const record, const char * const Name, const cli_commandHandlerCallback_t CommandHandlerCallback)
{
    eclectic_status_t status;
    
    /*** Error Check ***/
    /* NULL Pointer */
    if((record == NULL) || (Name == NULL) || (CommandHandlerCallback == NULL))
        return ECLECTIC_STATUS_ERROR_NULL_POINTER;

    /* Name */
    if((status = cli_verifyRecordNameAcceptable(Name)) != ECLECTIC_STATUS_SUCCESS)
        return status;
    
    /*** Preset Record ***/
    record->name = Name;
    record->prev = NULL;
    record->next = NULL;
    record->commandHandlerCallback = CommandHandlerCallback;

    /*** Add Record ***/
    cli_addRecord(record);
    
    return ECLECTIC_STATUS_SUCCESS;
}

/****************************************************************************************************
 * Function Definition (Private)
 ****************************************************************************************************/

/*** Add Record ***/
ECLECTIC_STATIC(void cli_addRecord(cli_record_t * const newRecord))
{
    cli_record_t *existingRecord;
    
    /*** Add Record ***/
    if(cli_recordList == NULL)
    {
        /*** New Record Is Only Record ***/
        cli_recordList = newRecord;
    }
    else if(strcmp(newRecord->name, cli_recordList->name) < 0)
    {
        /*** New Record Is First Record ***/
        newRecord->next = cli_recordList;
        cli_recordList->prev = newRecord;
        cli_recordList = newRecord;
    }
    else
    {
        /*** New Record Is Middle Or Last Record ***/
        /* Find Existing Record That Is Right After New Record */
        existingRecord = cli_recordList;
        while((existingRecord->next != NULL) && (strcmp(newRecord->name, existingRecord->next->name) > 0))
            existingRecord = existingRecord->next;
        
        /* Set New Record Links */
        newRecord->next = existingRecord->next;
        newRecord->prev = existingRecord;
        
        /* Update Existing Record Links */
        existingRecord->next = newRecord;
        if(newRecord->next != NULL)
            newRecord->next->prev = newRecord;
    }
}

/*** Get Command And Argument List ***/
ECLECTIC_STATIC(eclectic_status_t cli_getCommandAndArgumentList(char * const input, const uint8_t MaxArgumentListCount, char **command, uint8_t * const argumentListCount, char *argumentList[]))
{
    bool found = false;
    uint16_t i;
    
    /*** Error Check ***/
    if((input == NULL) || (command == NULL) || (argumentListCount == NULL) || (argumentList == NULL))
        return ECLECTIC_STATUS_ERROR_NULL_POINTER;
    
    /*** Set Up ***/
    *command = NULL;
    *argumentListCount = 0;
    for(i = 0; i < MaxArgumentListCount; i++)
        argumentList[i] = NULL;
    
    /*** Find Start and End of Command ***/
    for(i = 0; i < strlen(input); i++)
    {
        if(isalpha(input[i]) && (*command == NULL))
            *command = &input[i];
        else if(!isalpha(input[i]) && (*command != NULL))
            break;
    }

    /*** Error Check ***/
    if(*command == NULL)
        return ECLECTIC_STATUS_ERROR_INVALID;
    
    /*** Find Command Arguments ***/
    while(input[i] != '\0') // Can't Use strlen Because input Is Modified
    {
        if(!isspace(input[i]))
        {
            if(!found)
            {
                found = true;
                argumentList[(*argumentListCount)++] = &input[i];
            }
        }
        else
        {
            found = false;
            input[i] = '\0'; // Set To NUL So strlen Works On argumentList Arguments
            if(*argumentListCount == MaxArgumentListCount)
                break;
        }
        i++;
    }
    
    return ECLECTIC_STATUS_SUCCESS;
}

/*** Help Command Handler Callback ***/
ECLECTIC_STATIC(void cli_helpCommandHandlerCallback(uint8_t argc, char *argv[]))
{
    char command[] = "--help";
    char *commandList[] = {command};
    cli_record_t *record;
    
    /*** Unused ***/
    ECLECTIC_UNUSED(argc);
    ECLECTIC_UNUSED(argv);
    
    /*** Help ***/
    record = cli_recordList;
    while(record != NULL)
    {
        if((strcmp(record->name, CLI_CONFIG_BUILT_IN_COMMAND_HELP_NAME) != 0) && (strcmp(record->name, CLI_CONFIG_BUILT_IN_COMMAND_LIST_NAME) != 0))
            record->commandHandlerCallback(sizeof(commandList) / sizeof(commandList[0]), commandList);
        record = record->next;
    }
}

/*** List Command Handler Callback ***/
ECLECTIC_STATIC(void cli_listCommandHandlerCallback(uint8_t argc, char *argv[]))
{
    cli_record_t *record;
    
    /*** Unused ***/
    ECLECTIC_UNUSED(argc);
    ECLECTIC_UNUSED(argv);
    
    /*** List ***/
    record = cli_recordList;
    while(record != NULL)
    {
        if((strcmp(record->name, CLI_CONFIG_BUILT_IN_COMMAND_HELP_NAME) != 0) && (strcmp(record->name, CLI_CONFIG_BUILT_IN_COMMAND_LIST_NAME) != 0)) // Don't List Built-In Commands
            cli_printfCallback(false, "%s\n", record->name);
        record = record->next;
    }
    cli_printfCallback(true, "");
}

/*** Verify Record Name Acceptable ***/
ECLECTIC_STATIC(eclectic_status_t cli_verifyRecordNameAcceptable(const char * const Name))
{
    size_t i;
    cli_record_t *record;
    
    /*** Error Check ***/
    /* NULL Pointer */
    if(Name == NULL)
        return ECLECTIC_STATUS_ERROR_NULL_POINTER;
    
    /* Length */
    if(strlen(Name) == 0)
        return ECLECTIC_STATUS_ERROR_LENGTH;

    /*** Verify Record Name Acceptable ***/
    /* Illegal Characters In Name */
    for(i = 0; i < strlen(Name); i++)
    {
        if(!isalpha(Name[i]))
            return ECLECTIC_STATUS_ERROR_STRING;
    }

    /* Not Unique Name */
    record = cli_recordList;
    while(record != NULL)
    {
        if(strcasecmp(record->name, Name) == 0)
            return ECLECTIC_STATUS_ERROR_NOT_UNIQUE;
        record = record->next;
    }

    return ECLECTIC_STATUS_SUCCESS;
}
