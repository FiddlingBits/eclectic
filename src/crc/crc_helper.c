/****************************************************************************************************
 * Include
 ****************************************************************************************************/

#include "crc_helper.h"

/****************************************************************************************************
 * Function Definition (Public)
 ****************************************************************************************************/

/*** Reflect8 ***/
uint8_t crcHelper_reflect8(const uint8_t Data)
{
    size_t bit;
    uint8_t reflectedData = 0x00;

    /*** Reflect8 ***/
    for(bit = 0; bit < 8; bit++)
        reflectedData |= ((Data & (1 << bit)) >> bit) << (7 - bit);

    return reflectedData;
}

/*** Reflect16 ***/
uint16_t crcHelper_reflect16(const uint16_t Data)
{
    size_t bit;
    uint16_t reflectedData = 0x0000;
    
    /*** Reflect16 ***/
    for(bit = 0; bit < 16; bit++)
        reflectedData |= ((Data & (1 << bit)) >> bit) << (15 - bit);
    
    return reflectedData;
}

/*** Reflect32 ***/
uint32_t crcHelper_reflect32(const uint32_t Data)
{
    size_t bit;
    uint32_t reflectedData = 0x00000000;
    
    /*** Reflect32 ***/
    for(bit = 0; bit < 32; bit++)
        reflectedData |= ((Data & (1 << bit)) >> bit) << (31 - bit);
    
    return reflectedData;
}
