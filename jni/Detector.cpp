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

MoveDetector *moveDetector;
MatchDetector *matchDetector;
bool running;

JNIEXPORT void JNICALL Java_marshal_magicbookapp_Detector_init(JNIEnv * env,
		jobject thiz) {
	moveDetector = new MoveDetector;
	matchDetector = new MatchDetector;
	running = true;
}

JNIEXPORT void JNICALL Java_marshal_magicbookapp_Detector_clear(JNIEnv * env,
		jobject thiz) {
	if (!running) {
		return;
	}

	delete moveDetector;
	delete matchDetector;
}

JNIEXPORT jboolean JNICALL Java_marshal_magicbookapp_Detector_detectMove(
		JNIEnv *env, jobject thiz, jbyteArray data) {
	if (!running) {
		return (jboolean) true;
	}

	jbyte* yuv = env->GetByteArrayElements(data, 0);
	moveDetector->setCurrentFrame((unsigned char *) yuv);
	env->ReleaseByteArrayElements(data, yuv, 0);

	return (jboolean) moveDetector->detectMove();
}

JNIEXPORT void JNICALL Java_marshal_magicbookapp_Detector_setSize(JNIEnv * env,
		jobject thiz, jint width, jint height) {
	moveDetector->setSize((int) width, (int) height);
}

JNIEXPORT jint JNICALL Java_marshal_magicbookapp_Detector_detectMatch(
		JNIEnv *env, jobject thiz, jbyteArray data) {
	if (!running) {
		return (jint) -1;
	}
	LOGI(">>>>运行匹配图形的本地方法");
	matchDetector->setMatchData(moveDetector->getPreviousMat());
	return (jint) matchDetector->detectMatch(moveDetector->getCurrentMat());
}

//Takes a descriptor and turns it into an xy point
void keypoints2points(const vector<KeyPoint>& in, vector<Point2f>& out) {
	out.clear();
	out.reserve(in.size());
	for (size_t i = 0; i < in.size(); ++i) {
		out.push_back(in[i].pt);
	}
}

///////////////////////////
// MoveDetector
///////////////////////////

MoveDetector::MoveDetector() {
	LOGI("------->>>创建detector对象");
	width = height = 0;
}

MoveDetector::~MoveDetector() {
	LOGI("-------->>>>释放detector对象");
}

bool MoveDetector::detectMove() {
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

			now = clock();
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
	LOGI("检测是否移动：%s", isMoved ? "移动了 " : "静止的 ");
	return isMoved;
}

void MoveDetector::setCurrentFrame(unsigned char * frame) {
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

void MoveDetector::setSize(int width, int height) {
	this->width = width;
	this->height = height;
}

Mat MoveDetector::getCurrentMat() {
	return currentMat;
}
Mat MoveDetector::getPreviousMat() {
	return previousMat;
}

///////////////////////////
// MatchDetector
///////////////////////////

MatchDetector::MatchDetector() {

}

MatchDetector::~MatchDetector() {

}

int MatchDetector::detectMatch(Mat mat) {
	LOGI("ORB匹配..");
	clock_t now = clock();

	Ptr<flann::IndexParams> indexParams = new flann::KDTreeIndexParams();
	Ptr<flann::SearchParams> searchParams = new flann::SearchParams();

	indexParams->setAlgorithm(cvflann::FLANN_INDEX_LSH);
	indexParams->setInt("table_number", 6);
	indexParams->setInt("key_size", 12);
	indexParams->setInt("multi_probe_level", 1);
	searchParams->setAlgorithm(cvflann::FLANN_INDEX_LSH);

	FlannBasedMatcher matcher(indexParams, searchParams);

	OrbFeatureDetector detector(1000);
	OrbDescriptorExtractor extractor(1000);

	vector<KeyPoint> keyPoints0;
	Mat descriptor0;
	detector.detect(templateMat, keyPoints0, descriptor0);
	extractor.compute(templateMat, keyPoints0, descriptor0);

	LOGI("ORB计算耗时(模版计算时间)：%ld", (clock() - now) / 1000);

	vector<vector<DMatch> > matches;

	vector<Mat> descriptors;
	descriptors.push_back(descriptor0);
	matcher.add(descriptors);

	vector<KeyPoint> keyPoints;
	Mat descriptor;
	detector.detect(mat, keyPoints, descriptor);
	extractor.compute(mat, keyPoints, descriptor);

	matcher.knnMatch(descriptor, matches, 2);

	LOGI("ORB计算耗时：%ld", (clock() - now) / 1000);
	return 1;
}

void MatchDetector::setMatchData(Mat mat) {
	templateMat = mat;
}

