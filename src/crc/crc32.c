/****************************************************************************************************
 * Include
 ****************************************************************************************************/

#include "crc32.h"

/****************************************************************************************************
 * Function Definition (Public)
 ****************************************************************************************************/

/*** Calculate ***/
uint32_t crc32_calculate(const crc32_configuration_t * const Configuration, const uint8_t *Data, const size_t DataLength)
{
    uint32_t crc = CRC_CONFIG_CRC32_ERROR_CRC;
    size_t i;

    /*** Error Check ***/
    if((Configuration != NULL) && (Data != NULL))
    {
        /*** Set Up ***/
        crc = Configuration->initial;

        /*** Calculate ***/
        for(i = 0; i < DataLength; i++)
            crc = crc32_calculatePartial(Configuration, crc, Data[i], (i == 0), (i == (DataLength - 1)));
    }

    return crc;
}

/*** Calculate Partial ***/
uint32_t crc32_calculatePartial(const crc32_configuration_t * const Configuration, uint32_t crc, const uint8_t Data, const bool First, const bool Last)
{
    size_t bit;

    /*** Error Check ***/
    if(Configuration == NULL)
    {
        crc = CRC_CONFIG_CRC32_ERROR_CRC;
    }
    else
    {
        /*** Calculate Partial ***/
        if(Configuration->lookupTable == NULL)
        {
            /*** Loop ***/
            /* Reflect */
            if(Configuration->reflect)
                crc ^= (crcHelper_reflect8(Data) << 24);
            else
                crc ^= (Data << 24);

            /* Calculate */
            for(bit = 0; bit < 8; bit++)
            {
                if((crc & 0x80000000) == 0x80000000)
                    crc = (crc << 1) ^ Configuration->polynomial;
                else
                    crc <<= 1;
            }

            /* Last */
            if(Last)
            {
                crc ^= Configuration->xorOut;
                if(Configuration->reflect)
                    crc = crcHelper_reflect32(crc);
            }
        }
        else
        {
            /*** Lookup Table ***/
            /* Reflect */
            if(Configuration->reflect)
            {
                if(First)
                    crc = crcHelper_reflect32(crc);
                crc = (crc >> 8) ^ Configuration->lookupTable[(crc ^ Data) % 256];
            }
            else
            {
                crc = (crc << 8) ^ Configuration->lookupTable[(crc >> 24) ^ Data];
            }
            
            /* Last */
            if(Last)
                crc ^= Configuration->xorOut;
        }
    }

    return crc;
}

/*** Initialize ***/
eclectic_status_t crc32_init(const crc32_algorithm_t Algorithm, crc32_configuration_t * const configuration, uint32_t * const lookupTableMemory, const size_t LookupTableMemoryLength)
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
        case CRC32_ALGORITHM_CRC32:
            configuration->check = 0xCBF43926;
            configuration->polynomial = 0x04C11DB7;
            configuration->initial = 0xFFFFFFFF;
            configuration->reflect = true;
            configuration->xorOut = 0xFFFFFFFF;
            break;
        case CRC32_ALGORITHM_CRC32_BZIP2:
            configuration->check = 0xFC891918;
            configuration->polynomial = 0x04C11DB7;
            configuration->initial = 0xFFFFFFFF;
            configuration->reflect = false;
            configuration->xorOut = 0xFFFFFFFF;
            break;
        case CRC32_ALGORITHM_CRC32_JAMCRC:
            configuration->check = 0x340BC6D9;
            configuration->polynomial = 0x04C11DB7;
            configuration->initial = 0xFFFFFFFF;
            configuration->reflect = true;
            configuration->xorOut = 0x00000000;
            break;
        case CRC32_ALGORITHM_CRC32_MPEG_2:
            configuration->check = 0x0376E6E7;
            configuration->polynomial = 0x04C11DB7;
            configuration->initial = 0xFFFFFFFF;
            configuration->reflect = false;
            configuration->xorOut = 0x00000000;
            break;
        case CRC32_ALGORITHM_CRC32_POSIX:
            configuration->check = 0x765E7680;
            configuration->polynomial = 0x04C11DB7;
            configuration->initial = 0x00000000;
            configuration->reflect = false;
            configuration->xorOut = 0xFFFFFFFF;
            break;
        case CRC32_ALGORITHM_CRC32_SATA:
            configuration->check = 0xCF72AFE8;
            configuration->polynomial = 0x04C11DB7;
            configuration->initial = 0x52325032;
            configuration->reflect = false;
            configuration->xorOut = 0x00000000;
            break;
        case CRC32_ALGORITHM_CRC32_XFER:
            configuration->check = 0xBD0BE338;
            configuration->polynomial = 0x000000AF;
            configuration->initial = 0x00000000;
            configuration->reflect = false;
            configuration->xorOut = 0x00000000;
            break;
        case CRC32_ALGORITHM_CRC32C:
            configuration->check = 0xE3069283;
            configuration->polynomial = 0x1EDC6F41;
            configuration->initial = 0xFFFFFFFF;
            configuration->reflect = true;
            configuration->xorOut = 0xFFFFFFFF;
            break;
        case CRC32_ALGORITHM_CRC32D:
            configuration->check = 0x87315576;
            configuration->polynomial = 0xA833982B;
            configuration->initial = 0xFFFFFFFF;
            configuration->reflect = true;
            configuration->xorOut = 0xFFFFFFFF;
            break;
        case CRC32_ALGORITHM_CRC32Q:
            configuration->check = 0x3010BF7F;
            configuration->polynomial = 0x814141AB;
            configuration->initial = 0x00000000;
            configuration->reflect = false;
            configuration->xorOut = 0x00000000;
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
        if(LookupTableMemoryLength != (256 * sizeof(uint32_t)))
        {
            return ECLECTIC_STATUS_ERROR_LENGTH;
        }
        else
        {
            for(i = 0; i < 256; i++)
                lookupTableMemory[i] = crc32_calculatePartial(configuration, 0x00000000, (uint8_t)i, true, true) ^ configuration->xorOut;
            configuration->lookupTable = lookupTableMemory;
        }
    }

    return ECLECTIC_STATUS_SUCCESS;
}
