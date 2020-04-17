#pragma once

#include <jni.h>
#include <cgss_api.h>

#include "jni_string.h"

CGSS_NS_BEGIN

    namespace jni {

        namespace java_exceptions {

            extern const char *IOException;
            extern const char *IllegalArgumentException;
            extern const char *CgssException;

        }

        template<class TOut, class TIn>
        static TOut direct_cast(const TIn &in) {
            return *(TOut *)&in;
        };

        void *getNativePtr(JNIEnv *env, jobject object);

        /**
         *
         * @remarks
         *   This function is not robust. To make it work correctly, you must ensure (actual type of the pointer is noted as U):
         *   1) T is a supertype of U;
         *   2) There is no multiple inheritance in the type tree until T;
         *   3) The compiler implements inheritance by V-table approach. (Fortunately mainstream compilers act this way.)
         *
         * @tparam T Desired type of the pointer.
         * @param env
         * @param object The Java object that holds a pointer field.
         * @return
         */
        template<class T>
        T *getNativePtrAs(JNIEnv *env, jobject object) {
            auto ptr = getNativePtr(env, object);
            return reinterpret_cast<T *>(ptr);
        }

        void setNativePtr(JNIEnv *env, jobject object, const void *ptr);

        template<class T>
        void setNativePtrAs(JNIEnv *env, jobject object, const T *ptr) {
            setNativePtr(env, object, reinterpret_cast<const void *>(ptr));
        }

        jboolean getBooleanField(JNIEnv *env, jobject thiz, const char *name);

        jbyte getByteField(JNIEnv *env, jobject thiz, const char *name);

        jshort getShortField(JNIEnv *env, jobject thiz, const char *name);

        jint getIntField(JNIEnv *env, jobject thiz, const char *name);

        jlong getLongField(JNIEnv *env, jobject thiz, const char *name);

        jfloat getFloatField(JNIEnv *env, jobject thiz, const char *name);

        jdouble getDoubleField(JNIEnv *env, jobject thiz, const char *name);

        jobject getObjectField(JNIEnv *env, jobject thiz, const char *name, const char *fullClassName);

        void setField(JNIEnv *env, jobject thiz, const char *name, jboolean val);

        void setField(JNIEnv *env, jobject thiz, const char *name, jbyte val);

        void setField(JNIEnv *env, jobject thiz, const char *name, jshort val);

        void setField(JNIEnv *env, jobject thiz, const char *name, jint val);

        void setField(JNIEnv *env, jobject thiz, const char *name, jlong val);

        void setField(JNIEnv *env, jobject thiz, const char *name, jfloat val);

        void setField(JNIEnv *env, jobject thiz, const char *name, jdouble val);

        /**
         *
         * @param env
         * @param thiz
         * @param name
         * @param fullClassName Complete class name with dot (.) as identifier separator (like that in C#). Does not support private classes.
         * @param val
         */
        void setField(JNIEnv *env, jobject thiz, const char *name, const char *fullClassName, jobject val);

        int32_t computeAvailableBufferSize(int32_t bufferSize, int32_t offset, int32_t count);

        void throwCgssException(JNIEnv *env, const std::string &message, CGSS_OP_RESULT opResult = CGSS_OP_GENERIC_FAULT);

        void throwCgssException(JNIEnv *env, const CException &exception);

    }

CGSS_NS_END
