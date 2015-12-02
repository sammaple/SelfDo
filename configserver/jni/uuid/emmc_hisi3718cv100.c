/***********************************************************************************
 *              Copyright 2004 - 2050, himedia Tech. Co., Ltd.
 *                           ALL RIGHTS RESERVED
 * FileName: deviceinfo.c
 * Description:´ËļþÖҪÓÓ¶ÔlashÉ±¸Ì¶¨µĵط½¶Á´²Ù����öʺÏڰ¢ÀÔosƽ̨
 *
 * History:
 * Version   Date             Author        DefectNum    Description
 * main\1    2013-10-30                  wukl         1.0         Create this file.
******************************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>


#include <stdio.h>
#include <stdlib.h>
#include <cutils/log.h>

#include "uuid_imp.h"

#include "hi_flash.h"
#include "hi_type.h"

#define BASEADDRESS (2*1024*1024-8*1024)//¶Á´Ê¾ݵĵØ·
#define MAXLEN (8*1024) //¶Á´Ê¾ݵÄî³¤¶È
/*
*    Hi_Flash_Read£º¶Á¡flashÏ¹ØøϢ
* ÊÈ²Îý        pBuf£º  ¶Á¡Ê¾ݺóuffer
*            u32Len£º¶Á¡Ê¾ݵĳ¤¶ȣ¬±ØëС8*1024¸öÚ* ·µ»زÎý        ´ó£º    ˵Ã¶Á¡³ɹ¦£¬²¢ÇÊʵ¼ʶÁ¡µĳ¤¶È*            СÓ»òÚ£º˵Ã¶Á¡Ê¾Ý§°Ü*/
int Hi_Flash_Read(unsigned char *pBuf,unsigned int u32Len)
{
    HI_S32 ret = 0;
    HI_HANDLE fd = INVALID_FD;

    if ( NULL == pBuf || u32Len > MAXLEN )
    {
            UUIDLOGD("> %d Line: Arg Error!\n",__LINE__);
            return 0;
    }

    fd = HI_Flash_OpenByName("deviceinfo");
    if (INVALID_FD == fd)
    {
        UUIDLOGD("> %d LineHI_Flash_OpenByName failure!\n",__LINE__);
        return -1;
    }
    memset(pBuf, '\0', u32Len);
    ret = HI_Flash_Read(fd,BASEADDRESS,pBuf,u32Len,HI_FLASH_RW_FLAG_RAW);
    HI_Flash_Close(fd);
    return ret;
}


/*
*    Hi_Flash_Write£º°ÑַûÈµ½flashÏ¹Øø ÊÈ²Îý        pBuf£º  дÈÊ¾Ýuffer
*            u32Len£ºдÈÊ¾ݵĳ¤¶ȣ¬±ØëС8*1024¸öÚ* ·µ»زÎý        ´ó£º    ˵ÃдÈ³ɹ¦£¬²¢ÇÊʵ¼Ê´Èµĳ¤¶È*            СÓ»òÚ£º˵Ã¶Á¡Ê¾Ý§°Ü*/
int Hi_Flash_Write(unsigned char *pBuf,unsigned int u32Len)
{
    HI_S32 ret = 0;
    HI_HANDLE fd = INVALID_FD;
    if ( NULL == pBuf || u32Len > MAXLEN )
    {
            UUIDLOGD("> %d Line: Arg Error!\n",__LINE__);
            return 0;
    }
    fd = HI_Flash_OpenByName("deviceinfo");
    if (INVALID_FD == fd)
    {
        UUIDLOGD("> %d LineHI_Flash_OpenByName failure!\n",__LINE__);
        return -1;
    }
    ret = HI_Flash_Write(fd,BASEADDRESS,pBuf,u32Len,HI_FLASH_RW_FLAG_RAW);
    HI_Flash_Close(fd);
    return ret;
}

//for test ,ֱ½Ó´ÈÁ×·û¯4²â¶Á¡ºÍ´È
int main1(int argc, char *argv[])
{
    int index = 0;
    int ret = 0;
    unsigned char mReadBuf[MAXLEN];

    //µÚ»´ζÁ¡
    memset(mReadBuf, '\0', MAXLEN);
    ret = Hi_Flash_Read(mReadBuf,MAXLEN);
    if ( 0 >= ret )
    {
        UUIDLOGD("First Hi_Flash_Read fail!\n");
    }
    else
    {
        UUIDLOGD(" First Hi_Flash_Read len is %d\n",ret);

        for ( index = 0 ; index < ret; ++index)
        {
            UUIDLOGD("%d:0x%x\n",index,mReadBuf[index]);
        }
    }


    //дÈÊ¾Ý    for ( index = 0; index < MAXLEN; ++index)
    {
        mReadBuf[index] = 'a';
    }
    ret = 0;
    ret= Hi_Flash_Write(mReadBuf,MAXLEN);
    if ( 0 >= ret )
    {
        UUIDLOGD("Hi_Flash_Write fail ret %d !\n",ret);
    }
    else
    {
        UUIDLOGD("Hi_Flash_Write success ret %d!\n",ret);
    }

    //µڶþ´ζÁ¡
    ret = 0;
    memset(mReadBuf, '\0', MAXLEN);
    ret = Hi_Flash_Read(mReadBuf,MAXLEN);
    if ( 0 >= ret )
    {
        UUIDLOGD("Second Hi_Flash_Read fail ret %d !\n",ret);
    }
    else
    {
        UUIDLOGD(" Second Hi_Flash_Read len is %d\n",ret);

        for ( index = 0 ; index < ret; ++index)
        {
            UUIDLOGD("%d:0x%x\n",index,mReadBuf[index]);
        }
    }
    return 0;
}

BOOL UUID_setFormatEmmcUuid_imp(unsigned char *uuid, int len)
{
      if(uuid == NULL || len <= 0 || len>MAXLEN)
      {
          return FALSE;
      }

    if(Hi_Flash_Write(uuid, len)>0){
        return TRUE;
    }else{
        return FALSE;
    }
}

static unsigned char _uuid_data[MAXLEN] = {0};
unsigned char *UUID_getFormatEmmcUuid_imp( int uuid_len )
{
         if((uuid_len <= 0) || (uuid_len > MAXLEN))
      {
          return NULL;
      }

    if(Hi_Flash_Read(_uuid_data, uuid_len)>0){
        return _uuid_data;
    }else{
        return NULL;
    }
}

BOOL UUID_isOtpRom_imp( )
{
    return FALSE;
}

