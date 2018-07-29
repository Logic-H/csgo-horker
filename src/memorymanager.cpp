#include "memorymanager.h"

#include <algorithm>
#include <chrono>
#include <fstream>
#include <iostream>
#include <sstream>
#include <thread>

#include <sys/types.h>
#include <sys/stat.h>
#include <dirent.h>
#include <error.h>
#include <errno.h>
#include <cstring>
#include <unistd.h>

static std::string SafeReadLink(const std::string &strPath)
{
    std::string path;
    char *buffer = new char[PATH_MAX];
    ssize_t len = ::readlink(strPath.data(), buffer, PATH_MAX);
    if (len != -1) {
        buffer[len] = 0;
        path = buffer;
    }
    delete [] buffer;

    return path;
}

void MemoryManager::InitHexLookup()
{
   m_lookup['0'] = 0;
   m_lookup['1'] = 1;
   m_lookup['2'] = 2;
   m_lookup['3'] = 3;
   m_lookup['4'] = 4;
   m_lookup['5'] = 5;
   m_lookup['6'] = 6;
   m_lookup['7'] = 7;
   m_lookup['8'] = 8;
   m_lookup['9'] = 9;

   m_lookup['A'] = 10;
   m_lookup['B'] = 11;
   m_lookup['C'] = 12;
   m_lookup['D'] = 13;
   m_lookup['E'] = 14;
   m_lookup['F'] = 15;

   m_lookup['a'] = 10;
   m_lookup['b'] = 11;
   m_lookup['c'] = 12;
   m_lookup['d'] = 13;
   m_lookup['e'] = 14;
   m_lookup['f'] = 15;
   m_lookup['?'] = 0;
}

std::vector<uint8_t> MemoryManager::HexToBin(const std::string &str)
{
    std::vector<uint8_t> bin;
    size_t len = str.size();
    if ((len + 1) % 3 != 0) {
        return bin;
    }
    for (size_t i = 0; i < len; i += 3) {
        bin.push_back((m_lookup[(uint8_t)str[i]]<<4) | m_lookup[(uint8_t)str[i+1]]);
    }

    return bin;
}

MemoryManager::MemoryManager(const std::string &processName)
{
    m_processName = processName;
    InitHexLookup();
}

bool MemoryManager::IsValid()
{
    std::string procPath("/proc/" + std::to_string(m_pid));
    struct stat st;
    if (stat(procPath.data(), &st) == 0 && S_ISDIR(st.st_mode)) {
        return true;
    }
    return false;
}

bool MemoryManager::Attach(std::string processName)
{
    if (processName.empty()) {
        processName = m_processName;
    }

    DIR *pdir = opendir("/proc");
    if (pdir) {
        struct dirent *entry;
        while ((entry = readdir(pdir)) != NULL) {
            long id = strtol(entry->d_name, NULL, 10);
            if (id != 0L) {
                std::string path("/proc/" + std::to_string(id) + "/exe");
                std::string rlbuf = SafeReadLink(path);
                size_t separator = rlbuf.find_last_of('/');
                if (separator != std::string::npos) {
                    if (rlbuf.compare(separator + 1, std::string::npos, processName) == 0) {
                        m_pid = id;
                        break;
                    }
                }
            }
        }
        closedir(pdir);
    }
    return m_pid != -1;
}

bool MemoryManager::ParseModules()
{
    char g1;
    std::string g2;
    uintptr_t start, end;
    std::string name;

    std::string absPath("/proc/" + std::to_string(this->m_pid) + "/maps"); 
    std::ifstream ifs(absPath, std::fstream::in | std::fstream::binary);
    if (!ifs.is_open()) {
        return false;
    }
    std::string line;
    while (std::getline(ifs, line)) {
        std::istringstream ss(line);
        ss >> std::hex >> start >> g1 >> end;
        ss >> g2 >> g2 >> g2 >> g2;
        std::getline(ss, name);
        name.erase(name.begin(), std::find_if(name.begin(),
            name.end(), [](int ch) { return !std::isspace(ch);
        }));
        size_t separator = name.find_last_of('/');
        if (separator != std::string::npos) {
            name = name.substr(separator + 1);
        }
        auto&& module = m_modules[name];
        if (module.start == 0) {
            module.start = start;
        }
        if (module.end < end) {
            module.end = end;
        }
    }
    ifs.close();
    
    return !m_modules.empty();
}

uintptr_t MemoryManager::GetModuleStart(const std::string &moduleName)
{
    if (m_modules.find(moduleName) != m_modules.end()) {
        return m_modules[moduleName].start;
    }
    return 0;
}

uintptr_t MemoryManager::FindInModule(const std::string &moduleName, const std::string &pattern, size_t offset)
{
    if (m_modules.find(moduleName) == m_modules.end()) {
        return 0;
    }
    auto &&module = m_modules[moduleName];

    const size_t buf_size = 0x1000;
    uint8_t buffer[buf_size];
    std::vector<uint8_t> bp = this->HexToBin(pattern);

    size_t chunknum = 0;
    size_t chunksize = sizeof(buffer);
    size_t totalsize = module.end - module.start;
    while (totalsize > 0) {
        size_t readsize = (totalsize < chunksize) ? totalsize : chunksize;
        uintptr_t readaddr = module.start + (chunksize * chunknum);
        memset(buffer, 0, chunksize);

        if (this->Read(readaddr, buffer, readsize)) {
            for (uintptr_t b = 0; b < readsize; b++) {
                size_t matches = 0;
                while (buffer[b + matches] == bp[matches] || pattern[matches*3] == '?') {
                    matches++;
                    if (matches == bp.size()) {
                        return (readaddr + b + offset);
                    }
                }
            }
        }
        totalsize -= readsize;
        chunknum++;
    }
    return 0;
}

uintptr_t MemoryManager::GetCallAddress(uintptr_t address)
{
    uintptr_t code = 0;
    if (this->Read(address, code, 4)) {
        return (address + code + 4);
    }
    return 0;
}

