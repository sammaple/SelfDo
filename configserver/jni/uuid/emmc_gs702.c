#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include "uuid_imp.h"
#include <errno.h>
#include <string.h>

#undef UUID_MAX_LEN
// support uuid:max 32 byte,includes end character '\0'
#define UUID_MAX_LEN 32

#undef UUID_FILE_PATH
#define UUID_FILE_PATH "/sys/miscinfo/infos/uuid"

static void UUID_print_uuid(char * const format, unsigned char *pBuff, int len);

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
    int fd          = -1;
    int uuidLen   = 0;
    int ret       = -1;
    int i         = 0;

    if (0 == uuid || len <= 0) {
        if (0 == uuid) {
            UUIDLOGE("UUID_setFormatEmmcUuid_imp:uuid is null");
        }
        else {
            UUIDLOGE("UUID_setFormatEmmcUuid_imp: len is less than or equals to zero");
        }
        return FALSE;
    }

    uuidLen = len > UUID_MAX_LEN ? UUID_MAX_LEN : len;
    //test code : print uuid content
    UUID_print_uuid("UUID_setFormatEmmcUuid_imp:", uuid, uuidLen);

    fd = open(UUID_FILE_PATH, O_WRONLY);
    if (fd >= 0) {
        ret = write(fd, uuid, uuidLen);
        if (ret <= 0) {
            UUIDLOGE("UUID_setFormatEmmcUuid_imp:write uuid return error, errno is %d", errno);
        }
        close(fd);
        return ret <= 0 ? FALSE : TRUE;
    }
    else {
        UUIDLOGE("UUID_setFormatEmmcUuid_imp:open uuid file failure, errno is %d", errno);
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
                        NULL               : get uuid FAIL
 *  remarks           : the uuid lenght is less then 24 Byte
 **************************************************************************/
unsigned char *UUID_getFormatEmmcUuid_imp( int uuid_len )
{
    int fd     = -1;
    int ret = -1;
    unsigned char * pBuffer = NULL;

    UUIDLOGE("UUID_getFormatEmmcUuid_imp: len is %d", uuid_len);

    if (uuid_len > UUID_MAX_LEN) {
        uuid_len = UUID_MAX_LEN;
    }

    if (uuid_len > 0) {
        pBuffer = (unsigned char * )malloc(uuid_len);
        if (NULL == pBuffer) {
            UUIDLOGE("UUID_getFormatEmmcUuid_imp:malloc memory failure!!!!");
            return NULL;
        }

        memset(pBuffer, 0, uuid_len);

        fd = open(UUID_FILE_PATH, O_RDONLY);
        if (fd >= 0) {
            ret = read(fd, pBuffer, uuid_len);
            if (ret >= 0) {
                close(fd);
                UUID_print_uuid("UUID_getFormatEmmcUuid_imp:", pBuffer, uuid_len);
                return pBuffer;
            }
            else {
                UUIDLOGE("UUID_getFormatEmmcUuid_imp: read uuid file failure,errno is %d", errno);
            }
            close(fd);
        }
        else {
            UUIDLOGE("UUID_getFormatEmmcUuid_imp: open uuid file failure, errno is %d", errno);
        }

        free(pBuffer);
        return NULL;
    }

    return NULL;
}

// print uuid in hexdump
static void UUID_print_uuid(char * const format, unsigned char *pBuff, int len) {
    unsigned char charBuf[100] = {0};
    int i = 0;
    int a = 0;

    for (i = 0; i < len; ++i) {
        a = (pBuff[i]) / 16;
        if (a >= 0 && a <= 9) {
            charBuf[3 * i] = 48 + a;
        }
        else {
            charBuf[3 * i] = 55 + a;
        }

        a = (pBuff[i]) % 16;
        if (a >= 0 && a <= 9) {
            charBuf[3 * i + 1] = 48 + a;
        }
        else {
            charBuf[3 * i + 1] = 55 + a;
        }
        charBuf[3 * i + 2] = ' ';
    }
    UUIDLOGE("%s len is %d, uuid is %s", format, len, charBuf);
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
