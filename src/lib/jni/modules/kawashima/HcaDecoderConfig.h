#pragma once

#include <jni.h>
#include <cgss_cdata.h>

void ApplyDecoderConfigObject(JNIEnv *env, jobject config, HCA_DECODER_CONFIG &cfg);
