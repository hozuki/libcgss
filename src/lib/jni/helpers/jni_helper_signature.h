#pragma once

#include <vector>
#include <string>
#include <memory>

#include "cgss_jni_ns.h"

CGSS_JNI_NS_BEGIN

        namespace signature {

            namespace __signature_internal {

                struct jni_data_type;

                struct jni_data_type {

                private:

                    int _typeId;
                    std::string _typeName;
                    std::shared_ptr<jni_data_type> _underlyingType;

                public:

                    enum {
                        typeid_jvoid = 0,
                        typeid_jboolean = 1,
                        typeid_jchar = 2,
                        typeid_jbyte = 3,
                        typeid_jshort = 4,
                        typeid_jint = 5,
                        typeid_jlong = 6,
                        typeid_jfloat = 7,
                        typeid_jdouble = 8,
                        typeid_jobject = 9,
                        typeid_jstring = 10,
                        typeid_jarray = 11,
                    };

                    jni_data_type(const jni_data_type &) = default;

                    jni_data_type(jni_data_type &&) = default;

                    jni_data_type &operator=(const jni_data_type &) = default;

                    jni_data_type &operator=(jni_data_type &&) = default;

                    static jni_data_type primitive(int typeId) noexcept;

                    static jni_data_type primitive(int typeId, const std::string &typeName) noexcept;

                    static jni_data_type object(const std::string &typeName) noexcept;

                    static jni_data_type string() noexcept;

                    static jni_data_type array_of(const jni_data_type &dataType) noexcept;

                    int type_id() const;

                    const std::string &type_name() const;

                    const std::shared_ptr<jni_data_type> &underlying_type() const;

                private:

                    jni_data_type(int typeId, const std::string &typeName, const std::shared_ptr<jni_data_type> &underlying);

                };

                struct jni_signature {

                private:

                    int _op;
                    std::vector<jni_data_type> _paramTypes;
                    jni_data_type _returnType;

                public:

                    enum {
                        opmode_none = 0,
                        opmode_params = 1,
                        opmode_return = 2,
                        opmode_complete = 3,
                    };

                    jni_signature();

                    int &op();

                    int op() const;

                    std::vector<jni_data_type> &param_types();

                    const std::vector<jni_data_type> &param_types() const;

                    jni_data_type &return_type();

                    const jni_data_type &return_type() const;

                };

                struct jni_signature_declare_params {
                };

                struct jni_signature_declare_return {
                };

                struct jni_signature_to_string {
                };

            }

            // Public APIs

            extern const __signature_internal::jni_signature_declare_params param_types;
            extern const __signature_internal::jni_signature_declare_return return_type;

            extern const __signature_internal::jni_data_type jvoid;
            extern const __signature_internal::jni_data_type jboolean;
            extern const __signature_internal::jni_data_type jchar;
            extern const __signature_internal::jni_data_type jbyte;
            extern const __signature_internal::jni_data_type jshort;
            extern const __signature_internal::jni_data_type jint;
            extern const __signature_internal::jni_data_type jlong;
            extern const __signature_internal::jni_data_type jfloat;
            extern const __signature_internal::jni_data_type jdouble;
            extern const __signature_internal::jni_data_type jstring;

            extern const __signature_internal::jni_signature_to_string to_string;

            __signature_internal::jni_data_type jobject(const std::string &typeName);

            __signature_internal::jni_data_type jarray(const __signature_internal::jni_data_type &underlyingType);

            std::string data_type_signature(const __signature_internal::jni_data_type &dataType);

        }

CGSS_JNI_NS_END

// Helper operators

// chain
::cgss::jni::signature::__signature_internal::jni_signature operator>>(const ::cgss::jni::signature::__signature_internal::jni_signature &sig, const ::cgss::jni::signature::__signature_internal::jni_data_type &dataType);

// begin
::cgss::jni::signature::__signature_internal::jni_signature operator>>(const ::cgss::jni::signature::__signature_internal::jni_signature_declare_params &op, const ::cgss::jni::signature::__signature_internal::jni_data_type &dataType);

::cgss::jni::signature::__signature_internal::jni_signature operator>>(const ::cgss::jni::signature::__signature_internal::jni_signature_declare_params &op1, const ::cgss::jni::signature::__signature_internal::jni_signature_declare_return &op2);

// switch
::cgss::jni::signature::__signature_internal::jni_signature operator>>(const ::cgss::jni::signature::__signature_internal::jni_signature &sig, const ::cgss::jni::signature::__signature_internal::jni_signature_declare_return &op);

// output
std::string operator>>(const ::cgss::jni::signature::__signature_internal::jni_signature &sig, const ::cgss::jni::signature::__signature_internal::jni_signature_to_string &op);
