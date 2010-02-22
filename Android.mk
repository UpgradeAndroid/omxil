LOCAL_PATH := $(call my-dir)

#Building omxregister-bellagio binary which will be placed in the /system/bin folder
include $(CLEAR_VARS)

LOCAL_SRC_FILES := \
	src/common.c \
	src/omxregister.c \
	src/omxregister.h 

LOCAL_MODULE := omxregister-bellagio

LOCAL_CFLAGS :=  -DOMXILCOMPONENTSPATH=\"/system/lib\"

LOCAL_C_INCLUDES := \
	$(PV_TOP)/codecs_v2/omx/omx_mastercore/src \
 	$(PV_TOP)/codecs_v2/omx/omx_mastercore/include \
 	$(PV_TOP)/codecs_v2/omx/omx_common/include \
 	$(PV_TOP)/extern_libs_v2/khronos/openmax/include \
	$(PV_TOP)/codecs_v2/omx/omx_sharedlibrary/omxil/src \
	$(PV_TOP)/codecs_v2/omx/omx_sharedlibrary/omxil/src/base \
 	$(PV_INCLUDES)

LOCAL_ARM_MODE := arm

include $(BUILD_EXECUTABLE)

# Building the libomxil-bellagio_lib 
include $(CLEAR_VARS)

LOCAL_SRC_FILES := \
 	src/pv_omx_interface.cpp \
	src/common.c \
	src/content_pipe_file.c \
	src/content_pipe_inet.c \
	src/omx_create_loaders_linux.c \
	src/omxcore.c \
	src/omxregister.c \
	src/queue.c \
	src/st_static_component_loader.c \
	src/tsemaphore.c \
	src/base/omx_base_audio_port.c \
	src/base/omx_base_clock_port.c \
	src/base/omx_base_component.c \
	src/base/omx_base_filter.c \
	src/base/omx_base_image_port.c \
	src/base/omx_base_port.c \
	src/base/omx_base_sink.c \
	src/base/omx_base_source.c \
	src/base/omx_base_video_port.c

LOCAL_MODULE := libomxil-bellagio_lib

LOCAL_CFLAGS :=  -DOMXILCOMPONENTSPATH=\"lib\" $(PV_CFLAGS)

LOCAL_ARM_MODE := arm

LOCAL_SHARED_LIBRARIES := libutils

LOCAL_C_INCLUDES := \
	$(PV_TOP)/codecs_v2/omx/omx_mastercore/src \
 	$(PV_TOP)/codecs_v2/omx/omx_mastercore/include \
 	$(PV_TOP)/codecs_v2/omx/omx_common/include \
 	$(PV_TOP)/extern_libs_v2/khronos/openmax/include \
	$(PV_TOP)/codecs_v2/omx/omx_sharedlibrary/omxil/src \
	$(PV_TOP)/codecs_v2/omx/omx_sharedlibrary/omxil/src/base \
 	$(PV_INCLUDES)

include $(BUILD_STATIC_LIBRARY)

include $(PV_TOP)/codecs_v2/omx/omx_sharedlibrary/components/ffmpeg-dist/Android.mk
include $(PV_TOP)/codecs_v2/omx/omx_sharedlibrary/lib/ffmpeg-mt/Android.mk
