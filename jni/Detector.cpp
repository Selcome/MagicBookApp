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

#include <cassert>

#include "marshal_magicbookapp_Detector.h"
#include "Detector.h"

#define LOGI(...) ((void)__android_log_print(ANDROID_LOG_INFO, "detector", __VA_ARGS__))

using namespace std;
using namespace cv;

SimpleDetector *detector;

JNIEXPORT void JNICALL Java_marshal_magicbookapp_Detector_init(JNIEnv * env,
		jobject thiz) {
	detector = new SimpleDetector;
}

JNIEXPORT void JNICALL Java_marshal_magicbookapp_Detector_clear(JNIEnv * env,
		jobject thiz) {
	delete detector;
}

JNIEXPORT jboolean JNICALL Java_marshal_magicbookapp_Detector_detectMove(
		JNIEnv *env, jobject thiz, jbyteArray data) {
	jbyte* yuv = env->GetByteArrayElements(data, 0);
	detector->setCurrentFrame((unsigned char *) yuv);
	return (jboolean) detector->detectMove();
}

JNIEXPORT void JNICALL Java_marshal_magicbookapp_Detector_setSize(JNIEnv * env,
		jobject thiz, jint width, jint height) {
	detector->setSize((int) width, (int) height);
}

SimpleDetector::SimpleDetector() {
	LOGI("------->>>创建detector对象");
	this->height=this->width=0;
	this->currentMat=this->previousMat=0;
}

SimpleDetector::~SimpleDetector() {
	LOGI("-------->>>>释放detector对象");
}

bool SimpleDetector::detectMove() {
	if(this->previousMat){

	}
	this->previousMat=this->currentMat;
	return true;
}

void SimpleDetector::setCurrentFrame(unsigned char * frame) {
	this->currentMat=new Mat(this->height,this->width,CV_8UC1,frame);//我现在不确认旧的mat是不是被正确的释放了，至少系统可维持很长时间，看起来是释放了。
}

void SimpleDetector::setSize(int width, int height) {
	this->width=width;
	this->height=height;
}
