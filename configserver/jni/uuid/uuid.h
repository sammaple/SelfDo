/*
 * The chip API for ali UUID
 */

#ifndef __ALI_UUID_H__
#define __ALI_UUID_H__

#ifdef LOG_TAG
#undef LOG_TAG
#endif /* #endif LOG_TAG */
#define LOG_TAG         "ALI_UUID"

#define BOOL        int
#define TRUE        1
#define FALSE       0


#define IN
#define OUT
#define INOUT

#define LOG_TAG "ALI_UUID"

/**
 * Return uuid from emmc
 * Get the UUID value by the length of uuid_len
 */
extern unsigned char *UUID_getFormatEmmcUuid(IN int uuid_len) __attribute__((visibility("default")));

/**
 * Set uuid into emmc, return TRUE if successfully
 * Save UUID to EMMC
 */
extern BOOL UUID_setFormatEmmcUuid(INOUT unsigned char *uuid, IN int len) __attribute__((visibility("default")));

/**
 * Verify OTP is supported to write uuid
 */
extern BOOL UUID_isOtpRom( ) __attribute__((visibility("default")));

#endif /* #endif __ALI_UUID_H__ */
