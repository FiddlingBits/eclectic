/****************************************************************************************************
 * Include
 ****************************************************************************************************/

#include "crc16.h"

/****************************************************************************************************
 * Function Definition (Public)
 ****************************************************************************************************/

/*** Calculate ***/
uint16_t crc16_calculate(const crc16_configuration_t * const Configuration, const uint8_t *Data, const size_t DataLength)
{
    uint16_t crc = CRC_CONFIG_CRC16_ERROR_CRC;
    size_t i;

    /*** Error Check ***/
    if((Configuration != NULL) && (Data != NULL))
    {
        /*** Set Up ***/
        crc = Configuration->initial;

        /*** Calculate ***/
        for(i = 0; i < DataLength; i++)
            crc = crc16_calculatePartial(Configuration, crc, Data[i], (i == 0), (i == (DataLength - 1)));
    }

    return crc;
}

/*** Calculate Partial ***/
uint16_t crc16_calculatePartial(const crc16_configuration_t * const Configuration, uint16_t crc, const uint8_t Data, const bool First, const bool Last)
{
    size_t bit;

    /*** Error Check ***/
    if(Configuration == NULL)
    {
        crc = CRC_CONFIG_CRC16_ERROR_CRC;
    }
    else
    {
        /*** Calculate Partial ***/
        if(Configuration->lookupTable == NULL)
        {
            /*** Loop ***/
            /* Reflect */
            if(Configuration->reflect)
                crc ^= (crcHelper_reflect8(Data) << 8);
            else
                crc ^= (Data << 8);

            /* Calculate */
            for(bit = 0; bit < 8; bit++)
            {
                if((crc & 0x8000) == 0x8000)
                    crc = (crc << 1) ^ Configuration->polynomial;
                else
                    crc <<= 1;
            }

            /* Last */
            if(Last)
            {
                crc ^= Configuration->xorOut;
                if(Configuration->reflect)
                    crc = crcHelper_reflect16(crc);
            }
        }
        else
        {
            /*** Lookup Table ***/
            /* Reflect */
            if(Configuration->reflect)
            {
                if(First)
                    crc = crcHelper_reflect16(crc);
                crc = (crc >> 8) ^ Configuration->lookupTable[(crc ^ Data) % 256];
            }
            else
            {
                crc = (crc << 8) ^ Configuration->lookupTable[(crc >> 8) ^ Data];
            }
            
            /* Last */
            if(Last)
                crc ^= Configuration->xorOut;
        }
    }

    return crc;
}

