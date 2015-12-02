/* Copyright Statement:
 *
 * This software/firmware and related documentation ("MediaTek Software") are
 * protected under relevant copyright laws. The information contained herein
 * is confidential and proprietary to MediaTek Inc. and/or its licensors.
 * Without the prior written permission of MediaTek inc. and/or its licensors,
 * any reproduction, modification, use or disclosure of MediaTek Software,
 * and information contained herein, in whole or in part, shall be strictly prohibited.
 */
/* MediaTek Inc. (C) 2010. All rights reserved.
 *
 * BY OPENING THIS FILE, RECEIVER HEREBY UNEQUIVOCALLY ACKNOWLEDGES AND AGREES
 * THAT THE SOFTWARE/FIRMWARE AND ITS DOCUMENTATIONS ("MEDIATEK SOFTWARE")
 * RECEIVED FROM MEDIATEK AND/OR ITS REPRESENTATIVES ARE PROVIDED TO RECEIVER ON
 * AN "AS-IS" BASIS ONLY. MEDIATEK EXPRESSLY DISCLAIMS ANY AND ALL WARRANTIES,
 * EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE IMPLIED WARRANTIES OF
 * MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE OR NONINFRINGEMENT.
 * NEITHER DOES MEDIATEK PROVIDE ANY WARRANTY WHATSOEVER WITH RESPECT TO THE
 * SOFTWARE OF ANY THIRD PARTY WHICH MAY BE USED BY, INCORPORATED IN, OR
 * SUPPLIED WITH THE MEDIATEK SOFTWARE, AND RECEIVER AGREES TO LOOK ONLY TO SUCH
 * THIRD PARTY FOR ANY WARRANTY CLAIM RELATING THERETO. RECEIVER EXPRESSLY ACKNOWLEDGES
 * THAT IT IS RECEIVER'S SOLE RESPONSIBILITY TO OBTAIN FROM ANY THIRD PARTY ALL PROPER LICENSES
 * CONTAINED IN MEDIATEK SOFTWARE. MEDIATEK SHALL ALSO NOT BE RESPONSIBLE FOR ANY MEDIATEK
 * SOFTWARE RELEASES MADE TO RECEIVER'S SPECIFICATION OR TO CONFORM TO A PARTICULAR
 * STANDARD OR OPEN FORUM. RECEIVER'S SOLE AND EXCLUSIVE REMEDY AND MEDIATEK'S ENTIRE AND
 * CUMULATIVE LIABILITY WITH RESPECT TO THE MEDIATEK SOFTWARE RELEASED HEREUNDER WILL BE,
 * AT MEDIATEK'S OPTION, TO REVISE OR REPLACE THE MEDIATEK SOFTWARE AT ISSUE,
 * OR REFUND ANY SOFTWARE LICENSE FEES OR SERVICE CHARGE PAID BY RECEIVER TO
 * MEDIATEK FOR SUCH MEDIATEK SOFTWARE AT ISSUE.
 *
 * The following software/firmware and/or related documentation ("MediaTek Software")
 * have been modified by MediaTek Inc. All revisions are subject to any receiver's
 * applicable license agreements with MediaTek Inc.
 */


#include <ctype.h>
#include <errno.h>
#include <fcntl.h>
#include <getopt.h>
#include <limits.h>
#include <linux/input.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/reboot.h>
#include <sys/types.h>
#include <time.h>
#include <unistd.h>
#include <cutils/properties.h>
#include <unistd.h>
#include <pthread.h>

#include "private/android_filesystem_config.h"

#include "libnvram.h"
#include "CFG_file_info_custom.h"

#include "uuid_imp.h"

#define MAX_RETRY_COUNT 20
#define UUID_OFFSET     630

#define ISREAD      1
#define ISWRITE     0

typedef enum
{
    WRITE,
    READ
}WR;

static BOOL nvramControl(WR wr, unsigned char *uuid, int len)
{
    if(uuid == NULL || len <= 0)
    {
        return FALSE;
    }

    int file_lid = AP_CFG_REEB_PRODUCT_INFO_LID;
    int read_nvram_ready_retry = 0;
    F_ID fid;
    int rec_size = 0;
    int rec_num = 0;
    char nvram_init_val[128] = {0};

    for(read_nvram_ready_retry = 0; read_nvram_ready_retry < MAX_RETRY_COUNT; read_nvram_ready_retry++)
    {
        property_get("nvram_init", nvram_init_val, NULL);
        if(strcmp(nvram_init_val, "Ready") == 0)
        {
            break;
        }
        else
        {
            usleep(500*1000);
        }
    }
    if(read_nvram_ready_retry >= MAX_RETRY_COUNT)
    {
        return FALSE;
    }

    /* read nvram */
    PRODUCT_INFO *barcode_struct = (PRODUCT_INFO *)malloc(sizeof(PRODUCT_INFO));
    if(barcode_struct == NULL)
    {
        return false;
    }

    fid = NVM_GetFileDesc(file_lid, &rec_size, &rec_num, ISREAD);
    if(fid.iFileDesc < 0)
    {
        return FALSE;
    }

    rec_size = rec_size < (int)sizeof(PRODUCT_INFO) ? rec_size : (int)sizeof(PRODUCT_INFO);
    if(rec_size != read(fid.iFileDesc, barcode_struct, rec_size))
    {
        free(barcode_struct);
        return FALSE;
    }
    NVM_CloseFileDesc(fid);
    /* read nvram end */


    int seek = lseek(fid.iFileDesc, 0, SEEK_SET);

    BOOL rst = TRUE;
    switch(wr)
    {
        case WRITE:
            {
                memcpy(barcode_struct->reserved.space + UUID_OFFSET, uuid, len);
                fid = NVM_GetFileDesc(file_lid, &rec_size, &rec_num, ISWRITE);
                if(fid.iFileDesc < 0 || rec_size < len)
                {
                    return FALSE;
                }

                rst = (sizeof(PRODUCT_INFO) == write(fid.iFileDesc, barcode_struct, sizeof(PRODUCT_INFO)));
                if(!NVM_CloseFileDesc(fid))
                {
                    rst = FALSE;
                }
            }
            break;

        case READ:
            memcpy(uuid, barcode_struct->reserved.space + UUID_OFFSET, len);
            break;

        default:
            rst = FALSE;
            break;
    }

    return rst;
}

BOOL UUID_setFormatEmmcUuid_imp(unsigned char *uuid, int len)
{
    return nvramControl(WRITE, uuid, len);
}

unsigned char *UUID_getFormatEmmcUuid_imp( int uuid_len )
{
    if(uuid_len <= 0)
    {
        return NULL;
    }

    unsigned char *uuid = malloc(uuid_len);
    if(uuid == NULL)
    {
        return NULL;
    }

    if(nvramControl(READ, uuid, uuid_len) == FALSE)
    {
        return NULL;
    }

    return uuid;
}

BOOL UUID_isOtpRom_imp( )
{
    /* NVRAM is not OTP Rom */
    return FALSE;
}

