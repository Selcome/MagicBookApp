/*
 * Detector.cpp
 *
 *  Created on: 2013年11月18日
 *      Author: marshal
 */

#include <jni.h>
#include <iostream>

#include <opencv2/opencv.hpp>
#include <android/log.h>

#include "marshal_magicbookapp_Detector.h"

#define LOGI(...) ((void)__android_log_print(ANDROID_LOG_INFO, "detector", __VA_ARGS__))

using namespace std;
using namespace cv;

SimpleDetector *detector;

JNIEXPORT void JNICALL Java_marshal_magicbookapp_Detector_init(JNIEnv * env,
		jobject thiz) {
//	detector=new SimpleDetector;
//	detector->printOk();
}

JNIEXPORT void JNICALL Java_marshal_magicbookapp_Detector_clear(JNIEnv * env,
		jobject thiz) {
//	delete detector;
}

JNIEXPORT jboolean JNICALL Java_marshal_magicbookapp_Detector_detectMove(
		JNIEnv *env, jobject thiz, jbyteArray frame) {
	return (jboolean) true;
}

SimpleDetector::SimpleDetector(){
	LOGI("------->>>创建detector对象");
}

SimpleDetector::~SimpleDetector(){
	LOGI("-------->>>>释放detector对象");
}

void SimpleDetector::printOk() {
	LOGI(">>>>>>>>>>>>测试一下");
}
