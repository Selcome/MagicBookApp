/*
 * Detector.cpp
 *
 *  Created on: 2013年11月18日
 *      Author: marshal
 */

#include <jni.h>
#include <iostream>

//#include <opencv2/opencv.hpp>
//#include <android/log.h>

#include "marshal_magicbookapp_Detector.h"


using namespace std;

JNIEXPORT jboolean JNICALL Java_marshal_magicbookapp_Detector_detectMove
  (JNIEnv *env, jobject thiz, jbyteArray frame){
	return (jboolean)true;
}
