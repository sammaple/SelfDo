#include <stdio.h>
#include <stdlib.h>

#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

#include "uuid_imp.h"

#define MAX_LEN 24

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
    int res, fd=0;
    const char *fileName = "/factory_setting/uuid.txt";
/*
       res = access(fileName, 0);
    if (!res)
        return FALSE;
*/

    if (len > MAX_LEN)
        return FALSE;

    fd = open(fileName, O_RDWR | O_CREAT, 0544);
    if (fd>0){
        res = write(fd, uuid, len);
        if (res == len ){
            return TRUE;
        }else{
            remove(fileName);
        }
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
static unsigned char data[30] = {0};
unsigned char *UUID_getFormatEmmcUuid_imp(int uuid_len)
{
    // TODO: needs to be done
    int res, fd=0;
//    unsigned char data[30] = {0};
    const char *fileName = "/factory_setting/uuid.txt";

    res = access(fileName, 0);
    if (res)
        return NULL;

    fd = open(fileName, O_RDONLY);
    if (fd>0){
        res = read(fd, data, uuid_len);

        if (res == uuid_len)
            return data;
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



