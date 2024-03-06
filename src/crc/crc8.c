/****************************************************************************************************
 * Include
 ****************************************************************************************************/

#include "crc8.h"

/****************************************************************************************************
 * Function Definition (Public)
 ****************************************************************************************************/

/*** Calculate ***/
uint8_t crc8_calculate(const crc8_configuration_t * const Configuration, const uint8_t *Data, const size_t DataLength)
{
    uint8_t crc = CRC_CONFIG_CRC8_ERROR_CRC;
    size_t i;

    /*** Error Check ***/
    if((Configuration != NULL) && (Data != NULL))
    {
        /*** Set Up ***/
        crc = Configuration->initial;

        /*** Calculate ***/
        for(i = 0; i < DataLength; i++)
            crc = crc8_calculatePartial(Configuration, crc, Data[i], (i == (DataLength - 1)));
    }

    return crc;
}

/*** Calculate Partial ***/
uint8_t crc8_calculatePartial(const crc8_configuration_t * const Configuration, uint8_t crc, const uint8_t Data, const bool Last)
{
    size_t bit;

    /*** Error Check ***/
    if(Configuration == NULL)
    {
        crc = CRC_CONFIG_CRC8_ERROR_CRC;
    }
    else
    {
        /*** Calculate Partial ***/
        if(Configuration->lookupTable == NULL)
        {
            /*** Loop ***/
            /* Reflect */
            if(Configuration->reflect)
                crc ^= crcHelper_reflect8(Data);
            else
                crc ^= Data;

            /* Calculate */
            for(bit = 0; bit < 8; bit++)
            {
                if((crc & 0x80) == 0x80)
                    crc = (crc << 1) ^ Configuration->polynomial;
                else
                    crc <<= 1;
            }

            /* Last */
            if(Last)
            {
                crc ^= Configuration->xorOut;
                if(Configuration->reflect)
                    crc = crcHelper_reflect8(crc);
            }
        }
        else
        {
            /*** Lookup Table ***/
            crc = Configuration->lookupTable[crc ^ Data];
            if(Last)
                crc ^= Configuration->xorOut;
        }
    }

    return crc;
}

/*** Initialize ***/
eclectic_status_t crc8_init(const crc8_algorithm_t Algorithm, crc8_configuration_t * const configuration, uint8_t * const lookupTableMemory, const size_t LookupTableMemoryLength)
{
    size_t i;

    /*** Error Check ***/
    /* NULL Pointer */
    if(configuration == NULL)
        return ECLECTIC_STATUS_ERROR_NULL_POINTER;

    /*** Initialize ***/
    /* Configuration */
    switch(Algorithm)
    {
        case CRC8_ALGORITHM_CRC8:
            configuration->check = 0xF4;
            configuration->polynomial = 0x07;
            configuration->initial = 0x00;
            configuration->reflect = false;
            configuration->xorOut = 0x00;
            break;
        case CRC8_ALGORITHM_CRC8_CDMA2000:
            configuration->check = 0xDA;
            configuration->polynomial = 0x9B;
            configuration->initial = 0xFF;
            configuration->reflect = false;
            configuration->xorOut = 0x00;
            break;
        case CRC8_ALGORITHM_CRC8_DARC:
            configuration->check = 0x15;
            configuration->polynomial = 0x39;
            configuration->initial = 0x00;
            configuration->reflect = true;
            configuration->xorOut = 0x00;
            break;
        case CRC8_ALGORITHM_CRC8_DVB_S2:
            configuration->check = 0xBC;
            configuration->polynomial = 0xD5;
            configuration->initial = 0x00;
            configuration->reflect = false;
            configuration->xorOut = 0x00;
            break;
        case CRC8_ALGORITHM_CRC8_EBU:
            configuration->check = 0x97;
            configuration->polynomial = 0x1D;
            configuration->initial = 0xFF;
            configuration->reflect = true;
            configuration->xorOut = 0x00;
            break;
        case CRC8_ALGORITHM_CRC8_I_CODE:
            configuration->check = 0x7E;
            configuration->polynomial = 0x1D;
            configuration->initial = 0xFD;
            configuration->reflect = false;
            configuration->xorOut = 0x00;
            break;
        case CRC8_ALGORITHM_CRC8_ITU:
            configuration->check = 0xA1;
            configuration->polynomial = 0x07;
            configuration->initial = 0x00;
            configuration->reflect = false;
            configuration->xorOut = 0x55;
            break;
        case CRC8_ALGORITHM_CRC8_MAXIM:
            configuration->check = 0xA1;
            configuration->polynomial = 0x31;
            configuration->initial = 0x00;
            configuration->reflect = true;
            configuration->xorOut = 0x00;
            break;
        case CRC8_ALGORITHM_CRC8_ROHC:
            configuration->check = 0xD0;
            configuration->polynomial = 0x07;
            configuration->initial = 0xFF;
            configuration->reflect = true;
            configuration->xorOut = 0x00;
            break;
        case CRC8_ALGORITHM_CRC8_WCDMA:
            configuration->check = 0x25;
            configuration->polynomial = 0x9B;
            configuration->initial = 0x00;
            configuration->reflect = true;
            configuration->xorOut = 0x00;
            break;
        default:
            /* Error */
            return ECLECTIC_STATUS_ERROR_INVALID;
    }

    /* Generate Lookup Table */
    configuration->lookupTable = NULL;
    if(lookupTableMemory != NULL)
    {
        /* Error Check */
        if(LookupTableMemoryLength != (256 * sizeof(uint8_t)))
        {
            return ECLECTIC_STATUS_ERROR_LENGTH;
        }
        else
        {
            for(i = 0; i < 256; i++)
                lookupTableMemory[i] = crc8_calculatePartial(configuration, 0x00, (uint8_t)i, true) ^ configuration->xorOut;
            configuration->lookupTable = lookupTableMemory;
        }
    }

    return ECLECTIC_STATUS_SUCCESS;
}
