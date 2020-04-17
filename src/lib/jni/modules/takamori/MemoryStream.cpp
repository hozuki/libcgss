#include <jni.h>
#include <cgss_api.h>

#include "../../jni_helper.hpp"

using namespace cgss;

#ifdef __cplusplus
extern "C" {
#endif

static void WriteToUnderlyingJavaBuffer(JNIEnv *env, CMemoryStream *stream) {
    if (stream == nullptr || stream->javaBuffer == nullptr) {
        return;
    }

    auto array = (jbyteArray)stream->javaBuffer;
    auto body = reinterpret_cast<jbyte *>(stream->GetBuffer());

    // TODO: We don't need to update the whole array all the time.
    env->SetByteArrayRegion(array, 0, stream->GetLength(), body);
}

static void ReadFromUnderlyingJavaBuffer(JNIEnv *env, CMemoryStream *stream) {
    if (stream == nullptr || stream->javaBuffer == nullptr) {
        return;
    }

    auto array = (jbyteArray)stream->javaBuffer;
    auto body = reinterpret_cast<jbyte *>(stream->GetBuffer());

    // TODO: We don't need to update the whole array all the time.
    env->GetByteArrayRegion(array, 0, stream->GetLength(), body);
}

JNIEXPORT jint JNICALL Java_moe_mottomo_cgss_takamori_MemoryStream_read
    (JNIEnv *env, jobject thiz, jbyteArray buffer, jint offset, jint count) {
    if (buffer == nullptr) {
        env->ThrowNew(env->FindClass(jni::java_exceptions::IOException), "JNI: MemoryStream::read: buffer pointer is null");
        return 0;
    }

    auto stream = jni::getNativePtrAs<CMemoryStream>(env, thiz);

    if (stream == nullptr) {
        env->ThrowNew(env->FindClass(jni::java_exceptions::IOException), "JNI: MemoryStream::read: pointer is null");
        return 0;
    }

    ReadFromUnderlyingJavaBuffer(env, stream);

    const auto bufferSize = env->GetArrayLength(buffer);
    auto maxRead = jni::computeAvailableBufferSize(bufferSize, offset, count);

    if (maxRead == 0) {
        return 0;
    }

    auto unsignedBuffer = new uint8_t[maxRead];

    int32_t read = 0;

    try {
        read = stream->Read(unsignedBuffer, maxRead, 0, maxRead);
    } catch (CException &e) {
        jni::throwCgssException(env, e);
    }

    if (read > 0) {
        auto signedBuffer = reinterpret_cast<jbyte *>(unsignedBuffer);

        env->SetByteArrayRegion(buffer, offset, read, signedBuffer);
    }

    delete[] unsignedBuffer;

    return read;
}

JNIEXPORT jint JNICALL Java_moe_mottomo_cgss_takamori_MemoryStream_write
    (JNIEnv *env, jobject thiz, jbyteArray buffer, jint offset, jint count) {
    if (buffer == nullptr) {
        env->ThrowNew(env->FindClass(jni::java_exceptions::IOException), "JNI: MemoryStream::write: buffer pointer is null");
        return 0;
    }

    auto stream = jni::getNativePtrAs<CMemoryStream>(env, thiz);

    if (stream == nullptr) {
        env->ThrowNew(env->FindClass(jni::java_exceptions::IOException), "JNI: MemoryStream::write: pointer is null");
        return 0;
    }

    const auto bufferSize = env->GetArrayLength(buffer);
    auto maxWrite = jni::computeAvailableBufferSize(bufferSize, offset, count);

    if (maxWrite == 0) {
        return 0;
    }

    auto unsignedBuffer = new uint8_t[maxWrite];
    auto signedBuffer = reinterpret_cast<jbyte *>(unsignedBuffer);

    env->GetByteArrayRegion(buffer, offset, maxWrite, signedBuffer);

    int32_t written = 0;

    try {
        written = stream->Write(unsignedBuffer, maxWrite, 0, maxWrite);
    } catch (CException &e) {
        jni::throwCgssException(env, e);
    }

    delete[] unsignedBuffer;

    if (written > 0) {
        if (stream->javaBuffer != nullptr) {
            WriteToUnderlyingJavaBuffer(env, stream);
        }
    }

    return written;
}

JNIEXPORT jboolean JNICALL Java_moe_mottomo_cgss_takamori_MemoryStream_isWritable
    (JNIEnv *env, jobject thiz) {
    auto stream = jni::getNativePtrAs<CMemoryStream>(env, thiz);

    if (stream == nullptr) {
        env->ThrowNew(env->FindClass(jni::java_exceptions::IOException), "JNI: MemoryStream::isWritable: pointer is null");
        return 0;
    }

    return stream->IsWritable();
}

JNIEXPORT jboolean JNICALL Java_moe_mottomo_cgss_takamori_MemoryStream_isReadable
    (JNIEnv *env, jobject thiz) {
    auto stream = jni::getNativePtrAs<CMemoryStream>(env, thiz);

    if (stream == nullptr) {
        env->ThrowNew(env->FindClass(jni::java_exceptions::IOException), "JNI: MemoryStream::isReadable: pointer is null");
        return 0;
    }

    return stream->IsReadable();
}

JNIEXPORT jboolean JNICALL Java_moe_mottomo_cgss_takamori_MemoryStream_isSeekable
    (JNIEnv *env, jobject thiz) {
    auto stream = jni::getNativePtrAs<CMemoryStream>(env, thiz);

    if (stream == nullptr) {
        env->ThrowNew(env->FindClass(jni::java_exceptions::IOException), "JNI: MemoryStream::isSeekable: pointer is null");
        return 0;
    }

    return stream->IsSeekable();
}

JNIEXPORT jlong JNICALL Java_moe_mottomo_cgss_takamori_MemoryStream_getPosition
    (JNIEnv *env, jobject thiz) {
    auto stream = jni::getNativePtrAs<CMemoryStream>(env, thiz);

    if (stream == nullptr) {
        env->ThrowNew(env->FindClass(jni::java_exceptions::IOException), "JNI: MemoryStream::getPosition: pointer is null");
        return 0;
    }

    try {
        return stream->GetPosition();
    } catch (CException &e) {
        jni::throwCgssException(env, e);
        return 0;
    }
}

JNIEXPORT void JNICALL Java_moe_mottomo_cgss_takamori_MemoryStream_setPosition
    (JNIEnv *env, jobject thiz, jlong position) {
    auto stream = jni::getNativePtrAs<CMemoryStream>(env, thiz);

    if (stream == nullptr) {
        env->ThrowNew(env->FindClass(jni::java_exceptions::IOException), "JNI: MemoryStream::setPosition: pointer is null");
        return;
    }

    try {
        stream->SetPosition(position);
    } catch (CException &e) {
        jni::throwCgssException(env, e);
    }
}

JNIEXPORT jlong JNICALL Java_moe_mottomo_cgss_takamori_MemoryStream_getLength
    (JNIEnv *env, jobject thiz) {
    auto stream = jni::getNativePtrAs<CMemoryStream>(env, thiz);

    if (stream == nullptr) {
        env->ThrowNew(env->FindClass(jni::java_exceptions::IOException), "JNI: MemoryStream::getLength: pointer is null");
        return 0;
    }

    try {
        return stream->GetLength();
    } catch (CException &e) {
        jni::throwCgssException(env, e);
        return 0;
    }
}

JNIEXPORT void JNICALL Java_moe_mottomo_cgss_takamori_MemoryStream_setLength
    (JNIEnv *env, jobject thiz, jlong length) {
    auto stream = jni::getNativePtrAs<CMemoryStream>(env, thiz);

    if (stream == nullptr) {
        env->ThrowNew(env->FindClass(jni::java_exceptions::IOException), "JNI: MemoryStream::setLength: pointer is null");
        return;
    }

    try {
        stream->SetLength(length);
    } catch (CException &e) {
        jni::throwCgssException(env, e);
    }
}

JNIEXPORT jbyteArray JNICALL Java_moe_mottomo_cgss_takamori_MemoryStream_toArray
    (JNIEnv *env, jobject thiz) {
    auto stream = jni::getNativePtrAs<CMemoryStream>(env, thiz);

    if (stream == nullptr) {
        env->ThrowNew(env->FindClass(jni::java_exceptions::IOException), "JNI: MemoryStream::toArray: pointer is null");
        return nullptr;
    }

    WriteToUnderlyingJavaBuffer(env, stream);

    const auto length = static_cast<jsize>(stream->GetLength());
    auto newByteArray = env->NewByteArray(length);

    auto srcPtr = stream->GetBuffer();
    auto dstPtr = new jbyte[length];

    memcpy(dstPtr, srcPtr, length);

    env->SetByteArrayRegion(newByteArray, 0, length, dstPtr);

    delete[] dstPtr;

    return newByteArray;
}

JNIEXPORT void JNICALL Java_moe_mottomo_cgss_takamori_MemoryStream_flush
    (JNIEnv *env, jobject thiz) {
    auto stream = jni::getNativePtrAs<CMemoryStream>(env, thiz);

    if (stream == nullptr) {
        env->ThrowNew(env->FindClass(jni::java_exceptions::IOException), "JNI: MemoryStream::flush: pointer is null");
        return;
    }

    WriteToUnderlyingJavaBuffer(env, stream);
}

JNIEXPORT void JNICALL Java_moe_mottomo_cgss_takamori_MemoryStream_initNewBuffer
    (JNIEnv *env, jobject thiz, jlong capacity, jboolean resizable) {
    auto mem = new CMemoryStream(capacity, resizable);

    jni::setNativePtr(env, thiz, mem);
}

JNIEXPORT void JNICALL Java_moe_mottomo_cgss_takamori_MemoryStream_initExtBuffer
    (JNIEnv *env, jobject thiz, jbyteArray buffer, jboolean writable) {
    if (buffer == nullptr) {
        env->ThrowNew(env->FindClass(jni::java_exceptions::IllegalArgumentException), "JNI: MemoryStream::<init>: buffer pointer is null");
        return;
    }

    auto bufferSize = env->GetArrayLength(buffer);
    auto extBuffer = new uint8_t[bufferSize];

    auto body = reinterpret_cast<jbyte *>(extBuffer);

    env->GetByteArrayRegion(buffer, 0, bufferSize, body);

    auto mem = new CMemoryStream(extBuffer, bufferSize, writable);

    jni::setNativePtr(env, thiz, mem);

    // Maintain a global reference to the buffer array
    mem->javaBuffer = env->NewGlobalRef(buffer);
}

JNIEXPORT void JNICALL Java_moe_mottomo_cgss_takamori_MemoryStream_dispose
    (JNIEnv *env, jobject thiz) {
    auto stream = jni::getNativePtrAs<CMemoryStream>(env, thiz);

    if (stream == nullptr) {
        return;
    }

    if (stream->javaBuffer != nullptr) {
        // We created the external buffer, so we have to delete it ourselves.
        auto extBuffer = stream->GetBuffer();
        delete[] extBuffer;

        env->DeleteGlobalRef(stream->javaBuffer);
        stream->javaBuffer = nullptr;
    }

    delete stream;

    jni::setNativePtr(env, thiz, nullptr);
}

#ifdef __cplusplus
}
#endif
