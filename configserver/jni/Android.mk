#include $(CLEAR_VARS)  
#LOCAL_MODULE := yunosfwnative_
#LOCAL_SRC_FILES := libyunosfwnative.so  
##LOCAL_SRC_FILES := ../../sojni/libs/armeabi/libinit.so  
#include $(PREBUILT_SHARED_LIBRARY) 

LOCAL_PATH:= $(call my-dir)
include $(CLEAR_VARS)

LOCAL_SRC_FILES:= \
	configserver.c

#LOCAL_SHARED_LIBRARIES:= \
#	libutils \
#	libbinder \
#	liblog \
#	libyunosfwnative

#LOCAL_LDLIBS := -lyunosfwnative -llog -lbinder -lutils
#LOCAL_LDFLAGS := -shared
#LOCAL_EXPORT_LDLIBS:=$(LOCAL_LDLIBS)

#LOCAL_LDFLAGS := -L$(LOCAL_PATH)/lib/libyunosfwnative.so 
   
LOCAL_C_INCLUDES := $(LOCAL_PATH)/uuid/
MYLOCAL_SDK_PATH  := $(ANDROID_BUILD_TOP)/device/hisilicon/bigfish/sdk
LOCAL_C_INCLUDES += $(MYLOCAL_SDK_PATH)/pub/include
LOCAL_C_INCLUDES += $(LOCAL_PATH) \
     $(TOPDIR)yunos/frameworks/base/include
       
ifeq ($(TARGET_OS),linux)
	LOCAL_CFLAGS += -DXP_UNIX
	#LOCAL_SHARED_LIBRARIES += librt
endif

LOCAL_MODULE:= jconfigserver

include $(BUILD_EXECUTABLE)
