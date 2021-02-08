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
/* CRC-8 */
#if defined(CRC_CRC8_LOOKUP_TABLE_METHOD) || defined(CRC_CRC8_LOOP_METHOD)
  #define CRC_CRC8_INITIAL_CRC8 (0x00)
#endif

/* CRC-8/CDMA2000 */
#if defined(CRC_CRC8_CDMA2000_LOOKUP_TABLE_METHOD) || defined(CRC_CRC8_CDMA2000_LOOP_METHOD)
  #define CRC_CRC8_CDMA2000_INITIAL_CRC8_CDMA2000 (0xFF)
#endif

/****************************************************************************************************
 * Includes
 ****************************************************************************************************/

#include <stdint.h>

/****************************************************************************************************
 * Function Prototypes
 ****************************************************************************************************/

/*** CRC-8 ***/
/* CRC-8 */
#if defined(CRC_CRC8_LOOKUP_TABLE_METHOD) || defined(CRC_CRC8_LOOP_METHOD)
  extern uint8_t crc_crc8Calculate(const uint8_t * const Data, const uint16_t DataLength);
  extern uint8_t crc_crc8CalculatePartial(const uint8_t Data, uint8_t crc8);
#endif

/* CRC-8/CDMA2000 */
#if defined(CRC_CRC8_CDMA2000_LOOKUP_TABLE_METHOD) || defined(CRC_CRC8_CDMA2000_LOOP_METHOD)
  extern uint8_t crc_crc8Cdma2000Calculate(const uint8_t * const Data, const uint16_t DataLength);
  extern uint8_t crc_crc8Cdma2000CalculatePartial(const uint8_t Data, uint8_t crc8Cdma2000);
#endif

#endif
