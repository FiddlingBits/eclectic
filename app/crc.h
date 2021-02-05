/****************************************************************************************************
 * FILE:    crc.h
 * BRIEF:   Cyclic Redundancy Check (CRC) Header File
 ****************************************************************************************************/

#ifndef CRC_H
#define CRC_H

/****************************************************************************************************
 * Definitions
 ****************************************************************************************************/

/*** CRC-8 ***/
#define CRC_CRC8_INITIAL_CRC8                   (0x00)
#define CRC_CRC8_CDMA2000_INITIAL_CRC8_CDMA2000 (0xFF)

/****************************************************************************************************
 * Includes
 ****************************************************************************************************/

#include <stdint.h>

/****************************************************************************************************
 * Function Prototypes
 ****************************************************************************************************/

/*** CRC-8 ***/
/* CRC-8 */
extern uint8_t crc_crc8CalculateLookupTable(const uint8_t * const Data, const uint16_t DataLength);
extern uint8_t crc_crc8CalculateLoop(const uint8_t * const Data, const uint16_t DataLength);
extern uint8_t crc_crc8CalculatePartialLookupTable(const uint8_t Data, uint8_t crc8);
extern uint8_t crc_crc8CalculatePartialLoop(const uint8_t Data, uint8_t crc8);

/* CRC-8/CDMA2000 */
extern uint8_t crc_crc8Cdma2000CalculateLookupTable(const uint8_t * const Data, const uint16_t DataLength);
extern uint8_t crc_crc8Cdma2000CalculateLoop(const uint8_t * const Data, const uint16_t DataLength);
extern uint8_t crc_crc8Cdma2000CalculatePartialLookupTable(const uint8_t Data, uint8_t crc8Cdma2000);
extern uint8_t crc_crc8Cdma2000CalculatePartialLoop(const uint8_t Data, uint8_t crc8Cdma2000);

#endif
