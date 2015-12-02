
#include <stdio.h>
#include <stdlib.h>
#include "drvMMIO.h"
#include "uuid_imp.h"
#define UUID_MAX_LENGTH 128
#define debug 0

static unsigned int u32NonPmBase;
static volatile BOOL OTP_init = FALSE ;

static BOOL UUID_init(void)
{
   unsigned long int  u32MMIOBaseAdr, u32BankSize;
   if(OTP_init == TRUE)
   {
        return TRUE;
   }
   if(!MDrv_MMIO_Init())
   {
        UUIDLOGD("MDrv_MMIO_Init FALSE\n");
        return FALSE;
   }
   if(!MDrv_MMIO_GetBASE(&u32MMIOBaseAdr, &u32BankSize, 0))
   {
        UUIDLOGD("MDrv_MMIO_GetBASE FALSE\n");
        return FALSE;
   }
   u32NonPmBase = u32MMIOBaseAdr;
   //UUIDLOGD("\033[36m u32NonPmBase is 0x%08x \033[m\n",u32NonPmBase);
   OTP_init = TRUE;
   return TRUE;
}

static BOOL UUID_IsEmpty(void)
{
    unsigned char *OTP_buf;
    int i = 0;
    OTP_buf = UUID_getFormatEmmcUuid_imp(UUID_MAX_LENGTH);
    for(i=0;i<UUID_MAX_LENGTH;i++)
    {
        if(OTP_buf[i] != 0)
        return FALSE;
    }
    return TRUE;
}
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
    unsigned char i = 0;
    unsigned char shift_num = 0;
    BOOL IS_UUID_Empty = UUID_IsEmpty();
    if(!OTP_init)
    {
      if( !UUID_init())
      {
        UUIDLOGD("UUID_init FALSE\n");
        return FALSE;
      }
    }
    volatile unsigned int  *OtpWrRdBase = (unsigned int *)(u32NonPmBase + ((0x38000 + 3588) << 2));
    if( (uuid == NULL) || (len > UUID_MAX_LENGTH) )
    {
        if(len > UUID_MAX_LENGTH)
        {
             UUIDLOGD("UUID_setFormatEmmcUuid_imp len is too long \n");
        }
        return FALSE;
    }
#if(debug == 0)
    if(!UUID_IsEmpty())
    {
        UUIDLOGD("UUID had writed before \n");
        return FALSE;
    }
#endif
    for(i = 0;i < len; i++ )
    {
        if(i%4 == 0 && i != 0)
        {
            OtpWrRdBase += 1;
        }
        shift_num = (i%4)*8 ;
        *OtpWrRdBase = (unsigned int)uuid[i] << shift_num;
    }
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
{   unsigned int ReadValue;
    unsigned char i = 0;
    unsigned char shift_num = 0;
    static unsigned char otp_buffer[UUID_MAX_LENGTH];
    if( !OTP_init)
    {
      if( !UUID_init() )
      {
        UUIDLOGD("UUID_init FALSE\n");
        return FALSE;
      }
    }
    volatile unsigned int  *OtpWrRdBase = (unsigned int *)(u32NonPmBase + ((0x38000 + 3588) << 2));
    if( uuid_len > UUID_MAX_LENGTH)
    {
        UUIDLOGD("UUID_setFormatEmmcUuid_imp len is too long \n");
        return NULL;
    }
    for(i = 0; i < UUID_MAX_LENGTH; i++ )
    {
        otp_buffer[i] = 0;
    }
    for(i = 0;i < uuid_len; i++ )
    {
        if(i%4 == 0 && i != 0)
        {
            OtpWrRdBase += 1;
        }
        shift_num = (i%4)*8 ;
        otp_buffer[i] =  ( (*OtpWrRdBase) >> shift_num)&0xFF ;
    }
    return otp_buffer;
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
BOOL UUID_isOtpRom_imp( void )
{
    // uuid_isotpRom
    return TRUE;
}
