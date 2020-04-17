#include <jni.h>
#include <cgss_api.h>

#include "../../jni_helper.h"
#include "../cgss_jni_class_names.h"

using namespace cgss;

#ifdef __cplusplus
extern "C" {
#endif

JNIEXPORT void JNICALL Java_moe_mottomo_cgss_takamori_FileStream_init
    (JNIEnv *env, jobject thiz, jstring fileName, jint fileMode, jint fileAccess) {
    auto ansiFileName = jni::utf8ToAnsi(env, fileName);

    auto mode = static_cast<FileMode>(fileMode);
    auto access = static_cast<FileAccess>(fileAccess);

    CFileStream *fs = nullptr;

    try {
        fs = new CFileStream(ansiFileName, mode, access);
    } catch (CException &e) {
        jni::throwCgssException(env, e);
    }

    jni::setNativePtr(env, thiz, fs, jni::cgss_class_names::FileStream_Name);
}

JNIEXPORT jint JNICALL Java_moe_mottomo_cgss_takamori_FileStream_read
    (JNIEnv *env, jobject thiz, jbyteArray buffer, jint offset, jint count) {
    if (buffer == nullptr) {
        env->ThrowNew(env->FindClass(jni::java_exceptions::IOException), "JNI: FileStream::read: buffer pointer is null");
        return 0;
    }

    auto stream = jni::getNativePtrAs<CFileStream>(env, thiz, jni::cgss_class_names::FileStream_Name);

    if (stream == nullptr) {
        env->ThrowNew(env->FindClass(jni::java_exceptions::IOException), "JNI: FileStream::read: pointer is null");
        return 0;
    }

    auto bufferSize = env->GetArrayLength(buffer);

    auto maxRead = jni::math::computeAvailableBufferSize(bufferSize, offset, count);

    if (maxRead == 0) {
        return 0;
    }

    auto body = new jbyte[maxRead];

    memset(body, 0, maxRead);

    const auto b = reinterpret_cast<uint8_t *>(body);

    int32_t read = 0;

    try {
        read = stream->Read(b, maxRead, 0, maxRead);

        env->SetByteArrayRegion(buffer, offset, read, body);
    } catch (CException &e) {
        jni::throwCgssException(env, e);
    }

    delete[] body;

    return read;
}

JNIEXPORT jint JNICALL Java_moe_mottomo_cgss_takamori_FileStream_write
    (JNIEnv *env, jobject thiz, jbyteArray buffer, jint offset, jint count) {
    if (buffer == nullptr) {
        env->ThrowNew(env->FindClass(jni::java_exceptions::IOException), "JNI: FileStream::write: buffer pointer is null");
        return 0;
    }

    auto stream = jni::getNativePtrAs<CFileStream>(env, thiz, jni::cgss_class_names::FileStream_Name);

    if (stream == nullptr) {
        env->ThrowNew(env->FindClass(jni::java_exceptions::IOException), "JNI: FileStream::write: pointer is null");
        return 0;
    }

    auto bufferSize = env->GetArrayLength(buffer);

    auto maxWrite = jni::math::computeAvailableBufferSize(bufferSize, offset, count);

    if (maxWrite == 0) {
        return 0;
    }

    auto body = new jbyte[maxWrite];
    const auto b = reinterpret_cast<uint8_t *>(body);

    env->GetByteArrayRegion(buffer, offset, maxWrite, body);

    int32_t written = 0;

    try {
        written = stream->Write(b, maxWrite, 0, maxWrite);
    } catch (CException &e) {
        jni::throwCgssException(env, e);
    }

    delete[] body;

    return written;
}

JNIEXPORT jboolean JNICALL Java_moe_mottomo_cgss_takamori_FileStream_isWritable
    (JNIEnv *env, jobject thiz) {
    auto stream = jni::getNativePtrAs<CFileStream>(env, thiz, jni::cgss_class_names::FileStream_Name);

    if (stream == nullptr) {
        env->ThrowNew(env->FindClass(jni::java_exceptions::IOException), "JNI: FileStream::isWritable: pointer is null");
        return JNI_FALSE;
    }

    try {
        return static_cast<jboolean>(stream->IsWritable());
    } catch (CException &e) {
        jni::throwCgssException(env, e);
        return JNI_FALSE;
    }
}

JNIEXPORT jboolean JNICALL Java_moe_mottomo_cgss_takamori_FileStream_isReadable
    (JNIEnv *env, jobject thiz) {
    auto stream = jni::getNativePtrAs<CFileStream>(env, thiz, jni::cgss_class_names::FileStream_Name);

    if (stream == nullptr) {
        env->ThrowNew(env->FindClass(jni::java_exceptions::IOException), "JNI: FileStream::isReadable: pointer is null");
        return JNI_FALSE;
    }

    try {
        return static_cast<jboolean>(stream->IsReadable());
    } catch (CException &e) {
        jni::throwCgssException(env, e);
        return JNI_FALSE;
    }
}

JNIEXPORT jboolean JNICALL Java_moe_mottomo_cgss_takamori_FileStream_isSeekable
    (JNIEnv *env, jobject thiz) {
    auto stream = jni::getNativePtrAs<CFileStream>(env, thiz, jni::cgss_class_names::FileStream_Name);

    if (stream == nullptr) {
        env->ThrowNew(env->FindClass(jni::java_exceptions::IOException), "JNI: FileStream::isSeekable: pointer is null");
        return JNI_FALSE;
    }

    try {
        return static_cast<jboolean>(stream->IsSeekable());
    } catch (CException &e) {
        jni::throwCgssException(env, e);
        return 0;
    }
}

JNIEXPORT jlong JNICALL Java_moe_mottomo_cgss_takamori_FileStream_getPosition
    (JNIEnv *env, jobject thiz) {
    auto stream = jni::getNativePtrAs<CFileStream>(env, thiz, jni::cgss_class_names::FileStream_Name);

    if (stream == nullptr) {
        env->ThrowNew(env->FindClass(jni::java_exceptions::IOException), "JNI: FileStream::getPosition: pointer is null");
        return 0;
    }

    try {
        return static_cast<jlong>(stream->GetPosition());
    } catch (CException &e) {
        jni::throwCgssException(env, e);
        return 0;
    }
}

JNIEXPORT void JNICALL Java_moe_mottomo_cgss_takamori_FileStream_setPosition
    (JNIEnv *env, jobject thiz, jlong position) {
    auto stream = jni::getNativePtrAs<CFileStream>(env, thiz, jni::cgss_class_names::FileStream_Name);

    if (stream == nullptr) {
        env->ThrowNew(env->FindClass(jni::java_exceptions::IOException), "JNI: FileStream::setPosition: pointer is null");
        return;
    }

    try {
        stream->SetPosition(static_cast<uint64_t>(position));
    } catch (CException &e) {
        jni::throwCgssException(env, e);
    }
}

JNIEXPORT jlong JNICALL Java_moe_mottomo_cgss_takamori_FileStream_getLength
    (JNIEnv *env, jobject thiz) {
    auto stream = jni::getNativePtrAs<CFileStream>(env, thiz, jni::cgss_class_names::FileStream_Name);

    if (stream == nullptr) {
        env->ThrowNew(env->FindClass(jni::java_exceptions::IOException), "JNI: FileStream::getLength: pointer is null");
        return 0;
    }

    try {
        return static_cast<jlong>(stream->GetLength());
    } catch (CException &e) {
        jni::throwCgssException(env, e);
        return 0;
    }
}

JNIEXPORT void JNICALL Java_moe_mottomo_cgss_takamori_FileStream_setLength
    (JNIEnv *env, jobject thiz, jlong length) {
    auto stream = jni::getNativePtrAs<CFileStream>(env, thiz, jni::cgss_class_names::FileStream_Name);

    if (stream == nullptr) {
        env->ThrowNew(env->FindClass(jni::java_exceptions::IOException), "JNI: FileStream::setLength: pointer is null");
        return;
    }

    try {
        stream->SetLength(static_cast<uint64_t>(length));
    } catch (CException &e) {
        jni::throwCgssException(env, e);
    }
}

JNIEXPORT void JNICALL Java_moe_mottomo_cgss_takamori_FileStream_flush
    (JNIEnv *env, jobject thiz) {
    auto stream = jni::getNativePtrAs<CFileStream>(env, thiz, jni::cgss_class_names::FileStream_Name);

    if (stream == nullptr) {
        env->ThrowNew(env->FindClass(jni::java_exceptions::IOException), "JNI: FileStream::flush: pointer is null");
        return;
    }

    try {
        stream->Flush();
    } catch (CException &e) {
        jni::throwCgssException(env, e);
    }
}

JNIEXPORT void JNICALL Java_moe_mottomo_cgss_takamori_FileStream_dispose
    (JNIEnv *env, jobject thiz) {
    auto stream = jni::getNativePtrAs<CFileStream>(env, thiz, jni::cgss_class_names::FileStream_Name);

    if (stream == nullptr) {
        return;
    }

    try {
        delete stream;
    } catch (CException &e) {
        jni::throwCgssException(env, e);
    }

    jni::setNativePtr(env, thiz, nullptr, jni::cgss_class_names::FileStream_Name);
}

#ifdef __cplusplus
}
#endif