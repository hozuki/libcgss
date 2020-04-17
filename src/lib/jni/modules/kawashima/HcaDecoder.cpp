#include <jni.h>
#include <cgss_api.h>

#include "../../jni_helper.h"
#include "../cgss_jni_class_names.h"
#include "HcaDecoderConfig.h"

using namespace cgss;

#ifdef __cplusplus
extern "C" {
#endif

JNIEXPORT jint JNICALL Java_moe_mottomo_cgss_kawashima_HcaDecoder_read
    (JNIEnv *env, jobject thiz, jbyteArray buffer, jint offset, jint count) {
    if (buffer == nullptr) {
        env->ThrowNew(env->FindClass(jni::java_exceptions::IOException), "JNI: HcaDecoder::read: buffer pointer is null");
        return 0;
    }

    auto decoder = jni::getNativePtrAs<CHcaDecoder>(env, thiz, jni::cgss_class_names::HcaDecoder_Name);

    if (decoder == nullptr) {
        env->ThrowNew(env->FindClass(jni::java_exceptions::IOException), "JNI: HcaDecoder::read: pointer is null");
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
        read = decoder->Read(unsignedBuffer, maxRead, 0, maxRead);

        env->SetByteArrayRegion(buffer, offset, read, signedBuffer);
    } catch (CException &e) {
        jni::throwCgssException(env, e);
    }

    delete[] signedBuffer;

    return read;
}

JNIEXPORT jlong JNICALL Java_moe_mottomo_cgss_kawashima_HcaDecoder_getPosition
    (JNIEnv *env, jobject thiz) {
    auto decoder = jni::getNativePtrAs<CHcaDecoder>(env, thiz, jni::cgss_class_names::HcaDecoder_Name);

    if (decoder == nullptr) {
        env->ThrowNew(env->FindClass(jni::java_exceptions::IOException), "JNI: HcaDecoder::getPosition: pointer is null");
        return 0;
    }

    try {
        return decoder->GetPosition();
    } catch (CException &e) {
        jni::throwCgssException(env, e);
        return 0;
    }
}

JNIEXPORT void JNICALL Java_moe_mottomo_cgss_kawashima_HcaDecoder_setPosition
    (JNIEnv *env, jobject thiz, jlong position) {
    auto decoder = jni::getNativePtrAs<CHcaDecoder>(env, thiz, jni::cgss_class_names::HcaDecoder_Name);

    if (decoder == nullptr) {
        env->ThrowNew(env->FindClass(jni::java_exceptions::IOException), "JNI: HcaDecoder::setPosition: pointer is null");
        return;
    }

    try {
        decoder->SetPosition(position);
    } catch (CException &e) {
        jni::throwCgssException(env, e);
    }
}

JNIEXPORT jlong JNICALL Java_moe_mottomo_cgss_kawashima_HcaDecoder_getLength
    (JNIEnv *env, jobject thiz) {
    auto decoder = jni::getNativePtrAs<CHcaDecoder>(env, thiz, jni::cgss_class_names::HcaDecoder_Name);

    if (decoder == nullptr) {
        env->ThrowNew(env->FindClass(jni::java_exceptions::IOException), "JNI: HcaDecoder::getLength: pointer is null");
        return 0;
    }

    try {
        return decoder->GetLength();
    } catch (CException &e) {
        jni::throwCgssException(env, e);
        return 0;
    }
}

JNIEXPORT void JNICALL Java_moe_mottomo_cgss_kawashima_HcaDecoder_dispose
    (JNIEnv *env, jobject thiz) {
    auto decoder = jni::getNativePtrAs<CHcaDecoder>(env, thiz, jni::cgss_class_names::HcaDecoder_Name);

    if (decoder == nullptr) {
        return;
    }

    try {
        delete decoder;
    } catch (CException &e) {
        jni::throwCgssException(env, e);
    }

    jni::setNativePtr(env, thiz, nullptr, jni::cgss_class_names::HcaDecoder_Name);
}

JNIEXPORT void JNICALL Java_moe_mottomo_cgss_kawashima_HcaDecoder_init
    (JNIEnv *env, jobject thiz, jobject stream, jobject config) {
    if (stream == nullptr) {
        env->ThrowNew(env->FindClass(jni::java_exceptions::IllegalArgumentException), "JNI: HcaDecoder::<init>: stream is null");
        return;
    }

    auto nativeStream = jni::getNativePtrAs<IStream>(env, stream, jni::cgss_class_names::NativeStream_Name);

    if (nativeStream == nullptr) {
        env->ThrowNew(env->FindClass(jni::java_exceptions::IllegalArgumentException), "JNI: HcaDecoder::<init>: stream pointer is null");
        return;
    }

    if (config == nullptr) {
        env->ThrowNew(env->FindClass(jni::java_exceptions::IllegalArgumentException), "JNI: HcaDecoder::<init>: config is null");
        return;
    }

    HCA_DECODER_CONFIG cfg = {0};

    ApplyDecoderConfigObject(env, config, cfg);

    CHcaDecoder *decoder = nullptr;

    try {
        decoder = new CHcaDecoder(nativeStream, cfg);
    } catch (CException &e) {
        jni::throwCgssException(env, e);
    }

    jni::setNativePtr(env, thiz, decoder, jni::cgss_class_names::HcaDecoder_Name);
}

#ifdef __cplusplus
}
#endif