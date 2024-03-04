/****************************************************************************************************
 * Pragma
 ****************************************************************************************************/

#pragma once

/****************************************************************************************************
 * Include
 ****************************************************************************************************/

#include "cli_config.h"
#include <ctype.h>
#include "eclectic.h"
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <string.h>

/****************************************************************************************************
 * Type Definition
 ****************************************************************************************************/

/*** Function Pointer ***/
typedef void (*cli_alertProcessInputCallback_t)(void);
typedef void (*cli_commandHandlerCallback_t)(uint8_t argc, char *argv[]);
typedef void (*cli_printfCallback_t)(const bool Flush, const char * const Format, ...);

/*** Structure ***/
typedef struct cli_record_s
{
    const char *name;
    struct cli_record_s *prev;
    struct cli_record_s *next;
    cli_commandHandlerCallback_t commandHandlerCallback;
} cli_record_t;

/****************************************************************************************************
 * Function Prototype
 ****************************************************************************************************/

extern eclectic_status_t cli_addInputCharacter(const char Character);
extern eclectic_status_t cli_init(const cli_alertProcessInputCallback_t AlertProcessInputCallback, const cli_printfCallback_t PrintfCallback, const char * const CommandPrompt);
extern eclectic_status_t cli_processInput(void);
extern eclectic_status_t cli_registerCommand(cli_record_t * const record, const char * const Name, const cli_commandHandlerCallback_t CommandHandlerCallback);
