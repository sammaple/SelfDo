typedef unsigned char UI8;
typedef unsigned short UI16;
typedef unsigned long UI32;

#define P32 0xEDB88320L
static int crc_tab32_init = 0;
static UI32 crc_tab32[256];

static void init_crc32_tab( void )
{
    int i,j;
    UI32 crc;

    for (i=0; i<256; i++) {
        crc = (UI32) i;
        for (j=0; j<8; j++) {
            if (crc & 0x00000001L) {
                crc = (crc >> 1) ^ P32;
            } else {
                crc = crc >> 1;
            }
        }
        crc_tab32[i] = crc;
    }
    crc_tab32_init = 1;
}

static UI32 update_crc_32(UI32 crc, char c)
{
    UI32 tmp, long_c;

    long_c = 0x000000ffL & (UI32) c;
    tmp = crc ^ long_c;
    crc = (crc >> 8) ^ crc_tab32[tmp & 0xff];
    return crc;
}

unsigned int CalcCRC32( unsigned char* pBuff,unsigned int length)
{
    UI32 crc32;
    UI32 i;
    crc32 = 0xffffffffL;

    if (!crc_tab32_init) {
        init_crc32_tab();
    }
    for(i=0 ; i < length ; i++) {
        crc32 = update_crc_32(crc32,(char)pBuff[i]);
    }
    crc32 ^= 0xffffffffL;
    return crc32;
}
