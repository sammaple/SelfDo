
#include <stdio.h>
#include <stdlib.h>

#include "uuid_imp.h"

#define UUID_ZONE_SIZE    64
static unsigned char UUID_ZONE[UUID_ZONE_SIZE];
static BOOL uuid_writed = FALSE;

BOOL UUID_setFormatEmmcUuid_imp(unsigned char *uuid, int len)
{
    UUIDLOGI("this is UUID_WRITE_DEBUG mode, we write uuid into memory.");
    UUIDLOGI("uuid = %s", uuid);
    memset(UUID_ZONE, 0, UUID_ZONE_SIZE);
    memcpy(UUID_ZONE, uuid, len);
    uuid_writed = TRUE;

    return TRUE;
}

unsigned char *UUID_getFormatEmmcUuid_imp( int uuid_len )
{
    UUIDLOGI("this is UUID_WRITE_DEBUG mode, we read uuid from memory");
    if(uuid_writed) {
        UUIDLOGI("uuid = %s", UUID_ZONE);
        return UUID_ZONE;
    } else {
        UUIDLOGI("uuid is null");
        return NULL;
    }
}

BOOL UUID_isOtpRom_imp( )
{
    return TRUE;
}
