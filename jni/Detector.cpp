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
	env->ReleaseByteArrayElements(data, yuv, 0);
	return (jboolean) detector->detectMove();
}

JNIEXPORT void JNICALL Java_marshal_magicbookapp_Detector_setSize(JNIEnv * env,
		jobject thiz, jint width, jint height) {
	detector->setSize((int) width, (int) height);
}

SimpleDetector::SimpleDetector() {
	LOGI("------->>>创建detector对象");
	width = height = 0;
}

SimpleDetector::~SimpleDetector() {
	LOGI("-------->>>>释放detector对象");
}

bool SimpleDetector::detectMove() {
//	previousMat = currentMat;
	return true;
}

void SimpleDetector::setCurrentFrame(unsigned char * frame) {
	if (height == 0) {
		return;
	}
	Mat mat(height, width, CV_8UC1, frame);

	Size dsize = Size((int)(mat.cols * SIZE_FACTOR), (int)(mat.rows * SIZE_FACTOR));
	currentMat = Mat(dsize, CV_8UC1);
//	LOGI(">>>>>>>>set current frame, w:%i, h:%i", dsize.width, dsize.height);
	resize(mat, currentMat, dsize);
}

void SimpleDetector::setSize(int width, int height) {
	this->width = width;
	this->height = height;
}
