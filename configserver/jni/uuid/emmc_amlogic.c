#include "uuid_imp.h"

#ifndef _EFUSE_H_
#define _EFUSE_H_

#include <linux/ioctl.h>
#include <ctype.h>
#include <errno.h>
#include <fcntl.h>
#include <limits.h>
#include <sys/stat.h>
#include <sys/wait.h>
#include <unistd.h>
#include <math.h>

#include <stdio.h>
#include <stdlib.h>
#include <dirent.h>
#include <malloc.h>
#include <asm/byteorder.h>
#include <linux/types.h>

#include <cutils/properties.h>
#include <cutils/efuse_bch_8.h>

#define EFUSE_ENCRYPT_DISABLE   _IO('f', 0x10)
#define EFUSE_ENCRYPT_ENABLE    _IO('f', 0x20)
#define EFUSE_ENCRYPT_RESET     _IO('f', 0x30)
#define EFUSE_INFO_GET          _IO('f', 0x40)

#define MAX_EFUSE_BYTES             512

#define EFUSE_NONE_ID           0
#define EFUSE_VERSION_ID        1
#define EFUSE_LICENCE_ID        2
#define EFUSE_MAC_ID                3
#define EFUSE_MAC_WIFI_ID   4
#define EFUSE_MAC_BT_ID     5
#define EFUSE_HDCP_ID           6
#define EFUSE_USID_ID               7

#if defined(MESON6) || defined(MESON8)
#define EFUSE_MACHINEID_ID          10
#endif

#if defined(MESON6) || defined(MESON8)
//M6 platform  or M8 platform
typedef struct efuseinfo_item{
    char title[40];
    unsigned id;
    // unsigned offset;    // write offset
    long long offset;  // write offset, match common/include/linux/efuse.h
    unsigned enc_len;
    unsigned data_len;
    int bch_en;
    int bch_reverse;
} efuseinfo_item_t;

typedef enum {
    EFUSE_NONE = 0,
    EFUSE_LICENCE,
    EFUSE_MAC,
    EFUSE_HDCP,
    EFUSE_MAC_BT,
    EFUSE_MAC_WIFI,
    EFUSE_USID,
    EFUSE_VERSION,
    EFUSE_MACHINEID,
    EFUSE_TYPE_MAX,
} efuse_type_t;

static char* efuse_title[EFUSE_TYPE_MAX] = {
    NULL,
    "licence",
    "mac",
    "hdcp",
    "mac_bt",
    "mac_wifi",
    "usid",
    "version",
    "machineid",
};

static unsigned int efuse_id[EFUSE_TYPE_MAX] = {
    EFUSE_NONE_ID,
    EFUSE_LICENCE_ID,
    EFUSE_MAC_ID,
    EFUSE_HDCP_ID,
    EFUSE_MAC_BT_ID,
    EFUSE_MAC_WIFI_ID,
    EFUSE_USID_ID,
    EFUSE_VERSION_ID,
    EFUSE_MACHINEID_ID,
};

#elif defined(MESON3)
//M3 platform
typedef struct efuseinfo_item{
    char title[40];
    unsigned id;
    unsigned offset;    // write offset
    unsigned data_len;
} efuseinfo_item_t;

typedef enum {
    EFUSE_NONE = 0,
    EFUSE_LICENCE,
    EFUSE_MAC,
    EFUSE_HDCP,
    EFUSE_MAC_BT,
    EFUSE_MAC_WIFI,
    EFUSE_USID,
    EFUSE_VERSION,
    EFUSE_TYPE_MAX,
} efuse_type_t;

static char* efuse_title[EFUSE_TYPE_MAX] = {
    NULL,
    "licence",
    "mac",
    "hdcp",
    "mac_bt",
    "mac_wifi",
    "usid",
    "version",
};

static unsigned int efuse_id[EFUSE_TYPE_MAX] = {
    EFUSE_NONE_ID,
    EFUSE_LICENCE_ID,
    EFUSE_MAC_ID,
    EFUSE_HDCP_ID,
    EFUSE_MAC_BT_ID,
    EFUSE_MAC_WIFI_ID,
    EFUSE_USID_ID,
    EFUSE_VERSION_ID,
};
#endif

