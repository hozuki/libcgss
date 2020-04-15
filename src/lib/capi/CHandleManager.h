#pragma once

#include <map>
#include "../cgss_env.h"
#include "../takamori/streams/IStream.h"

CGSS_NS_BEGIN

    class CHandleManager {

    public:

        enum class HandleType : uint32_t {
            None = 0x00,
            CStream = 0x01,
            CHcaReaderBase = 0x03
        };

        CHandleManager(const CHandleManager &) = delete;

        CHandleManager(CHandleManager &&) = delete;

        CHandleManager &operator=(const CHandleManager &) = delete;

        CHandleManager &operator=(CHandleManager &&) = delete;

        uint32_t alloc(IStream *p, HandleType type);

        void free(uint32_t handle, bool_t dispose = TRUE);

        bool_t handleExists(uint32_t handle) const;

        HandleType getHandleType(uint32_t handle) const;

        IStream *getHandlePtr(uint32_t handle) const;

        static CHandleManager *getInstance();

    private:

        CHandleManager() noexcept;

        ~CHandleManager();

        static CHandleManager *_instance;

        struct HandleRecord {
            IStream *ptr;
            HandleType type;
        };

        std::map<uint32_t, HandleRecord> _handles;

    };

CGSS_NS_END
