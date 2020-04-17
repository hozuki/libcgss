#include "HcaDecoderConfig.h"
#include "../../jni_helper.h"
#include "../cgss_jni_class_names.h"
#include "HcaCipherConfig.h"

using namespace cgss;

void ApplyDecoderConfigObject(JNIEnv *env, jobject config, HCA_DECODER_CONFIG &cfg) {
    auto cipherConfig = jni::getObjectField(env, config, "cipherConfig", jni::cgss_class_names::HcaCipherConfig_Path, jni::cgss_class_names::HcaDecoderConfig_Name);
    ApplyCipherConfigObject(env, cipherConfig, cfg.cipherConfig);

    auto waveHeaderEnabled = jni::getBooleanField(env, config, "waveHeaderEnabled", jni::cgss_class_names::HcaDecoderConfig_Name);
    cfg.waveHeaderEnabled = waveHeaderEnabled;

    auto loopEnabled = jni::getBooleanField(env, config, "loopEnabled", jni::cgss_class_names::HcaDecoderConfig_Name);
    cfg.loopEnabled = loopEnabled;

    auto loopCount = jni::getIntField(env, config, "loopCount", jni::cgss_class_names::HcaDecoderConfig_Name);
    cfg.loopCount = loopCount;

    auto decodeFunc = jni::getObjectField(env, config, "decodeFunc", jni::cgss_class_names::HcaDecodeFunction_Path, jni::cgss_class_names::HcaDecoderConfig_Name);

    HcaDecodeFunc df = nullptr;

    if (decodeFunc != nullptr) {
        auto decodeFuncClass = env->FindClass(jni::cgss_class_names::HcaDecodeFunction_Path);
        jni::checkClassInJava(env, decodeFuncClass, jni::cgss_class_names::HcaDecodeFunction_Name);

        auto valueMethodSignature = jni::signature::param_types >> jni::signature::return_type >> jni::signature::jint >> jni::signature::to_string;
        auto valueMethodId = env->GetMethodID(decodeFuncClass, "value", valueMethodSignature.c_str());
        jni::checkMethodInJava(env, valueMethodId, jni::cgss_class_names::HcaDecodeFunction_Name, "value");

        auto decodeFuncValue = env->CallIntMethod(decodeFunc, valueMethodId);

        switch (decodeFuncValue) {
            case 1:
                df = CDefaultWaveGenerator::Decode8BitU;
                break;
            case 2:
                df = CDefaultWaveGenerator::Decode16BitS;
                break;
            case 3:
                df = CDefaultWaveGenerator::Decode24BitS;
                break;
            case 4:
                df = CDefaultWaveGenerator::Decode32BitS;
                break;
            case 5:
                df = CDefaultWaveGenerator::DecodeFloat;
                break;
            default:
                break;
        }
    }

    cfg.decodeFunc = df;
}