#define EFUSE_DEVICE_NAME   "/dev/efuse"
#endif /* #endif _EFUSE_H_ */

//Add by wanxing.wwx
#ifdef VENDOR_MYGICA
efuse_type_t EFUSE_YUNOS_UUID = EFUSE_HDCP;
#else
efuse_type_t EFUSE_YUNOS_UUID = EFUSE_USID;
#endif

const char *efuse_dev = "/dev/efuse";

static int
efuse_opendev()
{
    int fd = open(efuse_dev, O_RDWR);
    if (fd < 0)
        UUIDLOGE("efuse device not found");
    return fd;
}

static void
efuse_closedev(int fd)
{
    close(fd);
}

static size_t efuse_read(int efuse_type, char* result_buffer, size_t buffer_size)
{
    loff_t ppos;
    size_t count;
    size_t read_size = 0;
    int fd = -1;
    efuseinfo_item_t efuseinfo_item;

    memset(&efuseinfo_item, 0, sizeof(efuseinfo_item_t));
    efuseinfo_item.id = efuse_id[efuse_type];

    fd  = efuse_opendev();

    if (fd >= 0) {
        if (ioctl(fd, EFUSE_INFO_GET, &efuseinfo_item)){
            UUIDLOGD("error, ioctl");
            goto error;
        }
        ppos = efuseinfo_item.offset;
        count = buffer_size;//efuseinfo_item.data_len;

        if (buffer_size > count) {
            UUIDLOGD("error, buffer size not enough");
            goto error;
        }
        lseek(fd, ppos, SEEK_SET);
        read_size = read(fd, result_buffer, count);
        UUIDLOGD("read_size = %d , count = %d,ppos = %d",read_size,count,ppos);
        if(read_size != count)
            goto error;

        efuse_closedev(fd);
    }

    return read_size;
error:
    UUIDLOGE("read efuse data %s error", efuse_title[efuse_type]);
    if(fd >= 0)
        efuse_closedev(fd);
    return -1 ;
}

static int efuse_written_check(int efuse_type)
{
    loff_t ppos;
    int count;
    size_t read_size = 0;
    int fd = -1, i, rc = 0;
    efuseinfo_item_t efuseinfo_item;
    char buffer[MAX_EFUSE_BYTES];

    memset(&efuseinfo_item, 0, sizeof(efuseinfo_item_t));
    efuseinfo_item.id = efuse_id[efuse_type];

    fd  = efuse_opendev();
    if (fd >= 0) {
        if (ioctl(fd, EFUSE_INFO_GET, &efuseinfo_item)) {
            UUIDLOGE("can't get efuse info");
            goto error;
        }

        ppos = efuseinfo_item.offset;
        count = efuseinfo_item.data_len;

        if (lseek(fd, ppos, SEEK_SET) == -1)
            goto error;

        memset(buffer, 0, MAX_EFUSE_BYTES);
        if(count != read(fd, buffer, count))
            goto error;

        for (i = 0; i < count; i++) {
            if (buffer[i]) {
                rc = 1;
                UUIDLOGE("this efuse segment has been written");
                break;
            }
        }

        efuse_closedev(fd);
    }

    return rc;

error:
    if(fd >= 0)
       efuse_closedev(fd);
    return -1 ;
}

static size_t efuse_write(int efuse_type, unsigned char *data, size_t buffer_size)
{
    size_t count;
    int ppos;
    size_t write_size = 0;
    int fd = -1;
    efuseinfo_item_t efuseinfo_item;

    memset(&efuseinfo_item, 0, sizeof(efuseinfo_item_t));
    efuseinfo_item.id = efuse_id[efuse_type];

    fd  = efuse_opendev();

    if (fd >= 0) {
        if (ioctl(fd, EFUSE_INFO_GET, &efuseinfo_item)) {
            UUIDLOGE("efuse ioctl error");
            goto error;
        }

        ppos = efuseinfo_item.offset;
        count = buffer_size;//efuseinfo_item.data_len;

        UUIDLOGE("efuse_write offset=%d, data_len=%d", ppos, count);

        if (buffer_size != count) {
            UUIDLOGE("error, efuse data %s format is wrong", efuse_title[efuse_type]);
            goto error;
        }
        if (lseek(fd, ppos, SEEK_SET) == -1)
            goto error;
        write_size = write(fd, data, buffer_size);
        if (write_size != buffer_size) {
            UUIDLOGE("error, efuse data %s write size wrong", efuse_title[efuse_type]);
            goto error;
        }
        efuse_closedev(fd);
    } else
    UUIDLOGE("error,%s open file failed", efuse_title[efuse_type]);

    return write_size;

error:
    if(fd >= 0)
       efuse_closedev(fd);
    return -1 ;
}

