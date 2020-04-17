#include "HcaCipherConfig.h"
#include "../../jni_helper.h"
#include "../cgss_jni_class_names.h"

using namespace cgss;

void ApplyCipherConfigObject(JNIEnv *env, jobject config, HCA_CIPHER_CONFIG &cfg) {
    auto cipherType = jni::getObjectField(env, config, "cipherType", jni::cgss_class_names::HcaCipherType_Path, jni::cgss_class_names::HcaCipherConfig_Name);

    {
        auto cipherTypeClass = env->FindClass(jni::cgss_class_names::HcaCipherType_Path);
        auto valueMethodId = env->GetMethodID(cipherTypeClass, "value", "()I");
        auto cipherTypeValue = env->CallIntMethod(cipherType, valueMethodId);

        cfg.cipherType = static_cast<CGSS_HCA_CIPHER_TYPE>(cipherTypeValue);
    }

    auto rawKey = jni::getLongField(env, config, "rawKey", jni::cgss_class_names::HcaCipherConfig_Name);
    cfg.key = jni::direct_cast<uint64_t>(rawKey);

    auto keyModifier = jni::getShortField(env, config, "keyModifier", jni::cgss_class_names::HcaCipherConfig_Name);
    cfg.keyModifier = jni::direct_cast<uint16_t>(keyModifier);
}
