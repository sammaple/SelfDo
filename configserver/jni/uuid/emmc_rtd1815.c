
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include "uuid_imp.h"

#define UUID_MAX_LENGTH 128 
#define UUID_FILE_NAME "/tmp/factory/yunos_uuid"

static const unsigned short UUID_FLAG=0xECDA;

static BOOL UUID_init(void)
{
   return TRUE;
}

static unsigned short UUID_ReadFlag()
{
    int fd;
    unsigned short flag = 0;

    fd = open(UUID_FILE_NAME, O_RDONLY | O_SYNC);
    if(fd < 0) {
        UUIDLOGD("Unable to open uuid file");
        return flag;
    }

    read(fd, &flag, sizeof(flag));
    close(fd);

    return flag;
}

static BOOL UUID_IsEmpty(void)
{
    int fd;

    if(access(UUID_FILE_NAME, F_OK | R_OK | W_OK)) {
        UUIDLOGD("uuid file is not created");
        return TRUE;
    }

    if(UUID_ReadFlag() != UUID_FLAG)
        return TRUE;

    return FALSE;
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

    int fd;

    fd = open(UUID_FILE_NAME, O_WRONLY | O_CREAT | O_SYNC, S_IRWXU);
    if(fd < 0) {
        UUIDLOGD("Can not open uuid file");
        return FALSE;     
    }
   
    if(write(fd, &UUID_FLAG, sizeof(UUID_FLAG)) !=
            sizeof(UUID_FLAG)) {
        UUIDLOGD("Write UUID FLAG error");
        return FALSE;
    }
    
    if(write(fd, uuid, len) != len) {
        UUIDLOGD("Write UUID data error");
        return FALSE;
    }

    close(fd);
    system("factory save");
    system("sync");

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
    static unsigned char uuid_buf[UUID_MAX_LENGTH];
    int fd = open(UUID_FILE_NAME, O_RDONLY | O_SYNC);
    unsigned short flag;

    memset(uuid_buf, 0, sizeof(uuid_buf));
    if(fd < 0) {
        UUIDLOGD("Error to open uuid file");
        return NULL;
    }

    read(fd, &flag, sizeof(flag));
    if(flag != UUID_FLAG) {
        UUIDLOGD("Error to read uuid flag");
        close(fd);
        return NULL;

    } else {
        read(fd, uuid_buf, uuid_len);
        close(fd);
        return uuid_buf;
    }   
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
BOOL UUID_isOtpRom_imp( void )
{
    // uuid_isotpRom
    return TRUE;
}