static int efuse_write_usid(char *usid_str, int len)
{
    if(usid_str == NULL || len <= 0)
    {
        return -1;
    }

    int i, rc = -1;
    unsigned char usid_data[512], usidData[512];
    int fd = -1;

    if (efuse_written_check(EFUSE_YUNOS_UUID)) {
        UUIDLOGE("%s written already or something error", efuse_title[EFUSE_YUNOS_UUID]);
        return -1;
    }
    UUIDLOGD("EFUSE_YUNOS_UUID will be writen in %s", efuse_title[EFUSE_YUNOS_UUID]);
    memset(usid_data, 0, sizeof(usid_data));
    memcpy(usid_data, usid_str, len);
    UUIDLOGD("usid_len=%d", len);
    UUIDLOGD("usid=%s ", usid_str);

    efuseinfo_item_t efuseinfo_item;
    memset(&efuseinfo_item, 0, sizeof(efuseinfo_item_t));
    efuseinfo_item.id = efuse_id[EFUSE_YUNOS_UUID];
    size_t count;
    fd = efuse_opendev();
    if (fd >= 0)
    {
        if (ioctl(fd, EFUSE_INFO_GET, &efuseinfo_item))
        {
            UUIDLOGD("efuse ioctl error");
            return rc;
        }
    }
    count = len;//efuseinfo_item.data_len;
    UUIDLOGD("usid data_len=%d", count);

    UUIDLOGD("========efuse write usid========");
    if(count == efuse_write(EFUSE_YUNOS_UUID, usid_data, count))
    {
        UUIDLOGD("efuse write usid in %s sucess", efuse_title[EFUSE_YUNOS_UUID]);
        rc = 0;                     //write sucess

        memset(usidData, 0, sizeof(usidData));
        if(count == efuse_read(EFUSE_YUNOS_UUID, (char*)usidData, count))
        {
            UUIDLOGD("test efuse read usid:read sucess,usid=%s", usidData);
            for(i=0; i<(int)count; i++)
            {
                if(usidData[i] != usid_data[i])
                {
                    rc = -1;
                    // if write success and read success,
                    // but read_data and write_data do not mach, then think to write fail
                    break;
                }
            }
        }
        else
            rc = 0;
            // if write success, but read fail, then think to write sucess

    }
    else
        rc = -1;
        // write fail

    return rc;
}

int efuse_read_usid(char *usid_str, int read_len)
{
    UUIDLOGD("usid read_len=%d\n", read_len);
    if(usid_str == NULL || read_len <= 0)
    {
        return -1;
    }

    int i, usid_flag = 0, rc = -1;
    int fd;

    efuseinfo_item_t efuseinfo_item;
    memset(&efuseinfo_item, 0, sizeof(efuseinfo_item_t));
    efuseinfo_item.id = efuse_id[EFUSE_YUNOS_UUID];
    size_t count;

    fd  = efuse_opendev();
    if(fd >= 0)
    {
        if (ioctl(fd, EFUSE_INFO_GET, &efuseinfo_item))
        {
            UUIDLOGD("efuse ioctl error\n");
            return rc;
        }
    }
    count = read_len;//efuseinfo_item.data_len;
    UUIDLOGD("usid data_len=%d\n", count);


    UUIDLOGD("========efuse read usid========\n");
    memset(usid_str, 0, read_len);
    if(count == (size_t)efuse_read(EFUSE_YUNOS_UUID, usid_str, count))
    {
        UUIDLOGD("efuse read usid:read sucess,usid=%s\n", usid_str);

        rc = -1;
        if(count >= (size_t)read_len)
        {
            for(i=0; i < read_len; i++)
            {
                if(usid_str[i] != 0x00)
                {
                    rc = 0;     //havefuse_read_uside readed usid
                    break;
                }
            }
        }
    }

    UUIDLOGD("efuse_read_suid return");
    return rc;
}

