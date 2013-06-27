LOCAL_PATH := $(call my-dir)

OMX_INCLUDES := \
	frameworks/native/include/media/openmax \

# build libOMX_Core library which will be placed in the /system/lib folder
include $(CLEAR_VARS)

LOCAL_WHOLE_STATIC_LIBRARIES := libomxil-bellagio_lib

LOCAL_MODULE := libOMX_Core
LOCAL_MODULE_TAGS := optional
LOCAL_PRELINK_MODULE := false

LOCAL_SHARED_LIBRARIES := \
	libutils \
	libcutils \
	liblog \
	libdl

include $(BUILD_SHARED_LIBRARY)

#Building omxregister-bellagio binary which will be placed in the /system/bin folder
include $(CLEAR_VARS)

LOCAL_SRC_FILES := \
	src/common.c \
	src/omxregister.c \
	src/omxregister.h 

LOCAL_MODULE := omxregister-bellagio
LOCAL_MODULE_TAGS := optional

LOCAL_CFLAGS :=  \
	-DOMXILCOMPONENTSPATH=\"/system/lib\" \

LOCAL_C_INCLUDES := \
	$(OMX_INCLUDES)

LOCAL_ARM_MODE := arm

LOCAL_SHARED_LIBRARIES := \
	libutils \
	libcutils \
	liblog \
	libdl

include $(BUILD_EXECUTABLE)

# Building the libomxil-bellagio static library
include $(CLEAR_VARS)

LOCAL_SRC_FILES := \
	src/common.c \
	src/content_pipe_file.c \
	src/content_pipe_inet.c \
	src/omx_create_loaders_linux.c \
	src/omxcore.c \
 	src/omx_reference_resource_manager.c \
	src/omxregister.c \
	src/queue.c \
	src/st_static_component_loader.c \
	src/tsemaphore.c \
	src/utils.c \
 	src/base/OMXComponentRMExt.c \
	src/base/omx_base_audio_port.c \
	src/base/omx_base_clock_port.c \
	src/base/omx_base_component.c \
	src/base/omx_base_filter.c \
	src/base/omx_base_image_port.c \
	src/base/omx_base_port.c \
	src/base/omx_base_sink.c \
	src/base/omx_base_source.c \
	src/base/omx_base_video_port.c \
 	src/core_extensions/OMXCoreRMExt.c

LOCAL_MODULE := libomxil-bellagio_lib
LOCAL_MODULE_TAGS := optional

LOCAL_CFLAGS :=  \
	-DOMX_LOADERS_DIRNAME=\"/system/lib/\" \
	-DOMXILCOMPONENTSPATH=\"/system/lib/\"

# For debugging
#LOCAL_CFLAGS += -DCONFIG_DEBUG_LEVEL=255

LOCAL_ARM_MODE := arm

LOCAL_SHARED_LIBRARIES := libutils

LOCAL_C_INCLUDES := \
	$(OMX_INCLUDES) \
	external/omxil/src/ \
	external/omxil/src/base \

LOCAL_COPY_HEADERS_TO := bellagio

LOCAL_COPY_HEADERS := \
src/omx_reference_resource_manager.h \
src/extension_struct.h \
src/base/omx_classmagic.h \
src/base/omx_base_clock_port.h \
src/base/OMXComponentRMExt.h \
src/base/omx_base_port.h \
src/base/omx_base_sink.h \
src/base/omx_base_image_port.h \
src/base/omx_base_filter.h \
src/base/omx_base_component.h \
src/base/omx_base_source.h \
src/base/omx_base_audio_port.h \
src/base/omx_base_video_port.h \
src/content_pipe_file.h \
src/utils.h \
src/omx_comp_debug_levels.h \
src/core_extensions/OMXCoreRMExt.h \
src/tsemaphore.h \
src/component_loader.h \
src/st_static_component_loader.h \
src/omx_create_loaders.h \
src/omxcore.h \
src/common.h \
src/queue.h \
src/content_pipe_inet.h \
test/components/common/user_debug_levels.h

include $(BUILD_STATIC_LIBRARY)
