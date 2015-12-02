#ifdef __cplusplus
extern "C" {
#endif

/*
 * The API for ali vendor id
 */

#ifndef __ALI_VENDORID_H__
#define __ALI_VENDORID_H__

/**
 * Return vendor id
 */
extern int  getVendorID()  __attribute__((visibility("default")));
extern int setVendorID(int vendorID) __attribute__((visibility("default")));

#endif /* #endif __ALI_VENDORID_H__ */

#ifdef __cplusplus
}
#endif
