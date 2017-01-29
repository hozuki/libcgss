#include <iostream>
#include "cgss_jni.h"
#include "../cgss_api.h"
#include "jni_helper.hpp"

using namespace cgss;

template<class TOut, class TIn>
static TOut direct_cast(const TIn &in) {
    return *(TOut *)&in;
};

template<class TOut, class TIn>
static TOut C(const TIn &in) {
    return static_cast<TOut>(in);
};

template<jsize size>
static inline jint decode_wave(HcaDecodeFunc func, JNIEnv *env, jfloat data, jbyteArray buffer, jint cursor) {
    jbyte body[size] = {0};
    uint8_t *b = reinterpret_cast<uint8_t *>(body);
    env->GetByteArrayRegion(buffer, cursor, size, body);
    uint32_t diff = func(data, b, 0);
    env->SetByteArrayRegion(buffer, cursor, size, body);
    return static_cast<jint>(cursor + diff);
}


JNIEXPORT jint JNICALL JNI_OnLoad(JavaVM *, void *) {
    std::cerr << "libcgss JNI library: loaded." << std::endl;
    return 0;
}

// http://www.math.uni-hamburg.de/doc/java/tutorial/native1.1/implementing/array.html
IMPL_CGSS_JNI_FUNC(jint, wave, DefaultWaveGenerator, decode8BitU)(JNIEnv *env, jclass klass, jfloat data, jbyteArray buffer, jint cursor) {
    return decode_wave<1>(CDefaultWaveGenerator::Decode8BitU, env, data, buffer, cursor);
}

IMPL_CGSS_JNI_FUNC(jint, wave, DefaultWaveGenerator, decode16BitS)(JNIEnv *env, jclass klass, jfloat data, jbyteArray buffer, jint cursor) {
    return decode_wave<2>(CDefaultWaveGenerator::Decode16BitS, env, data, buffer, cursor);
}

IMPL_CGSS_JNI_FUNC(jint, wave, DefaultWaveGenerator, decode24BitS)(JNIEnv *env, jclass klass, jfloat data, jbyteArray buffer, jint cursor) {
    return decode_wave<3>(CDefaultWaveGenerator::Decode24BitS, env, data, buffer, cursor);
}

IMPL_CGSS_JNI_FUNC(jint, wave, DefaultWaveGenerator, decode32BitS)(JNIEnv *env, jclass klass, jfloat data, jbyteArray buffer, jint cursor) {
    return decode_wave<4>(CDefaultWaveGenerator::Decode32BitS, env, data, buffer, cursor);
}

IMPL_CGSS_JNI_FUNC(jint, wave, DefaultWaveGenerator, decodeFloat)(JNIEnv *env, jclass klass, jfloat data, jbyteArray buffer, jint cursor) {
    return decode_wave<4>(CDefaultWaveGenerator::DecodeFloat, env, data, buffer, cursor);
}

IMPL_CGSS_JNI_FUNC(void, streams, Stream, seek)(JNIEnv *env, jobject thiz, jlong offset, jint origin) {
    auto *stream = jni::get_ptr_field_t<CStream>(env, thiz);
    if (!stream) {
        return;
    }
    stream->Seek(offset, static_cast<StreamSeekOrigin>(origin));
}

IMPL_CGSS_JNI_FUNC(jbyte, streams, Stream, readByte)(JNIEnv *env, jobject thiz) {
    auto *stream = jni::get_ptr_field_t<CStream>(env, thiz);
    if (!stream) {
        env->ThrowNew(env->FindClass("Ljava/io/IOException"), "JNI: CStream::readByte");
        return 0;
    }
    const auto val = stream->ReadByte();
    return direct_cast<jbyte>(val);
}

IMPL_CGSS_JNI_FUNC(jint, streams, Stream, writeByte)(JNIEnv *env, jobject thiz, jbyte byte) {
    auto *stream = jni::get_ptr_field_t<CStream>(env, thiz);
    if (!stream) {
        env->ThrowNew(env->FindClass("Ljava/io/IOException"), "JNI: CStream::writeByte");
        return 0;
    }
    const auto b = direct_cast<uint8_t>(byte);
    const auto written = stream->WriteByte(b);
    return static_cast<jint>(written);
}

IMPL_CGSS_JNI_FUNC(void, streams, Stream, copyTo)(JNIEnv *env, jobject thiz, jobject destStream, jint bufferSize) {
    if (bufferSize <= 0) {
        env->ThrowNew(env->FindClass("Ljava/lang/IllegalArgumentException"), "JNI: CStream::copyTo: bufferSize");
        return;
    }
    auto *src = jni::get_ptr_field_t<CStream>(env, thiz);
    if (!src) {
        env->ThrowNew(env->FindClass("Ljava/io/IOException"), "JNI: CStream::copyTo: src");
        return;
    }
    auto *dest = jni::get_ptr_field_t<CStream>(env, destStream);
    if (!dest) {
        env->ThrowNew(env->FindClass("Ljava/io/IOException"), "JNI: CStream::copyTo: dest");
        return;
    }
    uint32_t s = static_cast<uint32_t>(bufferSize);
    src->CopyTo(*dest, s);
}

