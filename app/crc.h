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

/* CRC-8/DARC */
#if defined(CRC_CRC8_DARC_LOOKUP_TABLE_METHOD) || defined(CRC_CRC8_DARC_LOOP_METHOD)
  #define CRC_CRC8_DARC_INITIAL_CRC8_DARC (0x00)
#endif

/* CRC-8/DVB-S2 */
#if defined(CRC_CRC8_DVB_S2_LOOKUP_TABLE_METHOD) || defined(CRC_CRC8_DVB_S2_LOOP_METHOD)
  #define CRC_CRC8_DVB_S2_INITIAL_CRC8_DVB_S2 (0x00)
#endif

/* CRC-8/EBU */
#if defined(CRC_CRC8_EBU_LOOKUP_TABLE_METHOD) || defined(CRC_CRC8_EBU_LOOP_METHOD)
  #define CRC_CRC8_EBU_INITIAL_CRC8_EBU (0xFF)
#endif

/* CRC-8/I-CODE */
#if defined(CRC_CRC8_I_CODE_LOOKUP_TABLE_METHOD) || defined(CRC_CRC8_I_CODE_LOOP_METHOD)
  #define CRC_CRC8_I_CODE_INITIAL_CRC8_I_CODE (0xFD)
#endif

/* CRC-8/ITU */
#if defined(CRC_CRC8_ITU_LOOKUP_TABLE_METHOD) || defined(CRC_CRC8_ITU_LOOP_METHOD)
  #define CRC_CRC8_ITU_INITIAL_CRC8_ITU (0x00)
#endif

/****************************************************************************************************
 * Includes
 ****************************************************************************************************/

#include <stdbool.h>
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

/* CRC-8/DARC */
#if defined(CRC_CRC8_DARC_LOOKUP_TABLE_METHOD) || defined(CRC_CRC8_DARC_LOOP_METHOD)
  extern uint8_t crc_crc8DarcCalculate(const uint8_t * const Data, const uint16_t DataLength);
  extern uint8_t crc_crc8DarcCalculatePartial(const uint8_t Data, uint8_t crc8Darc, const bool Final);
#endif

/* CRC-8/DVB-S2 */
#if defined(CRC_CRC8_DVB_S2_LOOKUP_TABLE_METHOD) || defined(CRC_CRC8_DVB_S2_LOOP_METHOD)
  extern uint8_t crc_crc8DvbS2Calculate(const uint8_t * const Data, const uint16_t DataLength);
  extern uint8_t crc_crc8DvbS2CalculatePartial(const uint8_t Data, uint8_t crc8DvbS2);
#endif

/* CRC-8/EBU */
#if defined(CRC_CRC8_EBU_LOOKUP_TABLE_METHOD) || defined(CRC_CRC8_EBU_LOOP_METHOD)
  extern uint8_t crc_crc8EbuCalculate(const uint8_t * const Data, const uint16_t DataLength);
  extern uint8_t crc_crc8EbuCalculatePartial(const uint8_t Data, uint8_t crc8Ebu, const bool Final);
#endif

/* CRC-8/I-CODE */
#if defined(CRC_CRC8_I_CODE_LOOKUP_TABLE_METHOD) || defined(CRC_CRC8_I_CODE_LOOP_METHOD)
  extern uint8_t crc_crc8ICodeCalculate(const uint8_t * const Data, const uint16_t DataLength);
  extern uint8_t crc_crc8ICodeCalculatePartial(const uint8_t Data, uint8_t crc8ICode);
#endif

/* CRC-8/ITU */
#if defined(CRC_CRC8_ITU_LOOKUP_TABLE_METHOD) || defined(CRC_CRC8_ITU_LOOP_METHOD)
  extern uint8_t crc_crc8ItuCalculate(const uint8_t * const Data, const uint16_t DataLength);
  extern uint8_t crc_crc8ItuCalculatePartial(const uint8_t Data, uint8_t crc8Itu, const bool Final);
#endif

#endif
