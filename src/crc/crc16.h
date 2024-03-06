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
typedef enum crc16_algorithm_e
{
    CRC16_ALGORITHM_CRC16_ARC,
    CRC16_ALGORITHM_CRC16_AUG_CCITT,
    CRC16_ALGORITHM_CRC16_BUYPASS,
    CRC16_ALGORITHM_CRC16_CCITT_FALSE,
    CRC16_ALGORITHM_CRC16_CDMA2000,
    CRC16_ALGORITHM_CRC16_DDS_110,
    CRC16_ALGORITHM_CRC16_DECT_R,
    CRC16_ALGORITHM_CRC16_DECT_X,
    CRC16_ALGORITHM_CRC16_DNP,
    CRC16_ALGORITHM_CRC16_EN_13757,
    CRC16_ALGORITHM_CRC16_GENIBUS,
    CRC16_ALGORITHM_CRC16_KERMIT,
    CRC16_ALGORITHM_CRC16_MAXIM,
    CRC16_ALGORITHM_CRC16_MCRF4XX,
    CRC16_ALGORITHM_CRC16_MODBUS,
    CRC16_ALGORITHM_CRC16_RIELLO,
    CRC16_ALGORITHM_CRC16_T10_DIF,
    CRC16_ALGORITHM_CRC16_TELEDISK,
    CRC16_ALGORITHM_CRC16_TMS37157,
    CRC16_ALGORITHM_CRC16_USB,
    CRC16_ALGORITHM_CRC16_X_25,
    CRC16_ALGORITHM_CRC16_XMODEM,
    CRC16_ALGORITHM_CRC_A,
    CRC16_ALGORITHM_CRC16_COUNT
} crc16_algorithm_t;

/*** Structure ***/
typedef struct crc16_configuration_s
{
    uint16_t check;
    uint16_t polynomial;
    uint16_t initial;
    bool reflect;
    uint16_t xorOut;
    uint16_t *lookupTable;
} crc16_configuration_t;

/****************************************************************************************************
 * Function Prototype
 ****************************************************************************************************/

extern uint16_t crc16_calculate(const crc16_configuration_t * const Configuration, const uint8_t *Data, const size_t DataLength);
extern uint16_t crc16_calculatePartial(const crc16_configuration_t * const Configuration, uint16_t crc, const uint8_t Data, const bool First, const bool Last);
extern eclectic_status_t crc16_init(const crc16_algorithm_t Algorithm, crc16_configuration_t * const configuration, uint16_t * const lookupTableMemory, const size_t LookupTableMemoryLength);
