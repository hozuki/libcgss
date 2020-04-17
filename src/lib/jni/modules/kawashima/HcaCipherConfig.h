#pragma once

#include <jni.h>
#include <cgss_cdata.h>

void ApplyCipherConfigObject(JNIEnv *env, jobject config, HCA_CIPHER_CONFIG &cfg);
