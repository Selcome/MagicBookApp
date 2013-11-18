LOCAL_PATH := $(call my-dir)
include $(CLEAR_VARS)
OPENCV_LIB_TYPE:=STATIC 
LOCAL_MODULE    := Detector 
LOCAL_SRC_FILES := Detector.cpp
APP_STL:=stlport_static
include $(BUILD_SHARED_LIBRARY)