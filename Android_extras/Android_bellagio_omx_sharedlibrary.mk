LOCAL_PATH := $(call my-dir)
include $(CLEAR_VARS)

LOCAL_WHOLE_STATIC_LIBRARIES := \
	libomxil-bellagio_lib

LOCAL_MODULE := libomxil-bellagio_sharedlibrary
LOCAL_PRELINK_MODULE := false

-include $(PV_TOP)/Android_platform_extras.mk
-include $(PV_TOP)/Android_system_extras.mk

include $(BUILD_SHARED_LIBRARY)
include   $(PV_TOP)/codecs_v2/omx/omx_sharedlibrary/omxil/Android.mk
