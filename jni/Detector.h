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

class SimpleDetector {
private:
	int width, height;
	Mat previousMat,currentMat;
	static const float SIZE_FACTOR=.3;
public:
	SimpleDetector();
	~SimpleDetector();

	void setCurrentFrame(unsigned char * frame);
	void setSize(int width, int height);
	bool detectMove();
};

#endif /* DETECTOR_H_ */
