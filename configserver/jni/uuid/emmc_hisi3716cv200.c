
#include <stdio.h>
#include <stdlib.h>
#include <cutils/log.h>

#include "uuid_imp.h"
#include "hi_unf_otp.h"

extern unsigned int CalcCRC32(unsigned char*, unsigned int);

/**************************************************************************
 *  function name     : UUID_setFormatEmmcUuid_imp
 *  function describe : write uuid to OTP memory
 *  in parameter      : unsigned char *uuid : uuid buffer pointer
                        int len             : uuid string length(Byte)
 *  out parameter     :
 *  author            :
 *  create time       :
 *  return value      : TRUE                : set uuid SUCCESS
                        FALSE               : set uuid FAIL
 *  remarks           : the uuid lenght is less then 24 Byte
 **************************************************************************/
BOOL UUID_setFormatEmmcUuid_imp(unsigned char *uuid, int len)
{
    unsigned char u8buf[32] = {0};
    unsigned int crc32 = 0;
    unsigned int i = 0;

    if (len > 24)
    {
        UUIDLOGE("Invalid UUID length!");
        return FALSE;
    }

    memcpy(u8buf, uuid, len);

    crc32 = CalcCRC32(u8buf, 24);
    memcpy((unsigned char *)(&(u8buf[24])), (unsigned char *)(&crc32), 4);  //*(unsigned int *)&u8buf[24] = crc32;

    /** write UUID to OTP */
    if (HI_UNF_OTP_Open() != HI_SUCCESS)
    {
        UUIDLOGE("Open OTP failed!");
        return FALSE;
    }

    if (HI_UNF_OTP_SetCustomerKey(u8buf, 16) != HI_SUCCESS)
    {
        UUIDLOGE("1:Set UUID failed!");
        HI_UNF_OTP_Close();
        return FALSE;
    }

    for (i = 0; i < 16; i++)
    {
        if (HI_UNF_OTP_SetStbPrivData(i, u8buf[i + 16]) != HI_SUCCESS)
        {
            UUIDLOGE("2:Set UUID failed!");
            HI_UNF_OTP_Close();
            return FALSE;
        }
    }
    HI_UNF_OTP_Close();

    return TRUE;
}

/**************************************************************************
 *  function name     : UUID_setFormatEmmcUuid_imp
 *  function describe : get uuid from OTP memory
 *  in parameter      : int uuid_len             : need to get length(Byte)
 *  out parameter     :
 *  author            :
 *  create time       :
 *  return value      : UUID memory        : get uuid SUCCESS
                        NULL               : set uuid FAIL
 *  remarks           : the uuid lenght is less then 24 Byte
 **************************************************************************/
unsigned char *UUID_getFormatEmmcUuid_imp( int uuid_len )
{
    static unsigned char u8buf[32] = {0};
    unsigned int crc32 = 0;
    unsigned int i = 0;

    if (uuid_len > 24)
    {
        UUIDLOGE("Invalid UUID length!");
        return FALSE;
    }

    /** get UUID from OTP */
    if (HI_UNF_OTP_Open() != HI_SUCCESS)
    {
        UUIDLOGE("Open OTP failed!");
        return NULL;
    }

    if (HI_UNF_OTP_GetCustomerKey(u8buf, 16) != HI_SUCCESS)
    {
        UUIDLOGE("1:Get UUID failed!");
        goto ERR;
    }

    for (i = 0; i < 16; i++)
    {
        if (HI_UNF_OTP_GetStbPrivData(i, &u8buf[i + 16]) != HI_SUCCESS)
        {
            UUIDLOGE("2:Get UUID failed!");
            goto ERR;
        }
    }

    memcpy((unsigned char *)&crc32, (unsigned char *)&u8buf[24], 4);    //crc32 = *(unsigned int *)&u8buf[24];
    if (crc32 != CalcCRC32(u8buf, 24))
    {
            UUIDLOGE("UUID not correct!");
            goto ERR;
    }

    memset(&u8buf[24], 0, (32 - 24));   // *(unsigned int *)&u8buf[24] = 0;
    HI_UNF_OTP_Close();

    return u8buf;

ERR:
    HI_UNF_OTP_Close();
    return NULL;
}

/**************************************************************************
 *  function name     : UUID_isOtpRom_imp
 *  function describe : return memory status
 *  in parameter      :
 *  out parameter     :
 *  author            :
 *  create time       :
 *  return value      : TRUE                : is OTP memory
                        FALSE               : is not OTP memory
 *  remarks           :
 **************************************************************************/
BOOL UUID_isOtpRom_imp( )
{
    return TRUE;
}
