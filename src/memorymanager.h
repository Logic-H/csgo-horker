#pragma once

#include <string>
#include <vector>
#include <unordered_map>
#include <sys/uio.h>

class MemoryManager {
    public:
        MemoryManager(const std::string &processName = "");
        ~MemoryManager() {};
        bool Attach(std::string processName = "");
        bool ParseModules();
        bool IsValid();
        uintptr_t GetCallAddress(uintptr_t address);
        uintptr_t FindInModule(const std::string &moduleName, const std::string &pattern, size_t offset);
        uintptr_t GetModuleStart(const std::string &moduleName);

        template <class T>
        inline bool Read(void* address, T& value, ssize_t rlen = sizeof(T)) {
            struct iovec local[1];
            struct iovec remote[1];
            ssize_t nread;

            local[0].iov_base = reinterpret_cast<void*>(&value);
            local[0].iov_len = rlen;
            remote[0].iov_base = address;
            remote[0].iov_len = rlen;

            nread = process_vm_readv(m_pid, local, 1, remote, 1, 0);
            return (nread == rlen);
        }

        template <class T>
        inline bool Read(uintptr_t address, T& value, ssize_t rlen = sizeof(T))
        {
            return this->Read(reinterpret_cast<void*>(address), value, rlen);
            struct iovec local[1];
            struct iovec remote[1];
            ssize_t nread;

            local[0].iov_base = reinterpret_cast<void*>(&value);
            local[0].iov_len = rlen;
            remote[0].iov_base = reinterpret_cast<void*>(address);
            remote[0].iov_len = rlen;

            nread = process_vm_readv(m_pid, local, 1, remote, 1, 0);
            return (nread == rlen);
        }

        template <class T>
        inline bool Write(uintptr_t address, T value, ssize_t rlen = sizeof(T))
        {
            T tempValue = value;
            struct iovec local[1];
            struct iovec remote[1];
            ssize_t nread;

            local[0].iov_base = reinterpret_cast<void*>(&tempValue);
            local[0].iov_len = rlen;
            remote[0].iov_base = reinterpret_cast<void*>(address);
            remote[0].iov_len = rlen;

            nread = process_vm_writev(m_pid, local, 1, remote, 1, 0);
            return (nread == rlen);
        }

        inline void WriteMulti(struct iovec *local, struct iovec *remote, size_t count)
        {
            process_vm_writev(m_pid, local, count, remote, count, 0);
        }

    private:
        struct Module {
            uintptr_t start;
            uintptr_t end;
        };
        void InitHexLookup();
        std::vector<uint8_t> HexToBin(const std::string &str);
        std::unordered_map<std::string, Module> m_modules;
        long m_pid = -1;
        char m_lookup[255];
        std::string m_processName;
};

