#include <jni.h>
#include <cgss_api.h>

#include "../../jni_helper.hpp"

using namespace cgss;

#ifdef __cplusplus
extern "C" {
#endif

JNIEXPORT jboolean JNICALL Java_moe_mottomo_cgss_kawashima_HcaFormatReader_isWritable
    (JNIEnv *env, jobject thiz) {
    auto reader = jni::getNativePtrAs<CHcaFormatReader>(env, thiz);

    if (reader == nullptr) {
        env->ThrowNew(env->FindClass(jni::java_exceptions::IOException), "JNI: HcaFormatReader::isWritable: pointer is null");
        return JNI_FALSE;
    }

    return reader->IsWritable();
}

JNIEXPORT jboolean JNICALL Java_moe_mottomo_cgss_kawashima_HcaFormatReader_isReadable
    (JNIEnv *env, jobject thiz) {
    auto reader = jni::getNativePtrAs<CHcaFormatReader>(env, thiz);

    if (reader == nullptr) {
        env->ThrowNew(env->FindClass(jni::java_exceptions::IOException), "JNI: HcaFormatReader::isReadable: pointer is null");
        return JNI_FALSE;
    }

    return reader->IsReadable();
}

JNIEXPORT jboolean JNICALL Java_moe_mottomo_cgss_kawashima_HcaFormatReader_isSeekable
    (JNIEnv *env, jobject thiz) {
    auto reader = jni::getNativePtrAs<CHcaFormatReader>(env, thiz);

    if (reader == nullptr) {
        env->ThrowNew(env->FindClass(jni::java_exceptions::IOException), "JNI: HcaFormatReader::isSeekable: pointer is null");
        return JNI_FALSE;
    }

    return reader->IsSeekable();
}

JNIEXPORT jobject JNICALL Java_moe_mottomo_cgss_kawashima_HcaFormatReader_getHcaInfo
    (JNIEnv *env, jobject thiz) {
    auto reader = jni::getNativePtrAs<CHcaFormatReader>(env, thiz);

    if (reader == nullptr) {
        env->ThrowNew(env->FindClass(jni::java_exceptions::IOException), "JNI: HcaFormatReader::getHcaInfo: pointer is null");
        return JNI_FALSE;
    }

    HCA_INFO info = {0};
    auto errored = false;

    try {
        reader->GetHcaInfo(&info);
    } catch (CException &e) {
        errored = true;
        jni::throwCgssException(env, e);
    }

    if (errored) {
        return nullptr;
    }

    jobject hca = nullptr;

    do {
        jclass hcaInfoClass = env->FindClass("moe/mottomo/cgss/kawashima/HcaInfo");

        if (hcaInfoClass == nullptr) {
            break;
        }

        jmethodID hcaInfoCtor = env->GetMethodID(hcaInfoClass, "<init>", "()V");

        if (hcaInfoCtor == nullptr) {
            break;
        }

        hca = env->NewObject(hcaInfoClass, hcaInfoCtor);
    } while (false);

    if (hca == nullptr) {
        return nullptr;
    }

#define set_field(type, name) jni::setField(env, hca, cgss_xstr(name), jni::direct_cast<j##type>(info. name))

    set_field(short, versionMajor);
    set_field(short, versionMinor);
    set_field(int, channelCount);
    set_field(int, samplingRate);
    set_field(int, blockCount);
    set_field(short, blockSize);
    set_field(short, athType);
    set_field(int, loopStart);
    set_field(int, loopEnd);
    set_field(float, rvaVolume);
    set_field(byte, commentLength);
    set_field(short, fmtR01);
    set_field(short, fmtR02);
    set_field(short, compR01);
    set_field(short, compR02);
    set_field(short, compR03);
    set_field(short, compR04);
    set_field(short, compR05);
    set_field(short, compR06);
    set_field(short, compR07);
    set_field(short, compR08);
    set_field(int, compR09);
    set_field(short, vbrR01);
    set_field(short, vbrR02);
    set_field(short, loopR01);
    set_field(short, loopR02);

#undef set_field

    jni::setField(env, hca, "loopExists", static_cast<jboolean>(info.loopExists));

    auto jstrComment = jni::ansiToUtf8(env, info.comment);
    jni::setField(env, hca, "comment", "java.lang.String", jstrComment);

    {
        auto cipherTypeClass = env->FindClass("moe/mottomo/cgss/kawashima/HcaCipherType");
        auto method = env->GetStaticMethodID(cipherTypeClass, "valueOf", "(I)Lmoe/mottomo/cgss/kawashima/HcaCipherType;");
        auto obj = env->CallStaticObjectMethod(cipherTypeClass, method, static_cast<jint>(info.cipherType));

        jni::setField(env, hca, "cipherType", "moe/mottomo/cgss/kawashima/HcaCipherType", obj);
    }

    return hca;
}

JNIEXPORT jboolean JNICALL Java_moe_mottomo_cgss_kawashima_HcaFormatReader_isPossibleHcaStream
    (JNIEnv *env, jclass, jobject stream) {
    if (stream == nullptr) {
        env->ThrowNew(env->FindClass(jni::java_exceptions::IllegalArgumentException), "JNI: HcaFormatReader::isPossibleHcaStream: stream is null");
        return JNI_FALSE;
    }

    auto nativeStream = jni::getNativePtrAs<IStream>(env, stream);

    if (nativeStream == nullptr) {
        env->ThrowNew(env->FindClass(jni::java_exceptions::IllegalArgumentException), "JNI: HcaFormatReader::isPossibleHcaStream: stream pointer is null");
        return JNI_FALSE;
    }

    try {
        return CHcaFormatReader::IsPossibleHcaStream(nativeStream);
    } catch (CException &e) {
        jni::throwCgssException(env, e);
        return JNI_FALSE;
    }
}

#ifdef __cplusplus
}
#endif