#include "uuid_imp.h"

/*
 * Get the UUID value by the length of uuid_len
 */
unsigned char *UUID_getFormatEmmcUuid( IN int uuid_len )
{
    ROOT_PERMISSION_CHECK();
    return UUID_getFormatEmmcUuid_imp(uuid_len);
}
/*
 * Save UUID to EMMC
 */
BOOL UUID_setFormatEmmcUuid( INOUT unsigned char *uuid, IN int len )
{
    ROOT_PERMISSION_CHECK();
    return UUID_setFormatEmmcUuid_imp(uuid,len);
}
/*
 * is OtpRom
 */
BOOL UUID_isOtpRom( )
{
    ROOT_PERMISSION_CHECK();
    return UUID_isOtpRom_imp();
}
