#include <jni.h>
#include <cgss_api.h>

#include "../../jni_helper.h"
#include "../cgss_jni_class_names.h"

using namespace cgss;

#ifdef __cplusplus
extern "C" {
#endif

JNIEXPORT jint JNICALL Java_moe_mottomo_cgss_takamori_NativeStream_readByte
    (JNIEnv *env, jobject thiz) {
    auto stream = jni::getNativePtrAs<CStream>(env, thiz, jni::cgss_class_names::NativeStream_Name);

    if (stream == nullptr) {
        env->ThrowNew(env->FindClass(jni::java_exceptions::IOException), "JNI: NativeStream::readByte: pointer is null");
        return 0;
    }

    int32_t val = -1;

    try {
        val = stream->ReadByte();
    } catch (CException &e) {
        jni::throwCgssException(env, e);
    }

    return jni::direct_cast<jint>(val);
}

JNIEXPORT jint JNICALL Java_moe_mottomo_cgss_takamori_NativeStream_writeByte
    (JNIEnv *env, jobject thiz, jbyte byte) {
    auto stream = jni::getNativePtrAs<CStream>(env, thiz, jni::cgss_class_names::NativeStream_Name);

    if (stream == nullptr) {
        env->ThrowNew(env->FindClass(jni::java_exceptions::IOException), "JNI: NativeStream::writeByte: pointer is null");
        return 0;
    }

    const auto b = jni::direct_cast<uint8_t>(byte);

    int32_t written = 0;

    try {
        written = stream->WriteByte(b);
    } catch (CException &e) {
        jni::throwCgssException(env, e);
    }

    return static_cast<jint>(written);
}

JNIEXPORT void JNICALL Java_moe_mottomo_cgss_takamori_NativeStream_copyTo
    (JNIEnv *env, jobject thiz, jobject destStream, jint bufferSize) {
    if (bufferSize <= 0) {
        env->ThrowNew(env->FindClass(jni::java_exceptions::IllegalArgumentException), "JNI: CStream::copyTo: bufferSize");
        return;
    }

    auto src = jni::getNativePtrAs<CStream>(env, thiz, jni::cgss_class_names::NativeStream_Name);

    if (src == nullptr) {
        env->ThrowNew(env->FindClass(jni::java_exceptions::IOException), "JNI: CStream::copyTo: src pointer is null");
        return;
    }

    auto *dest = jni::getNativePtrAs<CStream>(env, destStream, jni::cgss_class_names::NativeStream_Name);

    if (dest == nullptr) {
        env->ThrowNew(env->FindClass(jni::java_exceptions::IOException), "JNI: CStream::copyTo: dest pointer is null");
        return;
    }

    auto s = static_cast<uint32_t>(bufferSize);

    try {
        src->CopyTo(*dest, s);
    } catch (CException &e) {
        jni::throwCgssException(env, e);
    }
}

JNIEXPORT void JNICALL Java_moe_mottomo_cgss_takamori_NativeStream_seek
    (JNIEnv *env, jobject thiz, jlong offset, jint origin) {
    auto stream = jni::getNativePtrAs<CStream>(env, thiz, jni::cgss_class_names::NativeStream_Name);

    if (stream == nullptr) {
        env->ThrowNew(env->FindClass(jni::java_exceptions::IOException), "JNI: NativeStream::seek: pointer is null");
        return;
    }

    try {
        stream->Seek(offset, static_cast<StreamSeekOrigin>(origin));
    } catch (CException &e) {
        jni::throwCgssException(env, e);
    }
}

#ifdef __cplusplus
}
#endif
