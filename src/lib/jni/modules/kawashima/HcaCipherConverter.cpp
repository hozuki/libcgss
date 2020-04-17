#include <jni.h>
#include <cgss_api.h>

#include "../../jni_helper.h"
#include "../cgss_jni_class_names.h"

#include "HcaCipherConfig.h"

using namespace cgss;

#ifdef __cplusplus
extern "C" {
#endif

JNIEXPORT jint JNICALL Java_moe_mottomo_cgss_kawashima_HcaCipherConverter_read
    (JNIEnv *env, jobject thiz, jbyteArray buffer, jint offset, jint count) {
    if (buffer == nullptr) {
        env->ThrowNew(env->FindClass(jni::java_exceptions::IOException), "JNI: HcaCipherConverter::read: buffer pointer is null");
        return 0;
    }

    auto converter = jni::getNativePtrAs<CHcaCipherConverter>(env, thiz, jni::cgss_class_names::HcaCipherConverter_Name);

    if (converter == nullptr) {
        env->ThrowNew(env->FindClass(jni::java_exceptions::IOException), "JNI: HcaCipherConverter::read: pointer is null");
        return 0;
    }

    auto bufferSize = env->GetArrayLength(buffer);

    auto maxRead = jni::math::computeAvailableBufferSize(bufferSize, offset, count);

    if (maxRead == 0) {
        return 0;
    }

    auto signedBuffer = new jbyte[maxRead];

    memset(signedBuffer, 0, maxRead);

    const auto unsignedBuffer = reinterpret_cast<uint8_t *>(signedBuffer);

    int32_t read = 0;

    try {
        read = converter->Read(unsignedBuffer, maxRead, 0, maxRead);

        env->SetByteArrayRegion(buffer, offset, read, signedBuffer);
    } catch (CException &e) {
        jni::throwCgssException(env, e);
    }

    delete[] signedBuffer;

    return read;
}

JNIEXPORT jlong JNICALL Java_moe_mottomo_cgss_kawashima_HcaCipherConverter_getPosition
    (JNIEnv *env, jobject thiz) {
    auto converter = jni::getNativePtrAs<CHcaCipherConverter>(env, thiz, jni::cgss_class_names::HcaCipherConverter_Name);

    if (converter == nullptr) {
        env->ThrowNew(env->FindClass(jni::java_exceptions::IOException), "JNI: HcaCipherConverter::getPosition: pointer is null");
        return 0;
    }

    try {
        return converter->GetPosition();
    } catch (CException &e) {
        jni::throwCgssException(env, e);
        return 0;
    }
}

JNIEXPORT void JNICALL Java_moe_mottomo_cgss_kawashima_HcaCipherConverter_setPosition
    (JNIEnv *env, jobject thiz, jlong position) {
    auto converter = jni::getNativePtrAs<CHcaCipherConverter>(env, thiz, jni::cgss_class_names::HcaCipherConverter_Name);

    if (converter == nullptr) {
        env->ThrowNew(env->FindClass(jni::java_exceptions::IOException), "JNI: HcaCipherConverter::setPosition: pointer is null");
        return;
    }

    try {
        converter->SetPosition(position);
    } catch (CException &e) {
        jni::throwCgssException(env, e);
    }
}

JNIEXPORT jlong JNICALL Java_moe_mottomo_cgss_kawashima_HcaCipherConverter_getLength
    (JNIEnv *env, jobject thiz) {
    auto converter = jni::getNativePtrAs<CHcaCipherConverter>(env, thiz, jni::cgss_class_names::HcaCipherConverter_Name);

    if (converter == nullptr) {
        env->ThrowNew(env->FindClass(jni::java_exceptions::IOException), "JNI: HcaCipherConverter::getLength: pointer is null");
        return 0;
    }

    try {
        return converter->GetLength();
    } catch (CException &e) {
        jni::throwCgssException(env, e);
        return 0;
    }
}

JNIEXPORT void JNICALL Java_moe_mottomo_cgss_kawashima_HcaCipherConverter_dispose
    (JNIEnv *env, jobject thiz) {
    auto converter = jni::getNativePtrAs<CHcaCipherConverter>(env, thiz, jni::cgss_class_names::HcaCipherConverter_Name);

    if (converter == nullptr) {
        return;
    }

    try {
        delete converter;
    } catch (CException &e) {
        jni::throwCgssException(env, e);
    }

    jni::setNativePtr(env, thiz, nullptr, jni::cgss_class_names::HcaCipherConverter_Name);
}

JNIEXPORT void JNICALL Java_moe_mottomo_cgss_kawashima_HcaCipherConverter_init
    (JNIEnv *env, jobject thiz, jobject stream, jobject from, jobject to) {
    if (stream == nullptr) {
        env->ThrowNew(env->FindClass(jni::java_exceptions::IllegalArgumentException), "JNI: HcaCipherConverter::<init>: stream is null");
        return;
    }

    auto nativeStream = jni::getNativePtrAs<IStream>(env, stream, jni::cgss_class_names::NativeStream_Name);

    if (nativeStream == nullptr) {
        env->ThrowNew(env->FindClass(jni::java_exceptions::IllegalArgumentException), "JNI: HcaCipherConverter::<init>: stream pointer is null");
        return;
    }

    if (from == nullptr) {
        env->ThrowNew(env->FindClass(jni::java_exceptions::IllegalArgumentException), "JNI: HcaCipherConverter::<init>: from cipher is null");
        return;
    }

    if (to == nullptr) {
        env->ThrowNew(env->FindClass(jni::java_exceptions::IllegalArgumentException), "JNI: HcaCipherConverter::<init>: to cipher is null");
        return;
    }

    HCA_CIPHER_CONFIG fromCipher = {0};
    HCA_CIPHER_CONFIG toCipher = {0};

    ApplyCipherConfigObject(env, from, fromCipher);
    ApplyCipherConfigObject(env, to, toCipher);

    CHcaCipherConverter *converter = nullptr;

    try {
        converter = new CHcaCipherConverter(nativeStream, fromCipher, toCipher);
    } catch (CException &e) {
        jni::throwCgssException(env, e);
    }

    jni::setNativePtr(env, thiz, converter, jni::cgss_class_names::HcaCipherConverter_Name);
}

#ifdef __cplusplus
}
#endif
