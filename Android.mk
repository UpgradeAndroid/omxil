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

# Building the libomxil-bellagio 
include $(CLEAR_VARS)

LOCAL_SRC_FILES := \
 	src/pv_omx_interface.cpp \
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

LOCAL_CFLAGS :=  -DOMXILCOMPONENTSPATH=\"lib\" $(PV_CFLAGS) -DANDROID_COMPILATION

LOCAL_ARM_MODE := arm

LOCAL_STATIC_LIBRARIES := 

LOCAL_SHARED_LIBRARIES := libutils

LOCAL_C_INCLUDES := \
	$(PV_TOP)/codecs_v2/omx/omx_mastercore/src \
 	$(PV_TOP)/codecs_v2/omx/omx_mastercore/include \
 	$(PV_TOP)/codecs_v2/omx/omx_common/include \
 	$(PV_TOP)/extern_libs_v2/khronos/openmax/include \
	$(PV_TOP)/codecs_v2/omx/omx_sharedlibrary/omxil/src \
	$(PV_TOP)/codecs_v2/omx/omx_sharedlibrary/omxil/src/base \
	$(PV_TOP)/codecs_v2/omx/omx_sharedlibrary/omxil/src/core_extensions \
	$(PV_INCLUDES)

LOCAL_COPY_HEADERS_TO := $(PV_COPY_HEADERS_TO)/bellagio

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

