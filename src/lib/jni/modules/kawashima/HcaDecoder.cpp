#include <jni.h>
#include <cgss_api.h>

#include "../../jni_helper.hpp"

using namespace cgss;

#ifdef __cplusplus
extern "C" {
#endif

static void ApplyDecoderConfigObject(JNIEnv *env, jobject config, HCA_DECODER_CONFIG &cfg);

JNIEXPORT jint JNICALL Java_moe_mottomo_cgss_kawashima_HcaDecoder_read
    (JNIEnv *env, jobject thiz, jbyteArray buffer, jint offset, jint count) {
    if (buffer == nullptr) {
        env->ThrowNew(env->FindClass(jni::java_exceptions::IOException), "JNI: HcaDecoder::read: buffer pointer is null");
        return 0;
    }

    auto decoder = jni::getNativePtrAs<CHcaDecoder>(env, thiz);

    if (decoder == nullptr) {
        env->ThrowNew(env->FindClass(jni::java_exceptions::IOException), "JNI: HcaDecoder::read: pointer is null");
        return 0;
    }

    auto bufferSize = env->GetArrayLength(buffer);

    auto maxRead = jni::computeAvailableBufferSize(bufferSize, offset, count);

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
    auto decoder = jni::getNativePtrAs<CHcaDecoder>(env, thiz);

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
    auto decoder = jni::getNativePtrAs<CHcaDecoder>(env, thiz);

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
    auto decoder = jni::getNativePtrAs<CHcaDecoder>(env, thiz);

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
    auto decoder = jni::getNativePtrAs<CHcaDecoder>(env, thiz);

    if (decoder == nullptr) {
        return;
    }

    try {
        delete decoder;
    } catch (CException &e) {
        jni::throwCgssException(env, e);
    }

    jni::setNativePtr(env, thiz, nullptr);
}

JNIEXPORT void JNICALL Java_moe_mottomo_cgss_kawashima_HcaDecoder_init
    (JNIEnv *env, jobject thiz, jobject stream, jobject config) {
    if (stream == nullptr) {
        env->ThrowNew(env->FindClass(jni::java_exceptions::IllegalArgumentException), "JNI: HcaDecoder::<init>: stream is null");
        return;
    }

    auto nativeStream = jni::getNativePtrAs<IStream>(env, stream);

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

    jni::setNativePtr(env, thiz, decoder);
}

static void ApplyCipherConfigObject(JNIEnv *env, jobject config, HCA_CIPHER_CONFIG &cfg) {
    auto cipherType = jni::getObjectField(env, config, "cipherType", "moe/mottomo/cgss/kawashima/HcaCipherType");

    {
        auto cipherTypeClass = env->FindClass("moe/mottomo/cgss/kawashima/HcaCipherType");
        auto valueMethodId = env->GetMethodID(cipherTypeClass, "value", "()I");
        auto cipherTypeValue = env->CallIntMethod(cipherType, valueMethodId);

        cfg.cipherType = static_cast<CGSS_HCA_CIPHER_TYPE>(cipherTypeValue);
    }

    auto rawKey = jni::getLongField(env, config, "rawKey");
    cfg.key = jni::direct_cast<uint64_t>(rawKey);

    auto keyModifier = jni::getShortField(env, config, "keyModifier");
    cfg.keyModifier = jni::direct_cast<uint16_t>(keyModifier);
}

static void ApplyDecoderConfigObject(JNIEnv *env, jobject config, HCA_DECODER_CONFIG &cfg) {
    auto cipherConfig = jni::getObjectField(env, config, "cipherConfig", "moe/mottomo/cgss/kawashima/HcaCipherConfig");
    ApplyCipherConfigObject(env, cipherConfig, cfg.cipherConfig);

    auto waveHeaderEnabled = jni::getBooleanField(env, config, "waveHeaderEnabled");
    cfg.waveHeaderEnabled = waveHeaderEnabled;

    auto loopEnabled = jni::getBooleanField(env, config, "loopEnabled");
    cfg.loopEnabled = loopEnabled;

    auto loopCount = jni::getIntField(env, config, "loopCount");
    cfg.loopCount = loopCount;

    auto decodeFunc = jni::getObjectField(env, config, "decodeFunc", "moe/mottomo/cgss/kawashima/HcaDecodeFunction");

    HcaDecodeFunc df = nullptr;

    if (decodeFunc != nullptr) {
        auto decodeFuncClass = env->FindClass("moe/mottomo/cgss/kawashima/HcaDecodeFunction");
        auto valueMethodId = env->GetMethodID(decodeFuncClass, "value", "()I");

        auto decodeFuncValue = env->CallIntMethod(decodeFunc, valueMethodId);

        switch (decodeFuncValue) {
            case 1:
                df = CDefaultWaveGenerator::Decode8BitU;
                break;
            case 2:
                df = CDefaultWaveGenerator::Decode16BitS;
                break;
            case 3:
                df = CDefaultWaveGenerator::Decode24BitS;
                break;
            case 4:
                df = CDefaultWaveGenerator::Decode32BitS;
                break;
            case 5:
                df = CDefaultWaveGenerator::DecodeFloat;
                break;
            default:
                break;
        }
    }

    cfg.decodeFunc = df;
}

#ifdef __cplusplus
}
#endif