IMPL_CGSS_JNI_FUNC(void, streams, MemoryStream, initNewBuffer)(JNIEnv *env, jobject thiz, jlong capacity, jboolean resizable) {
    if (capacity < 0) {
        env->ThrowNew(env->FindClass("Ljava/lang/IllegalArgumentException"), "JNI: CMemoryStream::ctor: capacity");
        return;
    }
    uint64_t cap = static_cast<uint64_t>(capacity);
    bool_t r = static_cast<bool_t>(resizable);
    const auto stream = new CMemoryStream(cap, r);
    jni::set_ptr_field_t(env, thiz, stream);
}

IMPL_CGSS_JNI_FUNC(jboolean, hca, HcaFormatReader, isWritable)(JNIEnv *env, jobject thiz) {
    auto *reader = jni::get_ptr_field_t<CHcaFormatReader>(env, thiz);
    if (!reader) {
        env->ThrowNew(env->FindClass("Ljava/io/IOException"), "JNI: HcaFormatReader.isWritable");
        return static_cast<jboolean>(FALSE);
    }
    return static_cast<jboolean>(reader->IsWritable());
}

IMPL_CGSS_JNI_FUNC(jboolean, hca, HcaFormatReader, isReadable)(JNIEnv *env, jobject thiz) {
    auto *reader = jni::get_ptr_field_t<CHcaFormatReader>(env, thiz);
    if (!reader) {
        env->ThrowNew(env->FindClass("Ljava/io/IOException"), "JNI: HcaFormatReader.isWritable");
        return static_cast<jboolean>(FALSE);
    }
    return static_cast<jboolean>(reader->IsReadable());
}

IMPL_CGSS_JNI_FUNC(jboolean, hca, HcaFormatReader, isSeekable)(JNIEnv *env, jobject thiz) {
    auto *reader = jni::get_ptr_field_t<CHcaFormatReader>(env, thiz);
    if (!reader) {
        env->ThrowNew(env->FindClass("Ljava/io/IOException"), "JNI: HcaFormatReader.isWritable");
        return static_cast<jboolean>(FALSE);
    }
    return static_cast<jboolean>(reader->IsSeekable());
}

IMPL_CGSS_JNI_FUNC(jobject, hca, HcaFormatReader, getHcaInfo)(JNIEnv *env, jobject thiz) {
    auto *reader = jni::get_ptr_field_t<CHcaFormatReader>(env, thiz);
    if (!reader) {
        env->ThrowNew(env->FindClass("Ljava/io/IOException"), "JNI: HcaFormatReader.getHcaInfo");
        return static_cast<jobject>(NULL);
    }

    const jclass clazz = env->FindClass("Lmoe.mottomo.cgss.hca.HcaInfo");
    if (!clazz) {
        return static_cast<intptr_t>(NULL);
    }
    const jmethodID ctorID = env->GetMethodID(clazz, "<ctor>", "()V");
    if (!ctorID) {
        return static_cast<intptr_t>(NULL);
    }

    HCA_INFO info;
    reader->GetHcaInfo(info);
    auto instance = env->NewObject(clazz, ctorID);
#define STR(x) #x
#define XSTR(x) STR(x)
#define F(type, field) jni::setField(env, instance, XSTR(field), C<j##type>(info. field))
    F(short, versionMajor);
    F(short, versionMinor);
    F(int, channelCount);
    F(int, samplingRate);
    F(int, blockCount);
    F(short, blockSize);
    F(short, athType);
    F(boolean, loopExists);
    F(int, loopStart);
    F(int, loopEnd);
    F(float, rvaVolume);

    F(short, fmtR01);
    F(short, fmtR02);
    F(short, compR01);
    F(short, compR02);
    F(short, compR03);
    F(short, compR04);
    F(short, compR05);
    F(short, compR06);
    F(short, compR07);
    F(short, compR08);
    F(int, compR09);
    F(short, vbrR01);
    F(short, vbrR02);
    F(short, loopR01);
    F(short, loopR02);

    F(int, dataOffset);
#undef F
#undef XSTR
#undef STR

    auto enumClass = env->FindClass("Lmoe/mottomo/cgss/hca/HcaCipherType");
    auto vo = env->GetStaticMethodID(enumClass, "valueOf", "(I)V");
    auto val = env->CallStaticObjectMethod(enumClass, vo);
    jni::setField(env, instance, "cipherType", "moe.mottomo.cgss.hca.HcaCipherType", val);

    return instance;
}
