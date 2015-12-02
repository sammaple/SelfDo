
#include <stdio.h>
#include <stdlib.h>

#include "uuid_imp.h"

extern int wmt_getsyspara(const char *varname, char *varval, int * varlen);
extern int wmt_setsyspara(const char *varname, const char *varval);

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
    if(uuid == NULL || len <= 0)
    {
        return FALSE;
    }

    if(!wmt_setsyspara("uuid.ro",uuid))
    {
        return TRUE;
    }

    return FALSE;
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

      if(uuid_len <= 0)
      {
          return NULL;
      }

      char *uuid = malloc(uuid_len);
      if(uuid == NULL)
      {
          return NULL;
      }

      if (!wmt_getsyspara("uuid.ro", uuid, &uuid_len))
      {
          return (unsigned char *)uuid;
      }


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
    // TODO: needs to be done

    return FALSE;
}



