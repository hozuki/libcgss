#pragma once

#include <string>
#include <jni.h>
#include <cgss_intf.h>

#include "cgss_jni_ns.h"

CGSS_JNI_NS_BEGIN

        namespace java_exceptions {

            extern const char *IOException;
            extern const char *IllegalArgumentException;
            extern const char *ClassNotFoundException;
            extern const char *NoSuchFieldException;
            extern const char *NoSuchMethodException;
            extern const char *CgssException;

        }

        template<class TOut, class TIn>
        static TOut direct_cast(const TIn &in) {
            return *(TOut *)&in;
        };

        void *getNativePtr(JNIEnv *env, jobject object, const char *classNameDescription);

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
        T *getNativePtrAs(JNIEnv *env, jobject object, const char *classNameDescription) {
            auto ptr = getNativePtr(env, object, classNameDescription);
            return reinterpret_cast<T *>(ptr);
        }

        void setNativePtr(JNIEnv *env, jobject object, const void *ptr, const char *classNameDescription);

        template<class T>
        void setNativePtrAs(JNIEnv *env, jobject object, const T *ptr, const char *classNameDescription) {
            setNativePtr(env, object, reinterpret_cast<const void *>(ptr), classNameDescription);
        }

        jboolean getBooleanField(JNIEnv *env, jobject thiz, const char *fieldName, const char *classNameDescription);

        jbyte getByteField(JNIEnv *env, jobject thiz, const char *fieldName, const char *classNameDescription);

        jshort getShortField(JNIEnv *env, jobject thiz, const char *fieldName, const char *classNameDescription);

        jint getIntField(JNIEnv *env, jobject thiz, const char *fieldName, const char *classNameDescription);

        jlong getLongField(JNIEnv *env, jobject thiz, const char *fieldName, const char *classNameDescription);

        jfloat getFloatField(JNIEnv *env, jobject thiz, const char *fieldName, const char *classNameDescription);

        jdouble getDoubleField(JNIEnv *env, jobject thiz, const char *fieldName, const char *classNameDescription);

        jstring getStringField(JNIEnv *env, jobject thiz, const char *fieldName, const char *classNameDescription);

        /**
         *
         * @param env
         * @param thiz
         * @param fieldName
         * @param fullFieldClassName Complete class name with dot (.) as identifier delimiter (like that in C#), or native Java class path (/). For private classes use '+' as delimiter.
         * @returns
         */
        jobject getObjectField(JNIEnv *env, jobject thiz, const char *fieldName, const char *fullFieldClassName, const char *classNameDescription);

        void setField(JNIEnv *env, jobject thiz, const char *fieldName, jboolean val, const char *classNameDescription);

        void setField(JNIEnv *env, jobject thiz, const char *fieldName, jbyte val, const char *classNameDescription);

        void setField(JNIEnv *env, jobject thiz, const char *fieldName, jshort val, const char *classNameDescription);

        void setField(JNIEnv *env, jobject thiz, const char *fieldName, jint val, const char *classNameDescription);

        void setField(JNIEnv *env, jobject thiz, const char *fieldName, jlong val, const char *classNameDescription);

        void setField(JNIEnv *env, jobject thiz, const char *fieldName, jfloat val, const char *classNameDescription);

        void setField(JNIEnv *env, jobject thiz, const char *fieldName, jdouble val, const char *classNameDescription);

        void setField(JNIEnv *env, jobject thiz, const char *fieldName, jstring val, const char *classNameDescription);

        /**
         *
         * @param env
         * @param thiz
         * @param fieldName
         * @param fullFieldClassName Complete class name with dot (.) as identifier delimiter (like that in C#), or native Java class path (/). For private classes use '+' as delimiter.
         * @param val
         */
        void setField(JNIEnv *env, jobject thiz, const char *fieldName, const char *fullFieldClassName, jobject val, const char *classNameDescription);

        void throwCgssException(JNIEnv *env, const std::string &message, CGSS_OP_RESULT opResult = CGSS_OP_GENERIC_FAULT);

        void throwCgssException(JNIEnv *env, const CException &exception);

        void checkClassInJava(JNIEnv *env, jclass clazz, const char *classNameDescription);

        void checkFieldInJava(JNIEnv *env, jfieldID fieldId, const char *classNameDescription, const char *fieldNameDescription);

        void checkMethodInJava(JNIEnv *env, jmethodID methodId, const char *classNameDescription, const char *methodNameDescription);

CGSS_JNI_NS_END
