
#include <stdio.h>
#include <stdlib.h>

#include "uuid_imp.h"

#define OTP_MMC_BLOCK_START 19552

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
    FILE *fp;
    int i;
    int ret = -1;

   if(uuid == NULL || len <= 0)
      {
          return FALSE;
      }

    fp  = fopen ( "/dev/block/mmcblk0",  "w" );
    if (fp == NULL)
        return FALSE;
    fseek (fp, (OTP_MMC_BLOCK_START * 512), SEEK_SET);

    for(i=0;i<len;i++){
    fputc(uuid[i] , fp);
    }


    fsync(fp);
    fclose(fp);

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
    // TODO: needs to be done
    FILE *fp;
    int i;
    unsigned char *buffer=NULL;

         if(uuid_len <= 0)
      {
          return NULL;
      }

    buffer=(unsigned char *)malloc(uuid_len*sizeof(unsigned char));
    if(buffer==NULL)
        return NULL;

    fp  = fopen ( "/dev/block/mmcblk0",  "r" );
    if (fp == NULL)
        return NULL;
    fseek (fp, (OTP_MMC_BLOCK_START * 512), SEEK_SET);
    fread(buffer, uuid_len, 1, fp);
    fclose(fp);


    // make sure free the buffer after not use it anymore.
    return buffer;
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

    return FALSE;
}

//for test only
int main1(int argc,char *argv[]){
    unsigned char uuid[24]={1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16,17,18,19,20,21,22,23,24};
    unsigned char *myuuid;
    int i;

    UUID_setFormatEmmcUuid_imp(uuid,24);
    myuuid=UUID_getFormatEmmcUuid_imp(24);
    UUIDLOGD("UUID:\n");
    for(i=0;i<24;i++){
       UUIDLOGD("%d ",myuuid[i]);
    }
    return 0;
}
