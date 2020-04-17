#include "jni_helper_string.h"

#if defined(WIN32) || defined(_WIN32)

#define WIN32_LEAN_AND_MEAN

#include <Windows.h>

#endif

CGSS_JNI_NS_BEGIN

        static char *utf8ToAnsi(const char *str);

        static char *ansiToUtf8(const char *str);

        static void freeString(char **pstr);

        std::string utf8ToAnsi(JNIEnv *env, jstring str) {
            jboolean isCopy;
            auto utf8Str = env->GetStringUTFChars(str, &isCopy);
            auto ansiStr = utf8ToAnsi(utf8Str);

            std::string result(ansiStr);

            freeString(&ansiStr);
            env->ReleaseStringUTFChars(str, utf8Str);

            return result;
        }

        jstring ansiToUtf8(JNIEnv *env, const std::string &str) {
            auto utf8Str = ansiToUtf8(str.c_str());

            auto javaStr = env->NewStringUTF(utf8Str);

            freeString(&utf8Str);

            return javaStr;
        }

#if defined(WIN32) || defined(_WIN32)

        static char *utf8ToAnsi(const char *str) {
            int nLen = MultiByteToWideChar(CP_UTF8, NULL, str, -1, nullptr, 0);
            auto wszBuffer = new WCHAR[nLen + 1];
            nLen = MultiByteToWideChar(CP_UTF8, NULL, str, -1, wszBuffer, nLen);
            wszBuffer[nLen] = '\0';

            nLen = WideCharToMultiByte(CP_ACP, 0, wszBuffer, -1, nullptr, 0, nullptr, nullptr);
            auto szBuffer = new char[nLen + 1];
            nLen = WideCharToMultiByte(CP_ACP, 0, wszBuffer, -1, szBuffer, nLen, nullptr, nullptr);
            szBuffer[nLen] = '\0';

            delete[] wszBuffer;

            return szBuffer;
        }

        static char *ansiToUtf8(const char *str) {
            int nLen = MultiByteToWideChar(CP_ACP, NULL, str, -1, nullptr, 0);
            auto wszBuffer = new WCHAR[nLen + 1];
            nLen = MultiByteToWideChar(CP_ACP, NULL, str, -1, wszBuffer, nLen);
            wszBuffer[nLen] = '\0';

            nLen = WideCharToMultiByte(CP_UTF8, 0, wszBuffer, -1, nullptr, 0, nullptr, nullptr);
            auto szBuffer = new char[nLen + 1];
            nLen = WideCharToMultiByte(CP_UTF8, 0, wszBuffer, -1, szBuffer, nLen, nullptr, nullptr);
            szBuffer[nLen] = '\0';

            delete[] wszBuffer;

            return szBuffer;
        }

        static void freeString(char **pstr) {
            if (pstr) {
                delete[] *pstr;
                *pstr = nullptr;
            }
        }

#else

        static char *utf8ToAnsi(const char *str) {
            return const_cast<char *>(str);
        }

        static char *ansiToUtf8(const char *str) {
            return const_cast<char *>(str);
        }

        static void freeString(char **pstr) {
            // Do nothing
        }

#endif

CGSS_JNI_NS_END