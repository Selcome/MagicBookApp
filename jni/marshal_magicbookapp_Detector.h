/* DO NOT EDIT THIS FILE - it is machine generated */
#include <jni.h>
/* Header for class marshal_magicbookapp_Detector */

#ifndef _Included_marshal_magicbookapp_Detector
#define _Included_marshal_magicbookapp_Detector
#ifdef __cplusplus
extern "C" {
#endif
/*
 * Class:     marshal_magicbookapp_Detector
 * Method:    init
 * Signature: ()V
 */
JNIEXPORT void JNICALL Java_marshal_magicbookapp_Detector_init(JNIEnv *,
		jobject);

/*
 * Class:     marshal_magicbookapp_Detector
 * Method:    clear
 * Signature: ()V
 */
JNIEXPORT void JNICALL Java_marshal_magicbookapp_Detector_clear(JNIEnv *,
		jobject);

/*
 * Class:     marshal_magicbookapp_Detector
 * Method:    detectMove
 * Signature: ([B)Z
 */
JNIEXPORT jboolean JNICALL Java_marshal_magicbookapp_Detector_detectMove(
		JNIEnv *, jobject, jbyteArray);

#ifdef __cplusplus

class SimpleDetector {
public:
	SimpleDetector();
	void printOk();
	~SimpleDetector();
};

}
#endif
#endif
