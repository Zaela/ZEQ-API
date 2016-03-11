
#ifndef _ZEQ_PFS_HPP_
#define _ZEQ_PFS_HPP_

#include "define.hpp"
#include "path.hpp"
#include "file.hpp"
#include "FreeImage.h"
#include <string>
#include <vector>
#include <unordered_map>
#include <algorithm>

class PFS
{
private:
    struct Header
    {
        uint32_t offset;
        uint32_t signature;
        uint32_t unknown;
    };
    
    struct BlockHeader
    {
        uint32_t deflatedLen;
        uint32_t inflatedLen;
    };
    
    struct DirEntry
    {
        uint32_t crc;
        uint32_t offset;
        uint32_t inflatedLen;
    };
    
    struct ProcessedEntry
    {
        uint32_t    crc;
        uint32_t    offset;
        uint32_t    deflatedLen;
        uint32_t    inflatedLen;
    };

private:
    std::string m_path;

    uint32_t    m_rawLen;
    byte*       m_rawData;

    static const uint32_t SIGNATURE = 0x20534650; // "PFS "

    std::vector<ProcessedEntry>                             m_fileEntries;
    std::unordered_map<std::string, uint32_t>               m_byName;
    std::unordered_map<std::string, std::vector<uint32_t>>  m_byExtension;

private:
    byte* decompressEntry(uint32_t i, uint32_t& len);
    
public:
    PFS(const std::string& path);
    ~PFS();

    byte* getEntry(const std::string& name, uint32_t& len);

    bool hasWLD() const;
    bool hasTER() const;
    bool hasZON() const;
};

#endif//_ZEQ_PFS_HPP_
