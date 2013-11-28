/*
 * Detector.h
 *
 *  Created on: 2013年11月19日
 *      Author: marshal
 */

#ifndef DETECTOR_H_
#define DETECTOR_H_

#include <opencv2/opencv.hpp>

using namespace cv;

class MoveDetector {
private:
	int width, height;
	Mat previousMat, currentMat;
	static const float SIZE_FACTOR = .3;
	vector<Point2f> trackPrevPoints, trackNewPoints, offsetPoints;
public:
	MoveDetector();
	~MoveDetector();

	void setCurrentFrame(unsigned char * frame);
	void setSize(int width, int height);
	bool detectMove();
	Mat getCurrentMat();
	Mat getPreviousMat();
};

class MatchDetector {
private:
	Mat templateMat;
public:
	MatchDetector();
	~MatchDetector();

	void setMatchData(Mat mat); //先用第一个Mat做测试
	int detectMatch(Mat mat);
};

#endif /* DETECTOR_H_ */
