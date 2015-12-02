
#include <stdio.h>
#include <stdlib.h>
#include <cutils/log.h>
#include "uuid_imp.h"

static char *client_argv[] =    {"client", "iiop:1.0//127.0.0.1:999/AMP::FACTORY/factory"};

BOOL UUID_init()
{
    int ret;
    MV_OSAL_Init();
    ret = AMP_Initialize(2, client_argv, NULL);
    UUIDLOGI("AMP_Initialize return %d",ret);
    return ret?FALSE:TRUE;
}

BOOL UUID_deInit()
{
    AMP_Deinitialize();
    MV_OSAL_Exit();
    return TRUE;
}
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
    // TODO: needs to be done

    UUID_init();

    int ret;
    UUIDLOGI("UUID_setFormatEmmcUuid_imp: %s",(char *)uuid);
    ret = MV_DRM_GENERIC_BurnSN(uuid);

    UUID_deInit();

    return ret?FALSE:TRUE;
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
static char sn[25];
unsigned char *UUID_getFormatEmmcUuid_imp( int uuid_len )
{
    UUID_init();

    // TODO: needs to be done
    int ret;
    ret = MV_DRM_GENERIC_GetSN(sn);
    UUIDLOGI("UUID--> %s; ret = %d \n", sn, ret);

    UUID_deInit();

    return sn;
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
    // TODO: needs to be done

    return TRUE;
}