/**
  *  ---efuse_write_version
  *  @version_str: version(M6: 1 byte)
  *  return: 0->write success, -1->write failed
  */
int efuse_write_version(char* version_str)
{
    int rc = -1, version = -1;
    unsigned char version_data[1];
    char version_read[1];

    UUIDLOGD("prepate to write version=%s \n", version_str);
    memset(version_data, 0, sizeof(version_data));

    version_data[0] = strtol(version_str, NULL, 16);
    version = version_data[0];

    if(version == -1) {
        UUIDLOGD("m6 version wrong!: version=%d\n",version);
        return -1;
    }

    UUIDLOGD("version[1]={0x%02x}\n", version_data[0]);
    UUIDLOGD("%s:%d\n", __func__, __LINE__);
    if(1 == efuse_write(EFUSE_VERSION, version_data, 1 )) {
        rc = 0;
    }

    //write success,test efuse read
    if(rc == 0) {
        UUIDLOGD("efuse write version success,start to test efuse read version...\n");
        memset(version_read, 0, sizeof(version_read));
        if(1 == efuse_read(EFUSE_VERSION, version_read, 1)) {
            if(!memcmp(version_data, version_read, 1)) {
                UUIDLOGD("test efuse read version success,read version=%02x\n", version_read[0]);
                rc = 0;
            }
            else {
                UUIDLOGD("test efuse read version success,read version=%02x,but not mach write\n", version_read[0]);
                rc = -1;
            }
        }
    }

    return rc;
}
/**
  *  ---efuse_read_version
  *  @version_str: save version(M6: 1 byte) to this pointer
  *  return: 0->have writen before, 1->haven't write before, -1->read failed
  */
int efuse_read_version(char* version_str)
{
    int rc = -1;
    char versionData[1] = {0};

    /*modified by wanxing.wwx. The size version_str is just 1, memset 512 bytes will overflow the buffer */
    //memset(version_str, 0, 512);

    UUIDLOGD("%s:%d\n", __func__, __LINE__);
    if(1 == efuse_read(EFUSE_VERSION, (char*)versionData, 1)) {
        if(versionData[0] !=0x00) {
            rc = 0;                //have write version
            sprintf(version_str, "%02x", versionData[0]);
            UUIDLOGD("efuse read version success,version=%s\n", version_str);
        }
        else {
            rc = 1;                //haven't writen version
            UUIDLOGD("haven't write version before\n");
        }
    }

    return rc;
}
BOOL UUID_setFormatEmmcUuid_imp(unsigned char *uuid, int len)
{
    UUIDLOGD("UUID_setFormatEmmcUuid_imp in");

    char version_buf[1] = {0};
    UUIDLOGD("now we read efuse version");
    if( efuse_read_version(version_buf) ){
#if defined(MESON6)
            efuse_write_version("2");
#elif defined(MESON3)
            efuse_write_version("1");
#elif defined(MESON8)
            efuse_write_version("14");
#endif
    }

    int rst = efuse_write_usid((char *)uuid, len);
    UUIDLOGD("UUID_setFormatEmmcUuid_imp out");
    return (rst == 0);
}

unsigned char *UUID_getFormatEmmcUuid_imp( int uuid_len )
{
    UUIDLOGD("UUID_getFormatEmmcUuid_imp in");
    char *buf = malloc(uuid_len);
    if(buf == NULL)
    {
        return NULL;
    }
    UUIDLOGD("UUID_getFormatEmmcUuid_imp 1");
    if(efuse_read_usid(buf, uuid_len) == 0)
    {
        UUIDLOGD("UUID_getFormatEmmcUuid_imp out");
        return (unsigned char *)buf;
    }

    UUIDLOGD("UUID_getFormatEmmcUuid_imp 2");

    free(buf);

    UUIDLOGD("UUID_getFormatEmmcUuid_imp out");

    return NULL;
}

BOOL UUID_isOtpRom_imp( )
{
    return TRUE;
}
