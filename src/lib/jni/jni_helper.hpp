#pragma once

#include "cgss_jni.h"

struct jni {

    static intptr_t get_ptr_field(JNIEnv *env, jobject object) {
        const jclass nativeObjectClass = env->GetObjectClass(object);
        if (!nativeObjectClass) {
            return static_cast<intptr_t>(NULL);
        }
        const jfieldID nativePtrFieldID = env->GetFieldID(nativeObjectClass, "nativePtr", "J");
        if (!nativePtrFieldID) {
            return static_cast<intptr_t>(NULL);
        }
        jlong ptrValue = env->GetLongField(object, nativePtrFieldID);
        return static_cast<intptr_t>(ptrValue);
    }

    template<class T>
    static T *get_ptr_field_t(JNIEnv *env, jobject object) {
        auto ptr = get_ptr_field(env, object);
        // TODO: ?
        return (T *)ptr;
    }

    static void set_ptr_field(JNIEnv *env, jobject object, intptr_t ptr) {
        const jclass nativeObjectClass = env->GetObjectClass(object);
        if (!nativeObjectClass) {
            return;
        }
        const jfieldID nativePtrFieldID = env->GetFieldID(nativeObjectClass, "nativePtr", "J");
        if (!nativePtrFieldID) {
            return;
        }
        env->SetLongField(object, nativePtrFieldID, static_cast<jlong>(ptr));
    }

    template<class T>
    static void set_ptr_field_t(JNIEnv *env, jobject object, const T *ptr) {
        set_ptr_field(env, object, (intptr_t)ptr);
    }

    static jbyte getByteField(JNIEnv *env, jobject thiz, const char *name) {
        const jclass clazz = env->GetObjectClass(thiz);
        if (!clazz) {
            throw std::runtime_error("jni::getByteField");
        }
        const jfieldID fieldID = env->GetFieldID(clazz, name, "B");
        if (!fieldID) {
            throw std::runtime_error("jni::getByteField");
        }
        auto val = env->GetByteField(thiz, fieldID);
        return val;
    }

    static jint getIntField(JNIEnv *env, jobject thiz, const char *name) {
        const jclass clazz = env->GetObjectClass(thiz);
        if (!clazz) {
            throw std::runtime_error("jni::getIntField");
        }
        const jfieldID fieldID = env->GetFieldID(clazz, name, "I");
        if (!fieldID) {
            throw std::runtime_error("jni::getIntField");
        }
        auto val = env->GetIntField(thiz, fieldID);
        return val;
    }

    static jlong getLongField(JNIEnv *env, jobject thiz, const char *name) {
        const jclass clazz = env->GetObjectClass(thiz);
        if (!clazz) {
            throw std::runtime_error("jni::getLongField");
        }
        const jfieldID fieldID = env->GetFieldID(clazz, name, "J");
        if (!fieldID) {
            throw std::runtime_error("jni::getLongField");
        }
        auto val = env->GetLongField(thiz, fieldID);
        return val;
    }

    static jobject getObjectField(JNIEnv *env, jobject thiz, const char *name, const char *fullClassName) {
        char cls[256] = "L";
        strncat(cls, fullClassName, 254);
        for (auto i = 0; i < 256 && cls[i]; ++i) {
            if (cls[i] == '.') {
                cls[i] = '/';
            }
        }
        const jclass clazz = env->GetObjectClass(thiz);
        if (!clazz) {
            throw std::runtime_error("jni::getObjectField");
        }
        const jfieldID fieldID = env->GetFieldID(clazz, name, cls);
        if (!fieldID) {
            throw std::runtime_error("jni::getObjectField");
        }
        auto val = env->GetObjectField(thiz, fieldID);
        return val;
    }

    static void setField(JNIEnv *env, jobject thiz, const char *name, jbyte val) {
        const jclass clazz = env->GetObjectClass(thiz);
        if (!clazz) {
            return;
        }
        const jfieldID fieldID = env->GetFieldID(clazz, name, "B");
        if (!fieldID) {
            return;
        }
        env->SetByteField(thiz, fieldID, val);
    }

    static void setField(JNIEnv *env, jobject thiz, const char *name, jshort val) {
        const jclass clazz = env->GetObjectClass(thiz);
        if (!clazz) {
            return;
        }
        const jfieldID fieldID = env->GetFieldID(clazz, name, "S");
        if (!fieldID) {
            return;
        }
        env->SetShortField(thiz, fieldID, val);
    }

    static void setField(JNIEnv *env, jobject thiz, const char *name, jint val) {
        const jclass clazz = env->GetObjectClass(thiz);
        if (!clazz) {
            return;
        }
        const jfieldID fieldID = env->GetFieldID(clazz, name, "I");
        if (!fieldID) {
            return;
        }
        env->SetIntField(thiz, fieldID, val);
    }

    static void setField(JNIEnv *env, jobject thiz, const char *name, jlong val) {
        const jclass clazz = env->GetObjectClass(thiz);
        if (!clazz) {
            return;
        }
        const jfieldID fieldID = env->GetFieldID(clazz, name, "J");
        if (!fieldID) {
            return;
        }
        env->SetLongField(thiz, fieldID, val);
    }

    static void setField(JNIEnv *env, jobject thiz, const char *name, jboolean val) {
        const jclass clazz = env->GetObjectClass(thiz);
        if (!clazz) {
            return;
        }
        const jfieldID fieldID = env->GetFieldID(clazz, name, "Z");
        if (!fieldID) {
            return;
        }
        env->SetBooleanField(thiz, fieldID, val);
    }

    static void setField(JNIEnv *env, jobject thiz, const char *name, jfloat val) {
        const jclass clazz = env->GetObjectClass(thiz);
        if (!clazz) {
            return;
        }
        const jfieldID fieldID = env->GetFieldID(clazz, name, "F");
        if (!fieldID) {
            return;
        }
        env->SetFloatField(thiz, fieldID, val);
    }

    static void setField(JNIEnv *env, jobject thiz, const char *name, const char *fullClassName, jobject val) {
        char cls[256] = "L";
        strncat(cls, fullClassName, 254);
        for (auto i = 0; i < 256 && cls[i]; ++i) {
            if (cls[i] == '.') {
                cls[i] = '/';
            }
        }
        const jclass clazz = env->GetObjectClass(thiz);
        if (!clazz) {
            return;
        }
        const jfieldID fieldID = env->GetFieldID(clazz, name, cls);
        if (!fieldID) {
            return;
        }
        env->SetObjectField(thiz, fieldID, val);
    }

};
