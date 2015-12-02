
#include <stdio.h>
#include <stdlib.h>
#include <cutils/log.h>
#include <fcntl.h>

#include "uuid_imp.h"

#include <sys/ioctl.h>

#define BELT_MAGIC 'b'
#define BELT_ENV_GET    _IOW(BELT_MAGIC, 5, unsigned long)
#define BELT "/dev/belt"
#define INFOTM_UUID "ro.uuid"


BOOL UUID_setFormatEmmcUuid_imp(unsigned char *uuid, int len)
{
//    UUIDLOGD("in %s %d %s", __func__, len, uuid);
     if (len <=0 || len > 64 || uuid == NULL)
    {
        UUIDLOGE("invalid input parametsers");
        return FALSE;
    }

    unsigned char pres[96]="env put ro.uuid ";
        char commit[16]="env commit";
    strcat((char*)pres, (char*)uuid);
    int fd = open(BELT, O_RDWR);
    if (fd > 0)
    {
        write(fd, (void*)pres, 96);
        write(fd, (void*)commit, 16);
    }
    else{
        UUIDLOGE("cant open belt");
        return FALSE;
    }
//    UUIDLOGD("out %s", __func__);
    return TRUE;
}

unsigned char *UUID_getFormatEmmcUuid_imp( int uuid_len )
{
//    UUIDLOGD("in %s %d", __func__, uuid_len);
     if (uuid_len <=0 || uuid_len >64)
    {
        UUIDLOGE("invalid input parametsers");
        return NULL;
    }
    unsigned char uuid[64] = "ro.uuid";
    unsigned char *UUID  = NULL;
    UUID = (unsigned char*)malloc(256);
    int fd = open(BELT, O_RDWR);
    if (fd > 0)
    {
        strncpy((char*)UUID, (char*)uuid, 64);
        ioctl(fd, BELT_ENV_GET, UUID);
    }
    else{
        UUIDLOGE("cant open belt");
        return NULL;
    }
    if(!strcmp(UUID, uuid)){
        UUIDLOGD("out %s null uuid" , __func__);
        return NULL;
    }
    //strcpy((char*)UUID, (char*)uuid);
//    UUID[255] = '\0';
//    UUIDLOGD("out %s UUID = %s" , __func__, UUID);
    return UUID;
}

BOOL UUID_isOtpRom_imp( )
{
        return TRUE;
}

/*int main()
{
    unsigned char uuid[32]="1234567890123456789012345678901";
    UUID_setFormatEmmcUuid_imp(uuid, 32);
    UUIDLOGD("uuid = %s", UUID_getFormatEmmcUuid_imp(32));
    unsigned char uuid1[32]="0123456789012345678901234567890";
    UUID_setFormatEmmcUuid_imp(uuid1, 32);
    UUIDLOGD("uuid = %s", UUID_getFormatEmmcUuid_imp(32));
    return TRUE;
}*/
