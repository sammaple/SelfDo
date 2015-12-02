LOCAL_PATH:= $(call my-dir)
include $(CLEAR_VARS)

CHIP_OR_PRODUCT_LABEL:=$(PRODUCT_TV)

UUID_WRITE_DEBUG := 0
ifeq ($(UUID_WRITE_DEBUG), 1)
CHIP_OR_PRODUCT_LABEL:=memory_debug
endif

CHIP_OR_PRODUCT_LABEL:=$(strip $(CHIP_OR_PRODUCT_LABEL))
LOCAL_SRC_FILES :=
LOCAL_C_INCLUDES :=

######### use for write into memory
ifeq ($(CHIP_OR_PRODUCT_LABEL),memory_debug)
LOCAL_SRC_FILES+=emmc_memory_debug.c
endif

######### use nvram to save uuid
ifeq ($(CHIP_OR_PRODUCT_LABEL),MT6589)
LOCAL_SRC_FILES += nvram_mt6589.c
LOCAL_C_INCLUDES := \
    mediatek/custom/common/factory/inc \
    $(MTK_PATH_SOURCE)/external/nvram/libnvram
LOCAL_STATIC_LIBRARIES := libcutils
LOCAL_SHARED_LIBRARIES:= libnvram
endif

######### use for tv default
ifeq ($(CHIP_OR_PRODUCT_LABEL),default)
LOCAL_SRC_FILES += emmc_dev_io.c
LOCAL_CFLAGS += -DCHIP_A20
endif

######### use for tv amlogic
ifeq ($(CHIP_OR_PRODUCT_LABEL),amlogic_m6)
LOCAL_CFLAGS += -DMESON6
LOCAL_SRC_FILES += emmc_amlogic.c
    ifeq ($(YUNOS_VENDOR), mygica)
    LOCAL_CFLAGS += -DVENDOR_MYGICA
    endif
endif

######### use for tv rk3188
ifeq ($(CHIP_OR_PRODUCT_LABEL),rk3188)
    LOCAL_SRC_FILES += emmc_dev_io.c
    LOCAL_CFLAGS += -DCHIP_RK3188_RK3066
endif

######### use for tv rk3066
ifeq ($(CHIP_OR_PRODUCT_LABEL),rk3066)
    LOCAL_SRC_FILES += emmc_dev_io.c
    LOCAL_CFLAGS += -DCHIP_RK3188_RK3066
endif

######### use for tv Hi3716M
ifeq ($(CHIP_OR_PRODUCT_LABEL),Hi3716M)
    MYLOCAL_SDK_PATH  := $(ANDROID_BUILD_TOP)/device/hisilicon/godbox/driver/sdk

    LOCAL_SRC_FILES += emmc_hi3716m.c \
                       emmc_hi3716m_crc32.c

    LOCAL_C_INCLUDES += $(MYLOCAL_SDK_PATH)/msp_base/ecs/include
    LOCAL_C_INCLUDES += $(MYLOCAL_SDK_PATH)/msp_base/common/include
    LOCAL_SHARED_LIBRARIES := libhi_common
endif

######### use for tv HISI3716CV200
ifeq ($(CHIP_OR_PRODUCT_LABEL),Hi3716CV200)
    MYLOCAL_SDK_PATH  := $(ANDROID_BUILD_TOP)/device/hisilicon/bigfish/sdk
    LOCAL_SRC_FILES += emmc_hisi3716cv200.c \
                       emmc_hisi3716cv200_crc32.c
    LOCAL_C_INCLUDES += $(MYLOCAL_SDK_PATH)/pub/include
    LOCAL_C_INCLUDES += $(MYLOCAL_SDK_PATH)/source/common/include
    LOCAL_C_INCLUDES += $(MYLOCAL_SDK_PATH)/source/msp/include
    LOCAL_SHARED_LIBRARIES := libhi_otp libcutils
endif


######### use for tv Hi3718CV100
ifeq ($(CHIP_OR_PRODUCT_LABEL),HISI3718CV100)
    MYLOCAL_SDK_PATH  := $(ANDROID_BUILD_TOP)/device/hisilicon/bigfish/sdk

    LOCAL_SRC_FILES += emmc_hisi3718cv100.c

    LOCAL_C_INCLUDES += $(MYLOCAL_SDK_PATH)/pub/include
    #LOCAL_C_INCLUDES += $(MYLOCAL_SDK_PATH)/msp_base/common/include
    LOCAL_SHARED_LIBRARIES := libhi_common libcutils
endif


######### use for tv wm8880
ifeq ($(CHIP_OR_PRODUCT_LABEL),wm8880)
    LOCAL_SRC_FILES += emmc_wm8880.c
    LOCAL_SHARED_LIBRARIES := libwmtenv
endif

######### use for tv MSD6A801
ifeq ($(CHIP_OR_PRODUCT_LABEL),MSD6A801)
    LOCAL_SRC_FILES += emmc_msd6a801.c
    LOCAL_C_INCLUDES += \
      hardware/mstar/libutopia/$(TARGET_BOARD_PLATFORM)/include
    LOCAL_WHOLE_STATIC_LIBRARIES := libutopia
endif

######### use for tv  gs702a
ifeq ($(CHIP_OR_PRODUCT_LABEL),gs702a)
    LOCAL_SRC_FILES += emmc_gs702.c
endif

######### use for tv  iMAPx912
ifeq ($(CHIP_OR_PRODUCT_LABEL),iMAPx912)
    LOCAL_SRC_FILES += emmc_infotm.c
    LOCAL_SHARED_LIBRARIES := liblog
endif


####### use for tv RTD1815
ifeq ($(CHIP_OR_PRODUCT_LABEL), RTD1815)
	LOCAL_SRC_FILES += emmc_rtd1815.c
endif

######## No right chip/product
ifeq ($(strip $(LOCAL_SRC_FILES)),)
    LOCAL_SRC_FILES += emmc_default.c
endif # end of emmc define

ifneq ($(TARGET_BUILD_VARIANT),user)
LOCAL_CFLAGS += -DDEBUG_UUID -DUUID_DEAMON_DEBUG
endif

LOCAL_MODULE_TAGS := optional
LOCAL_MODULE := libyunosfwnative
LOCAL_CFLAGS += -fvisibility=hidden

LOCAL_STATIC_LIBRARIES += libcutils libc liblog

LOCAL_SRC_FILES += uuid_stub_imp.c
LOCAL_C_INCLUDES += $(LOCAL_PATH) \
     $(TOPDIR)yunos/frameworks/base/include

include $(BUILD_SHARED_LIBRARY)
