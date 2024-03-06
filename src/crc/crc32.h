/****************************************************************************************************
 * Pragma
 ****************************************************************************************************/

#pragma once

/****************************************************************************************************
 * Include
 ****************************************************************************************************/

#include "crc_config.h"
#include "crc_helper.h"
#include "eclectic.h"
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

/****************************************************************************************************
 * Type Definition
 ****************************************************************************************************/

/*** Enumeration ***/
typedef enum crc32_algorithm_e
{
    CRC32_ALGORITHM_CRC32,
    CRC32_ALGORITHM_CRC32_BZIP2,
    CRC32_ALGORITHM_CRC32_JAMCRC,
    CRC32_ALGORITHM_CRC32_MPEG_2,
    CRC32_ALGORITHM_CRC32_POSIX,
    CRC32_ALGORITHM_CRC32_SATA,
    CRC32_ALGORITHM_CRC32_XFER,
    CRC32_ALGORITHM_CRC32C,
    CRC32_ALGORITHM_CRC32D,
    CRC32_ALGORITHM_CRC32Q,
    CRC32_ALGORITHM_CRC32_COUNT
} crc32_algorithm_t;

/*** Structure ***/
typedef struct crc32_configuration_s
{
    uint32_t check;
    uint32_t polynomial;
    uint32_t initial;
    bool reflect;
    uint32_t xorOut;
    uint32_t *lookupTable;
} crc32_configuration_t;

/****************************************************************************************************
 * Function Prototype
 ****************************************************************************************************/

extern uint32_t crc32_calculate(const crc32_configuration_t * const Configuration, const uint8_t *Data, const size_t DataLength);
extern uint32_t crc32_calculatePartial(const crc32_configuration_t * const Configuration, uint32_t crc, const uint8_t Data, const bool First, const bool Last);
extern eclectic_status_t crc32_init(const crc32_algorithm_t Algorithm, crc32_configuration_t * const configuration, uint32_t * const lookupTableMemory, const size_t LookupTableMemoryLength);
