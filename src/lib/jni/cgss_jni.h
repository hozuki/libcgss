#pragma once

#ifdef __CGSS_WITH_JNI__

#include <jni.h>
#include "../cgss_env.h"

#ifdef __cplusplus
extern "C" {
#endif

#define DECL_CGSS_JNI_FUNC(ret_type, firstPackage, className, funcName) JNIEXPORT ret_type JNICALL Java_moe_mottomo_cgss_##firstPackage##_##className##_##funcName
#define IMPL_CGSS_JNI_FUNC(ret_type, firstPackage, className, funcName) ret_type Java_moe_mottomo_cgss_##firstPackage##_##className##_##funcName

DECL_CGSS_JNI_FUNC(jint, wave, DefaultWaveGenerator, decode8BitU)(JNIEnv *env, jclass klass, jfloat data, jbyteArray buffer, jint cursor);
DECL_CGSS_JNI_FUNC(jint, wave, DefaultWaveGenerator, decode16BitS)(JNIEnv *env, jclass klass, jfloat data, jbyteArray buffer, jint cursor);
DECL_CGSS_JNI_FUNC(jint, wave, DefaultWaveGenerator, decode24BitS)(JNIEnv *env, jclass klass, jfloat data, jbyteArray buffer, jint cursor);
DECL_CGSS_JNI_FUNC(jint, wave, DefaultWaveGenerator, decode32BitS)(JNIEnv *env, jclass klass, jfloat data, jbyteArray buffer, jint cursor);
DECL_CGSS_JNI_FUNC(jint, wave, DefaultWaveGenerator, decodeFloat)(JNIEnv *env, jclass klass, jfloat data, jbyteArray buffer, jint cursor);

DECL_CGSS_JNI_FUNC(void, streams, Stream, seek)(JNIEnv *env, jobject thiz, jlong offset, jint origin);
DECL_CGSS_JNI_FUNC(jbyte, streams, Stream, readByte)(JNIEnv *env, jobject thiz);
DECL_CGSS_JNI_FUNC(jint, streams, Stream, writeByte)(JNIEnv *env, jobject thiz, jbyte byte);
DECL_CGSS_JNI_FUNC(void, streams, Stream, copyTo)(JNIEnv *env, jobject thiz, jobject destStream, jint bufferSize);

DECL_CGSS_JNI_FUNC(void, streams, MemoryStream, initNewBuffer)(JNIEnv *env, jobject thiz, jlong capacity, jboolean resizable);

DECL_CGSS_JNI_FUNC(jboolean, hca, HcaFormatReader, isWritable)(JNIEnv *env, jobject thiz);
DECL_CGSS_JNI_FUNC(jboolean, hca, HcaFormatReader, isReadable)(JNIEnv *env, jobject thiz);
DECL_CGSS_JNI_FUNC(jboolean, hca, HcaFormatReader, isSeekable)(JNIEnv *env, jobject thiz);
DECL_CGSS_JNI_FUNC(jobject, hca, HcaFormatReader, getHcaInfo)(JNIEnv *env, jobject thiz);

#ifdef __cplusplus
};
#endif

#endif
