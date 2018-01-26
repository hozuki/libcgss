#ifdef _MSC_VER
#define _CRT_SECURE_NO_WARNINGS
#endif

#include "CHandleManager.h"
#include "../takamori/exceptions/CArgumentException.h"

CGSS_NS_BEGIN

    CHandleManager *CHandleManager::_instance = new CHandleManager();

    CHandleManager::CHandleManager() {
        HandleRecord record = {};
        record.ptr = nullptr;
        record.type = HandleType::None;

        _handles[0] = record;
    }

    CHandleManager::~CHandleManager() {
        for (const auto &r : _handles) {
            delete r.second.ptr;
        }
        _handles.clear();
    }

    CHandleManager::HandleType CHandleManager::getHandleType(uint32_t handle) const {
        const auto iter = _handles.find(handle);
        if (iter == _handles.cend()) {
            char buffer[100] = {0};
            sprintf(buffer, "Handle %u is invalid.", handle);
            throw CArgumentException(buffer);
        }
        return iter->second.type;
    }

    IStream *CHandleManager::getHandlePtr(uint32_t handle) const {
        const auto iter = _handles.find(handle);
        if (iter == _handles.cend()) {
            char buffer[100] = {0};
            sprintf(buffer, "Handle %u is invalid.", handle);
            throw CArgumentException(buffer);
        }
        return iter->second.ptr;
    }

    bool_t CHandleManager::handleExists(uint32_t handle) const {
        return static_cast<bool_t>(_handles.find(handle) != _handles.cend());
    }

    uint32_t CHandleManager::alloc(IStream *p, HandleType type) {
        uint32_t handle = 1;
        auto iter = _handles.find(handle);

        while (iter != _handles.cend()) {
            ++handle;
            iter = _handles.find(handle);
        }

        HandleRecord record = {};
        record.ptr = p;
        record.type = type;

        _handles[handle] = record;
        return handle;
    }

    void CHandleManager::free(uint32_t handle, bool_t dispose) {
        const auto iter = _handles.find(handle);
        if (iter == _handles.cend()) {
            char buffer[100] = {0};
            sprintf(buffer, "Handle %u is invalid.", handle);
            throw CArgumentException(buffer);
        }
        if (dispose) {
            delete iter->second.ptr;
        }
        _handles.erase(iter);
    }

    CHandleManager *CHandleManager::getInstance() {
        return _instance;
    }

CGSS_NS_END
