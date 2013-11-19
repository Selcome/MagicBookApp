LOCAL_PATH := $(call my-dir)
include $(CLEAR_VARS)
LOCAL_LDLIBS :=-llog
OPENCV_LIB_TYPE:=STATIC 
include /opt/OpenCV-2.4.6-android-sdk/sdk/native/jni/OpenCV.mk
LOCAL_MODULE    := Detector 
LOCAL_SRC_FILES := Detector.cpp
APP_STL:=stlport_static
include $(BUILD_SHARED_LIBRARY)

 APP_OPTIM := debug
