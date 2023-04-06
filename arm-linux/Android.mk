LOCAL_PATH := $(call my-dir)

include $(CLEAR_VARS)

LOCAL_MODULE := SecDet

LOCAL_SRC_FILES := $(wildcard *.java)

LOCAL_PROGUARD_ENABLED := obfuscation optimization
LOCAL_PROGUARD_FLAG_FILES := proguard-rules.pro

include $(BUILD_PACKAGE)
# Add any additional dependencies or libraries needed for your app here

include $(CLEAR_VARS)

LOCAL_MODULE := SecDet

LOCAL_SRC_FILES := mylib.c

include $(BUILD_SHARED_LIBRARY)
# Add any additional build configurations or settings here
# For example, if you need to build with specific C flags

include $(CLEAR_VARS)

LOCAL_MODULE := SecDet

LOCAL_CFLAGS := -std=c99

include $(BUILD_PACKAGE)
# If you need to specify any additional compiler or linker flags
# you can add them here

include $(CLEAR_VARS)

LOCAL_MODULE := SecDet

LOCAL_CFLAGS := -std=c99

LOCAL_LDFLAGS := -lmylib

include $(BUILD_PACKAGE)