#include <stdexcept>
#include <algorithm>
#include <cassert>

#include "jni_helper_signature.h"

CGSS_JNI_NS_BEGIN

        namespace signature {

            namespace __signature_internal {

                jni_data_type jni_data_type::primitive(int typeId, const std::string &typeName) noexcept {
                    return jni_data_type{typeId, typeName, nullptr};
                }

                jni_data_type jni_data_type::primitive(int typeId) noexcept {
                    return jni_data_type{typeId, "", nullptr};
                }

                jni_data_type jni_data_type::object(const std::string &typeName) noexcept {
                    return jni_data_type{typeid_jobject, typeName, nullptr};
                }

                jni_data_type jni_data_type::string() noexcept {
                    return jni_data_type{typeid_jstring, "Ljava/lang/String", nullptr};
                }

                jni_data_type jni_data_type::array_of(const jni_data_type &dataType) noexcept {
                    auto dt = new jni_data_type{dataType};
                    auto ptr = std::shared_ptr<jni_data_type>(dt);
                    return jni_data_type{typeid_jarray, "", ptr};
                }

                int jni_data_type::type_id() const {
                    return _typeId;
                }

                const std::string &jni_data_type::type_name() const {
                    return _typeName;
                }

                const std::shared_ptr<jni_data_type> &jni_data_type::underlying_type() const {
                    return _underlyingType;
                }

                jni_data_type::jni_data_type(int typeId, const std::string &typeName, const std::shared_ptr<jni_data_type> &underlying)
                    : _typeId(typeId), _typeName(typeName), _underlyingType(underlying) {
                }

                jni_signature::jni_signature()
                    : _op(opmode_none), _returnType(jvoid) {
                }

                int &jni_signature::op() {
                    return _op;
                }

                int jni_signature::op() const {
                    return _op;
                }

                std::vector<jni_data_type> &jni_signature::param_types() {
                    return _paramTypes;
                }

                const std::vector<jni_data_type> &jni_signature::param_types() const {
                    return _paramTypes;
                }

                jni_data_type &jni_signature::return_type() {
                    return _returnType;
                }

                const jni_data_type &jni_signature::return_type() const {
                    return _returnType;
                }

            }

            using namespace __signature_internal;

            const jni_signature_declare_params param_types;
            const jni_signature_declare_return return_type;

            const jni_data_type jvoid = jni_data_type::primitive(jni_data_type::typeid_jvoid, "void");
            const jni_data_type jboolean = jni_data_type::primitive(jni_data_type::typeid_jboolean, "boolean");
            const jni_data_type jchar = jni_data_type::primitive(jni_data_type::typeid_jchar, "char");
            const jni_data_type jbyte = jni_data_type::primitive(jni_data_type::typeid_jbyte, "byte");
            const jni_data_type jshort = jni_data_type::primitive(jni_data_type::typeid_jshort, "short");
            const jni_data_type jint = jni_data_type::primitive(jni_data_type::typeid_jint, "int");
            const jni_data_type jlong = jni_data_type::primitive(jni_data_type::typeid_jlong, "long");
            const jni_data_type jfloat = jni_data_type::primitive(jni_data_type::typeid_jfloat, "float");
            const jni_data_type jdouble = jni_data_type::primitive(jni_data_type::typeid_jdouble, "double");
            const jni_data_type jstring = jni_data_type::string();

            jni_data_type jobject(const std::string &typeName) {
                return jni_data_type::object(typeName);
            }

            jni_data_type jarray(const jni_data_type &underlyingType) {
                return jni_data_type::array_of(underlyingType);
            }

            const jni_signature_to_string to_string;

            std::string data_type_signature(const jni_data_type &dataType) {
                switch (dataType.type_id()) {
                    case jni_data_type::typeid_jvoid:
                        return "V";
                    case jni_data_type::typeid_jboolean:
                        return "Z";
                    case jni_data_type::typeid_jchar:
                        return "C";
                    case jni_data_type::typeid_jbyte:
                        return "B";
                    case jni_data_type::typeid_jshort:
                        return "S";
                    case jni_data_type::typeid_jint:
                        return "I";
                    case jni_data_type::typeid_jlong:
                        return "J";
                    case jni_data_type::typeid_jfloat:
                        return "F";
                    case jni_data_type::typeid_jdouble:
                        return "D";
                    case jni_data_type::typeid_jstring:
                        return "Ljava/lang/String;";
                    case jni_data_type::typeid_jobject: {
                        std::string name = dataType.type_name();

                        if (name.find('.') != std::string::npos) {
                            std::replace(name.begin(), name.end(), '.', '/');
                        }

                        return "L" + name + ";";
                    }
                    case jni_data_type::typeid_jarray: {
                        const auto &ptr = dataType.underlying_type();

                        assert(ptr != nullptr);
                        assert(ptr->type_id() != jni_data_type::typeid_jvoid);

                        auto underlyingName = data_type_signature(*ptr);
                        return "[" + underlyingName;
                    }
                    default:
                        throw std::invalid_argument("invalid data type");
                }
            }

        }

CGSS_JNI_NS_END

using namespace cgss::jni::signature;

jni_signature operator>>(const jni_signature &sig, const jni_data_type &dataType) {
    auto sig2 = sig;

    switch (sig.op()) {
        case jni_signature::opmode_params: {
            if (dataType.type_id() == jni_data_type::typeid_jvoid) {
                throw std::runtime_error("invalid param type");
            }

            sig2.param_types().push_back(dataType);
            break;
        }
        case jni_signature::opmode_return:
            sig2.return_type() = dataType;
            sig2.op() = jni_signature::opmode_complete;
            break;
        default:
            throw std::runtime_error("invalid signature concatenation");
    }

    return sig2;
}

jni_signature operator>>(const jni_signature_declare_params &op, const jni_data_type &dataType) {
    jni_signature sig;
    sig.op() = jni_signature::opmode_params;
    sig.param_types().push_back(dataType);
    return sig;
}

jni_signature operator>>(const jni_signature_declare_params &op1, const jni_signature_declare_return &op2) {
    jni_signature sig;
    sig.op() = jni_signature::opmode_return;
    return sig;
}

jni_signature operator>>(const jni_signature &sig, const jni_signature_declare_return &op) {
    auto sig2 = sig;
    sig2.op() = jni_signature::opmode_return;
    return sig2;
}

std::string operator>>(const jni_signature &sig, const jni_signature_to_string &op) {
    if (sig.op() != jni_signature::opmode_complete) {
        throw std::runtime_error("invalid signature state");
    }

    std::string param_types_str;

    if (!sig.param_types().empty()) {
        for (const auto &dt : sig.param_types()) {
            auto name = data_type_signature(dt);
            param_types_str.append(name);
        }
    }

    auto return_type_str = data_type_signature(sig.return_type());

    auto completeSignature = "(" + param_types_str + ")" + return_type_str;

    return completeSignature;
}