/*** Initialize ***/
eclectic_status_t crc16_init(const crc16_algorithm_t Algorithm, crc16_configuration_t * const configuration, uint16_t * const lookupTableMemory, const size_t LookupTableMemoryLength)
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
        case CRC16_ALGORITHM_CRC16_ARC:
            configuration->check = 0xBB3D;
            configuration->polynomial = 0x8005;
            configuration->initial = 0x0000;
            configuration->reflect = true;
            configuration->xorOut = 0x0000;
            break;
        case CRC16_ALGORITHM_CRC16_AUG_CCITT: 
            configuration->check = 0xE5CC;
            configuration->polynomial = 0x1021;
            configuration->initial = 0x1D0F;
            configuration->reflect = false;
            configuration->xorOut = 0x0000;
            break;
        case CRC16_ALGORITHM_CRC16_BUYPASS:
            configuration->check = 0xFEE8;
            configuration->polynomial = 0x8005;
            configuration->initial = 0x0000;
            configuration->reflect = false;
            configuration->xorOut = 0x0000;
            break;
        case CRC16_ALGORITHM_CRC16_CCITT_FALSE:
            configuration->check = 0x29B1;
            configuration->polynomial = 0x1021;
            configuration->initial = 0xFFFF;
            configuration->reflect = false;
            configuration->xorOut = 0x0000;
            break;
        case CRC16_ALGORITHM_CRC16_CDMA2000:
            configuration->check = 0x4C06;
            configuration->polynomial = 0xC867;
            configuration->initial = 0xFFFF;
            configuration->reflect = false;
            configuration->xorOut = 0x0000;
            break;
        case CRC16_ALGORITHM_CRC16_DDS_110:
            configuration->check = 0x9ECF;
            configuration->polynomial = 0x8005;
            configuration->initial = 0x800D;
            configuration->reflect = false;
            configuration->xorOut = 0x0000;
            break;
        case CRC16_ALGORITHM_CRC16_DECT_R:
            configuration->check = 0x007E;
            configuration->polynomial = 0x0589;
            configuration->initial = 0x0000;
            configuration->reflect = false;
            configuration->xorOut = 0x0001;
            break;
        case CRC16_ALGORITHM_CRC16_DECT_X:
            configuration->check = 0x007F;
            configuration->polynomial = 0x0589;
            configuration->initial = 0x0000;
            configuration->reflect = false;
            configuration->xorOut = 0x0000;
            break;
        case CRC16_ALGORITHM_CRC16_DNP:
            configuration->check = 0xEA82;
            configuration->polynomial = 0x3D65;
            configuration->initial = 0x0000;
            configuration->reflect = true;
            configuration->xorOut = 0xFFFF;
            break;
        case CRC16_ALGORITHM_CRC16_EN_13757:
            configuration->check = 0xC2B7;
            configuration->polynomial = 0x3D65;
            configuration->initial = 0x0000;
            configuration->reflect = false;
            configuration->xorOut = 0xFFFF;
            break;
        case CRC16_ALGORITHM_CRC16_GENIBUS:
            configuration->check = 0xD64E;
            configuration->polynomial = 0x1021;
            configuration->initial = 0xFFFF;
            configuration->reflect = false;
            configuration->xorOut = 0xFFFF;
            break;
        case CRC16_ALGORITHM_CRC16_KERMIT:
            configuration->check = 0x2189;
            configuration->polynomial = 0x1021;
            configuration->initial = 0x0000;
            configuration->reflect = true;
            configuration->xorOut = 0x0000;
            break;
        case CRC16_ALGORITHM_CRC16_MAXIM:
            configuration->check = 0x44C2;
            configuration->polynomial = 0x8005;
            configuration->initial = 0x0000;
            configuration->reflect = true;
            configuration->xorOut = 0xFFFF;
            break;
        case CRC16_ALGORITHM_CRC16_MCRF4XX:
            configuration->check = 0x6F91;
            configuration->polynomial = 0x1021;
            configuration->initial = 0xFFFF;
            configuration->reflect = true;
            configuration->xorOut = 0x0000;
            break;
        case CRC16_ALGORITHM_CRC16_MODBUS:
            configuration->check = 0x4B37;
            configuration->polynomial = 0x8005;
            configuration->initial = 0xFFFF;
            configuration->reflect = true;
            configuration->xorOut = 0x0000;
            break;
        case CRC16_ALGORITHM_CRC16_RIELLO:
            configuration->check = 0x63D0;
            configuration->polynomial = 0x1021;
            configuration->initial = 0xB2AA;
            configuration->reflect = true;
            configuration->xorOut = 0x0000;
            break;
        case CRC16_ALGORITHM_CRC16_T10_DIF:
            configuration->check = 0xD0DB;
            configuration->polynomial = 0x8BB7;
            configuration->initial = 0x0000;
            configuration->reflect = false;
            configuration->xorOut = 0x0000;
            break;
        case CRC16_ALGORITHM_CRC16_TELEDISK:
            configuration->check = 0x0FB3;
            configuration->polynomial = 0xA097;
            configuration->initial = 0x0000;
            configuration->reflect = false;
            configuration->xorOut = 0x0000;
            break;
        case CRC16_ALGORITHM_CRC16_TMS37157:
            configuration->check = 0x26B1;
            configuration->polynomial = 0x1021;
            configuration->initial = 0x89EC;
            configuration->reflect = true;
            configuration->xorOut = 0x0000;
            break;
        case CRC16_ALGORITHM_CRC16_USB:
            configuration->check = 0xB4C8;
            configuration->polynomial = 0x8005;
            configuration->initial = 0xFFFF;
            configuration->reflect = true;
            configuration->xorOut = 0xFFFF;
            break;
        case CRC16_ALGORITHM_CRC16_X_25:
            configuration->check = 0x906E;
            configuration->polynomial = 0x1021;
            configuration->initial = 0xFFFF;
            configuration->reflect = true;
            configuration->xorOut = 0xFFFF;
            break;
        case CRC16_ALGORITHM_CRC16_XMODEM:
            configuration->check = 0x31C3;
            configuration->polynomial = 0x1021;
            configuration->initial = 0x0000;
            configuration->reflect = false;
            configuration->xorOut = 0x0000;
            break;
        case CRC16_ALGORITHM_CRC_A:
            configuration->check = 0xBF05;
            configuration->polynomial = 0x1021;
            configuration->initial = 0xC6C6;
            configuration->reflect = true;
            configuration->xorOut = 0x0000;
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
        if(LookupTableMemoryLength != (256 * sizeof(uint16_t)))
        {
            return ECLECTIC_STATUS_ERROR_LENGTH;
        }
        else
        {
            for(i = 0; i < 256; i++)
                lookupTableMemory[i] = crc16_calculatePartial(configuration, 0x0000, (uint8_t)i, true, true) ^ configuration->xorOut;
            configuration->lookupTable = lookupTableMemory;
        }
    }

    return ECLECTIC_STATUS_SUCCESS;
}
