#pragma once

#include <string>
#include <jni.h>

#include "cgss_jni_ns.h"

CGSS_JNI_NS_BEGIN

        std::string utf8ToAnsi(JNIEnv *env, jstring str);

        jstring ansiToUtf8(JNIEnv *env, const std::string &str);

CGSS_JNI_NS_END
