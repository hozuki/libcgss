#include "CHcaCipherConverter.h"
#include "../../common/quick_utils.h"
#include "internal/CHcaCipher.h"

CGSS_NS_BEGIN

    CHcaCipherConverter::CHcaCipherConverter(CStream *stream, const HCA_CIPHER_CONFIG &cryptFrom, const HCA_CIPHER_CONFIG &cryptTo)
            : super(stream), _cipherFrom(), _cipherTo() {
        clone(cryptFrom, _ccFrom);
        clone(cryptTo, _ccTo);
    }

    CHcaCipherConverter::~CHcaCipherConverter() {
        if (_cipherFrom) {
            delete _cipherFrom;
            _cipherFrom = nullptr;
        }
        if (_cipherTo) {
            delete _cipherTo;
            _cipherTo = nullptr;
        }
    }

CGSS_NS_END
