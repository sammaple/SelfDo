

#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <sys/mount.h>

#include <android/log.h>
#include "uuid_imp.h"

#ifdef CHIP_A20
    #define UUID_FILE       "/mnt/private/uuid_2"
    #define DEVICE          "/dev/block/private"
    #define TARGET_DIR      "/mnt/private"
    #define FSTYPE          "vfat"
#endif

#ifdef CHIP_RK3188_RK3066
    #define UUID_FILE       "/mnt/oem/uuid_2"
    #define DEVICE          "/dev/block/oem"
    #define TARGET_DIR      "/mnt/oem"
    #define FSTYPE          "ext4"
#endif

BOOL UUID_setFormatEmmcUuid_imp(unsigned char *uuid, int len)
{
    if(uuid == NULL || len == 0)
    {
        return FALSE;
    }

    /*int mountfd = 0;
    char* dev = find_mount(TARGET_DIR);*/
    /*mount DEVICE rw*/
    /*if(dev != NULL){
        mountfd = mount(DEVICE, TARGET_DIR, FSTYPE, MS_REMOUNT|MS_SYNCHRONOUS, NULL);
    }else{
        mountfd = mount(DEVICE, TARGET_DIR, FSTYPE, MS_SYNCHRONOUS, NULL);
    }
    if(mountfd != 0){
        UUIDLOGD("UUID::mount %s rw failed. error info is %s\n", DEVICE, strerror(errno));
        return FALSE;
    }else{
        UUIDLOGD("UUID::mount %s rw succeed.\n", DEVICE);
    }*/
    /* write uuid */
    FILE *fp = fopen(UUID_FILE, "wb");
    if(fp == NULL)
    {
        return FALSE;
    }

    int writed = fwrite(uuid, 1, len, fp);
    fclose(fp);
    /*chmod 755
    chmod(UUID_FILE,0755);
    UUIDLOGD("UUID::chmod 0755.\n");*/

    /*mount DEVICE ro*/
    /*mountfd = 0;
    if(dev != NULL){
        mountfd = mount(DEVICE, TARGET_DIR, FSTYPE, MS_REMOUNT|MS_RDONLY, NULL);
    }else{
        mountfd = mount(DEVICE, TARGET_DIR, FSTYPE, MS_RDONLY, NULL);
    }
    if(mountfd != 0){
        UUIDLOGD("UUID::mount %s ro failed. error info is %s\n", DEVICE, strerror(errno));
    }else{
        UUIDLOGD("UUID::mount %s ro succeed.\n", DEVICE);
    }*/

    if(writed != len)
    {
        return FALSE;
    }

    /* check uuid */
    unsigned char *buf = UUID_getFormatEmmcUuid_imp(len);
    if(buf == NULL)
    {
        return FALSE;
    }
    int rst = !memcmp(buf, uuid, len);
    free(buf);

    return rst;
}

unsigned char *UUID_getFormatEmmcUuid_imp( int uuid_len )
{
    if(uuid_len <= 0)
    {
        return NULL;
    }
    unsigned char *buf = malloc(uuid_len);
    if(buf == NULL)
    {
        return NULL;
    }

    FILE *fp = fopen(UUID_FILE, "rb");
    if(fp == NULL)
    {
        free(buf);
        return NULL;
    }
    fread(buf, 1, uuid_len, fp);
    fclose(fp);

    return buf;
}

BOOL UUID_isOtpRom_imp( )
{
    return FALSE;
}



