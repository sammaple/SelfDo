/*
 * The chip API for ali UUID
 */

#ifndef __ALI_UUID_IMP_H__
#define __ALI_UUID_IMP_H__

#include <stdio.h>
#include <stdlib.h>
#include <android/log.h>

#include "uuid.h"

/*
 * Get the UUID value by the length of uuid_len
 */
extern unsigned char *UUID_getFormatEmmcUuid_imp( IN int uuid_len );
/*
 * Save UUID to EMMC
 */
extern BOOL UUID_setFormatEmmcUuid_imp( INOUT unsigned char *uuid, IN int len );
/*
 * is OtpRom
 */
extern BOOL UUID_isOtpRom_imp( );

#ifdef DEBUG_UUID
#define UUIDLOGD(fmt, args...)     __android_log_print(ANDROID_LOG_DEBUG, LOG_TAG, "-@%d     [DBG]    "fmt"\n", __LINE__, ##args)
#else
#define UUIDLOGD(fmt, args...)
#endif /* #endif DEBUG_UUID */
#define UUIDLOGI(fmt, args...)     __android_log_print(ANDROID_LOG_DEBUG, LOG_TAG, "-@%d     [INFO]    "fmt"\n", __LINE__, ##args)
#define UUIDLOGE(fmt, args...)     __android_log_print(ANDROID_LOG_ERROR, LOG_TAG, "-@%d     [ERR]    "fmt"\n", __LINE__, ##args)

#define ROOT_PERMISSION_CHECK() do{\
    if(0!=getuid()){\
        UUIDLOGE("Invoke by uid=%d",getuid());\
        exit(1);\
    }\
}while(0)

#endif /* #endif __ALI_UUID_IMP_H__ */
