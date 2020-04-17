#include <stdexcept>
#include <algorithm>
#include <cassert>

#if defined(WIN32) || defined(_WIN32)
#define NOMINMAX
#endif

#include "jni_helper_reflection.h"
#include "jni_helper_string.h"
#include "jni_helper_signature.h"

CGSS_JNI_NS_BEGIN

        namespace java_exceptions {

            const char *IOException = "java/io/IOException";
            const char *IllegalArgumentException = "java/lang/IllegalArgumentException";
            const char *ClassNotFoundException = "java/lang/ClassNotFoundException";
            const char *NoSuchFieldException = "java/lang/NoSuchFieldException";
            const char *NoSuchMethodException = "java/lang/NoSuchMethodException";
            const char *CgssException = "moe/mottomo/cgss/takamori/CgssException";

        }

        void *getNativePtr(JNIEnv *env, jobject object, const char *classNameDescription) {
            jclass nativeObjectClass = env->GetObjectClass(object);
            checkClassInJava(env, nativeObjectClass, classNameDescription);

            if (!nativeObjectClass) {
                return nullptr;
            }

            auto getterSignature = signature::param_types >> signature::return_type >> signature::jlong >> signature::to_string;
            jmethodID getter = env->GetMethodID(nativeObjectClass, "getNativePtr", getterSignature.c_str());
            checkMethodInJava(env, getter, classNameDescription, "getNativePtr():long");

            if (getter == nullptr) {
                return nullptr;
            }

            jlong ptrValue = env->CallLongMethod(object, getter);

            auto u = direct_cast<uint64_t>(ptrValue);
            auto intptr = static_cast<uintptr_t>(u);

            return direct_cast<void *>(intptr);
        }

        void setNativePtr(JNIEnv *env, jobject object, const void *ptr, const char *classNameDescription) {
            jclass nativeObjectClass = env->GetObjectClass(object);
            checkClassInJava(env, nativeObjectClass, classNameDescription);

            if (!nativeObjectClass) {
                return;
            }

            auto setterSignature = signature::param_types >> signature::jlong >> signature::return_type >> signature::jvoid >> signature::to_string;
            jmethodID setter = env->GetMethodID(nativeObjectClass, "setNativePtr", setterSignature.c_str());
            checkMethodInJava(env, setter, classNameDescription, "setNativePtr(long):void");

            if (setter == nullptr) {
                return;
            }

            auto intptr = direct_cast<uintptr_t>(ptr);
            auto u = static_cast<uint64_t>(intptr);
            auto val = direct_cast<jlong>(u);

            env->CallVoidMethod(object, setter, val);
        }

        jboolean getBooleanField(JNIEnv *env, jobject thiz, const char *fieldName, const char *classNameDescription) {
            jclass clazz = env->GetObjectClass(thiz);
            checkClassInJava(env, clazz, classNameDescription);

            if (!clazz) {
                return JNI_FALSE;
            }

            jfieldID fieldID = env->GetFieldID(clazz, fieldName, "Z");
            checkFieldInJava(env, fieldID, classNameDescription, fieldName);

            if (!fieldID) {
                return JNI_FALSE;
            }

            auto val = env->GetBooleanField(thiz, fieldID);

            return val;
        }

        jbyte getByteField(JNIEnv *env, jobject thiz, const char *fieldName, const char *classNameDescription) {
            jclass clazz = env->GetObjectClass(thiz);
            checkClassInJava(env, clazz, classNameDescription);

            if (!clazz) {
                return 0;
            }

            jfieldID fieldID = env->GetFieldID(clazz, fieldName, "B");
            checkFieldInJava(env, fieldID, classNameDescription, fieldName);

            if (!fieldID) {
                return 0;
            }

            auto val = env->GetByteField(thiz, fieldID);

            return val;
        }

        jshort getShortField(JNIEnv *env, jobject thiz, const char *fieldName, const char *classNameDescription) {
            jclass clazz = env->GetObjectClass(thiz);
            checkClassInJava(env, clazz, classNameDescription);

            if (!clazz) {
                return 0;
            }

            jfieldID fieldID = env->GetFieldID(clazz, fieldName, "S");
            checkFieldInJava(env, fieldID, classNameDescription, fieldName);

            if (!fieldID) {
                return 0;
            }

            auto val = env->GetByteField(thiz, fieldID);

            return val;
        }

        jint getIntField(JNIEnv *env, jobject thiz, const char *fieldName, const char *classNameDescription) {
            jclass clazz = env->GetObjectClass(thiz);
            checkClassInJava(env, clazz, classNameDescription);

            if (!clazz) {
                return 0;
            }

            jfieldID fieldID = env->GetFieldID(clazz, fieldName, "I");
            checkFieldInJava(env, fieldID, classNameDescription, fieldName);

            if (!fieldID) {
                return 0;
            }

            auto val = env->GetIntField(thiz, fieldID);

            return val;
        }

        jlong getLongField(JNIEnv *env, jobject thiz, const char *fieldName, const char *classNameDescription) {
            jclass clazz = env->GetObjectClass(thiz);
            checkClassInJava(env, clazz, classNameDescription);

            if (!clazz) {
                return 0;
            }

            jfieldID fieldID = env->GetFieldID(clazz, fieldName, "J");
            checkFieldInJava(env, fieldID, classNameDescription, fieldName);

            if (!fieldID) {
                return 0;
            }

            auto val = env->GetLongField(thiz, fieldID);

            return val;
        }

        jfloat getFloatField(JNIEnv *env, jobject thiz, const char *fieldName, const char *classNameDescription) {
            jclass clazz = env->GetObjectClass(thiz);
            checkClassInJava(env, clazz, classNameDescription);

            if (!clazz) {
                return 0;
            }

            jfieldID fieldID = env->GetFieldID(clazz, fieldName, "F");
            checkFieldInJava(env, fieldID, classNameDescription, fieldName);

            if (!fieldID) {
                return 0;
            }

            auto val = env->GetFloatField(thiz, fieldID);

            return val;
        }

        jdouble getDoubleField(JNIEnv *env, jobject thiz, const char *fieldName, const char *classNameDescription) {
            jclass clazz = env->GetObjectClass(thiz);
            checkClassInJava(env, clazz, classNameDescription);

            if (!clazz) {
                throw std::runtime_error("jni::getDoubleField");
            }

            jfieldID fieldID = env->GetFieldID(clazz, fieldName, "D");
            checkFieldInJava(env, fieldID, classNameDescription, fieldName);

            if (!fieldID) {
                return 0;
            }

            auto val = env->GetDoubleField(thiz, fieldID);

            return val;
        }

        jstring getStringField(JNIEnv *env, jobject thiz, const char *fieldName, const char *classNameDescription) {
            auto javaObject = getObjectField(env, thiz, fieldName, "java/lang/String", classNameDescription);
            return reinterpret_cast<jstring>(javaObject);
        }

        jobject getObjectField(JNIEnv *env, jobject thiz, const char *fieldName, const char *fullFieldClassName, const char *classNameDescription) {
            jclass clazz = env->GetObjectClass(thiz);
            checkClassInJava(env, clazz, classNameDescription);

            if (!clazz) {
                throw std::runtime_error("jni::getObjectField");
            }

            auto clsName = "L" + std::string(fullFieldClassName) + ";";

            if (clsName.find('.') != std::string::npos) {
                std::replace(clsName.begin(), clsName.end(), '.', '/');
            }

            jfieldID fieldID = env->GetFieldID(clazz, fieldName, clsName.c_str());
            checkFieldInJava(env, fieldID, classNameDescription, fieldName);

            if (!fieldID) {
                throw std::runtime_error("jni::getObjectField");
            }

            auto val = env->GetObjectField(thiz, fieldID);

            return val;
        }

        void setField(JNIEnv *env, jobject thiz, const char *fieldName, jboolean val, const char *classNameDescription) {
            jclass clazz = env->GetObjectClass(thiz);
            checkClassInJava(env, clazz, classNameDescription);

            if (!clazz) {
                return;
            }

            jfieldID fieldID = env->GetFieldID(clazz, fieldName, "Z");
            checkFieldInJava(env, fieldID, classNameDescription, fieldName);

            if (!fieldID) {
                return;
            }

            env->SetBooleanField(thiz, fieldID, val);
        }

        void setField(JNIEnv *env, jobject thiz, const char *fieldName, jbyte val, const char *classNameDescription) {
            jclass clazz = env->GetObjectClass(thiz);
            checkClassInJava(env, clazz, classNameDescription);

            if (!clazz) {
                return;
            }

            jfieldID fieldID = env->GetFieldID(clazz, fieldName, "B");
            checkFieldInJava(env, fieldID, classNameDescription, fieldName);

            if (!fieldID) {
                return;
            }

            env->SetByteField(thiz, fieldID, val);
        }

        void setField(JNIEnv *env, jobject thiz, const char *fieldName, jshort val, const char *classNameDescription) {
            jclass clazz = env->GetObjectClass(thiz);
            checkClassInJava(env, clazz, classNameDescription);

            if (!clazz) {
                return;
            }

            jfieldID fieldID = env->GetFieldID(clazz, fieldName, "S");
            checkFieldInJava(env, fieldID, classNameDescription, fieldName);

            if (!fieldID) {
                return;
            }

            env->SetShortField(thiz, fieldID, val);
        }

        void setField(JNIEnv *env, jobject thiz, const char *fieldName, jint val, const char *classNameDescription) {
            jclass clazz = env->GetObjectClass(thiz);
            checkClassInJava(env, clazz, classNameDescription);

            if (!clazz) {
                return;
            }

            jfieldID fieldID = env->GetFieldID(clazz, fieldName, "I");
            checkFieldInJava(env, fieldID, classNameDescription, fieldName);

            if (!fieldID) {
                return;
            }

            env->SetIntField(thiz, fieldID, val);
        }

        void setField(JNIEnv *env, jobject thiz, const char *fieldName, jlong val, const char *classNameDescription) {
            jclass clazz = env->GetObjectClass(thiz);
            checkClassInJava(env, clazz, classNameDescription);

            if (!clazz) {
                return;
            }

            jfieldID fieldID = env->GetFieldID(clazz, fieldName, "J");
            checkFieldInJava(env, fieldID, classNameDescription, fieldName);

            if (!fieldID) {
                return;
            }

            env->SetLongField(thiz, fieldID, val);
        }

        void setField(JNIEnv *env, jobject thiz, const char *fieldName, jfloat val, const char *classNameDescription) {
            jclass clazz = env->GetObjectClass(thiz);
            checkClassInJava(env, clazz, classNameDescription);

            if (!clazz) {
                return;
            }

            jfieldID fieldID = env->GetFieldID(clazz, fieldName, "F");
            checkFieldInJava(env, fieldID, classNameDescription, fieldName);

            if (!fieldID) {
                return;
            }

            env->SetFloatField(thiz, fieldID, val);
        }

        void setField(JNIEnv *env, jobject thiz, const char *fieldName, jdouble val, const char *classNameDescription) {
            jclass clazz = env->GetObjectClass(thiz);
            checkClassInJava(env, clazz, classNameDescription);

            if (!clazz) {
                return;
            }

            jfieldID fieldID = env->GetFieldID(clazz, fieldName, "D");
            checkFieldInJava(env, fieldID, classNameDescription, fieldName);

            if (!fieldID) {
                return;
            }

            env->SetDoubleField(thiz, fieldID, val);
        }

        void setField(JNIEnv *env, jobject thiz, const char *fieldName, jstring val, const char *classNameDescription) {
            setField(env, thiz, fieldName, "java/lang/String", val, classNameDescription);
        }

        void setField(JNIEnv *env, jobject thiz, const char *fieldName, const char *fullFieldClassName, jobject val, const char *classNameDescription) {
            jclass clazz = env->GetObjectClass(thiz);
            checkClassInJava(env, clazz, classNameDescription);

            if (!clazz) {
                return;
            }

            auto clsName = "L" + std::string(fullFieldClassName) + ";";

            if (clsName.find('.') != std::string::npos) {
                std::replace(clsName.begin(), clsName.end(), '.', '/');
            }

            jfieldID fieldID = env->GetFieldID(clazz, fieldName, clsName.c_str());
            checkFieldInJava(env, fieldID, classNameDescription, fieldName);

            if (!fieldID) {
                return;
            }

            env->SetObjectField(thiz, fieldID, val);
        }

        void throwCgssException(JNIEnv *env, const std::string &message, CGSS_OP_RESULT opResult) {
            auto clazz = env->FindClass(java_exceptions::CgssException);
            auto ctorSignature = signature::param_types >> signature::jstring >> signature::jint >> signature::return_type >> signature::jvoid >> signature::to_string;
            auto methodId = env->GetMethodID(clazz, "<init>", ctorSignature.c_str());

            auto javaStr = ansiToUtf8(env, message);
            auto opResultInt = static_cast<jint>(opResult);

            auto throwable = env->NewObject(clazz, methodId, javaStr, opResultInt);

            env->Throw((jthrowable)throwable);
        }

        void throwCgssException(JNIEnv *env, const CException &exception) {
            throwCgssException(env, exception.GetExceptionMessage(), exception.GetOpResult());
        }

        void checkClassInJava(JNIEnv *env, jclass clazz, const char *classNameDescription) {
            if (clazz != nullptr) {
                return;
            }

            std::string message;

            if (classNameDescription != nullptr) {
                message = classNameDescription;
            } else {
                message = "<unknown>";
            }

            auto exceptionClass = env->FindClass(java_exceptions::ClassNotFoundException);

            assert(exceptionClass != nullptr);

            env->ThrowNew(exceptionClass, message.c_str());
        }

        void checkFieldInJava(JNIEnv *env, jfieldID methodId, const char *classNameDescription, const char *fieldNameDescription) {
            if (methodId != nullptr) {
                return;
            }

            std::string className, fieldName;

            if (classNameDescription != nullptr) {
                className = classNameDescription;
            } else {
                className = "<unknown>";
            }

            if (fieldNameDescription != nullptr) {
                fieldName = fieldNameDescription;
            } else {
                fieldName = "<unknown>";
            }

            auto message = className + "::" + fieldName;

            auto exceptionClass = env->FindClass(java_exceptions::NoSuchFieldException);

            assert(exceptionClass != nullptr);

            env->ThrowNew(exceptionClass, message.c_str());
        }

        void checkMethodInJava(JNIEnv *env, jmethodID methodId, const char *classNameDescription, const char *methodNameDescription) {
            if (methodId != nullptr) {
                return;
            }

            std::string className, methodName;

            if (classNameDescription != nullptr) {
                className = classNameDescription;
            } else {
                className = "<unknown>";
            }

            if (methodNameDescription != nullptr) {
                methodName = methodNameDescription;
            } else {
                methodName = "<unknown>";
            }

            auto message = className + "::" + methodName;

            auto exceptionClass = env->FindClass(java_exceptions::NoSuchMethodException);

            assert(exceptionClass != nullptr);

            env->ThrowNew(exceptionClass, message.c_str());
        }

CGSS_JNI_NS_END
