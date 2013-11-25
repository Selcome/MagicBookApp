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

//Takes a descriptor and turns it into an xy point
void keypoints2points(const vector<KeyPoint>& in, vector<Point2f>& out) {
	out.clear();
	out.reserve(in.size());
	for (size_t i = 0; i < in.size(); ++i) {
		out.push_back(in[i].pt);
	}
}

SimpleDetector::SimpleDetector() {
	LOGI("------->>>创建detector对象");
	width = height = 0;
}

SimpleDetector::~SimpleDetector() {
	LOGI("-------->>>>释放detector对象");
}

bool SimpleDetector::detectMove() {
	bool isMoved = true;
	if (!previousMat.empty()) {
		FastFeatureDetector fast(40);
		vector<KeyPoint> v;

		clock_t now = clock();
		fast.detect(previousMat, v);

		LOGI("FAST获取关键点耗时：%ld", (clock() - now) / 1000);

		if (v.size() > 0) {
			keypoints2points(v, trackPrevPoints);

			vector<uchar> status;
			vector<float> err;
			Size winSize(15, 15);
			TermCriteria termcrit(CV_TERMCRIT_ITER | CV_TERMCRIT_EPS, 10, 0.1);
			Point2f newCenter(0, 0), prevCenter(0, 0), currShift;

			now = clock();
			calcOpticalFlowPyrLK(previousMat, currentMat, trackPrevPoints,
					trackNewPoints, status, err, winSize, 3, termcrit, 0);
			LOGI("光流计算耗时：%ld", (clock() - now) / 1000);

			now=clock();
			size_t i, k;
			for (i = k = 0; i < trackNewPoints.size(); i++) {
				if (!status[i])
					continue;

				prevCenter += trackPrevPoints[i];
				newCenter += trackNewPoints[i];
				trackNewPoints[k] = trackNewPoints[i];
				k++;
			}
			trackNewPoints.resize(k);

			currShift = newCenter * (1.0 / (float) k)
					- prevCenter * (1.0 / (float) k);
			isMoved = ((currShift.x + currShift.y)) > 1.5;

			LOGI("偏移量计算耗时：%ld", (clock() - now) / 1000);
		}
	}
	currentMat.copyTo(previousMat);
	LOGI("检测是否移动：%s", isMoved?"移动了 ":"静止的 ");
	return isMoved;
}

void SimpleDetector::setCurrentFrame(unsigned char * frame) {
	if (height == 0) {
		return;
	}
	Mat mat(height, width, CV_8UC1, frame);

	Size dsize = Size((int) (mat.cols * SIZE_FACTOR),
			(int) (mat.rows * SIZE_FACTOR));
	currentMat = Mat(dsize, CV_8UC1);
//	LOGI(">>>>>>>>set current frame, w:%i, h:%i", dsize.width, dsize.height);
	resize(mat, currentMat, dsize);
}

void SimpleDetector::setSize(int width, int height) {
	this->width = width;
	this->height = height;
}
