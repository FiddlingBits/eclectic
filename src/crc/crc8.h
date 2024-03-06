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
typedef enum crc8_algorithm_e
{
    CRC8_ALGORITHM_CRC8,
    CRC8_ALGORITHM_CRC8_CDMA2000,
    CRC8_ALGORITHM_CRC8_DARC,
    CRC8_ALGORITHM_CRC8_DVB_S2,
    CRC8_ALGORITHM_CRC8_EBU,
    CRC8_ALGORITHM_CRC8_I_CODE,
    CRC8_ALGORITHM_CRC8_ITU,
    CRC8_ALGORITHM_CRC8_MAXIM,
    CRC8_ALGORITHM_CRC8_ROHC,
    CRC8_ALGORITHM_CRC8_WCDMA,
    CRC8_ALGORITHM_CRC8_COUNT
} crc8_algorithm_t;

/*** Structure ***/
typedef struct crc8_configuration_s
{
    uint8_t check;
    uint8_t polynomial;
    uint8_t initial;
    bool reflect;
    uint8_t xorOut;
    uint8_t *lookupTable;
} crc8_configuration_t;

/****************************************************************************************************
 * Function Prototype
 ****************************************************************************************************/

extern uint8_t crc8_calculate(const crc8_configuration_t * const Configuration, const uint8_t *Data, const size_t DataLength);
extern uint8_t crc8_calculatePartial(const crc8_configuration_t * const Configuration, uint8_t crc, const uint8_t Data, const bool Last);
extern eclectic_status_t crc8_init(const crc8_algorithm_t Algorithm, crc8_configuration_t * const configuration, uint8_t * const lookupTableMemory, const size_t LookupTableMemoryLength);
