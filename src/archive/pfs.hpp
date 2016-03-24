
#ifndef _ZEQ_PFS_HPP_
#define _ZEQ_PFS_HPP_

#include "define.hpp"
#include "path.hpp"
#include "file.hpp"
#include "FreeImage.h"
#include "model_listing.hpp"
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
    class Iterator
    {
    private:
        PFS* m_pfs;
        std::unordered_map<std::string, uint32_t>::iterator m_nameIter;
    
    public:
        Iterator(PFS* pfs, std::unordered_map<std::string, uint32_t>::iterator iter) : m_pfs(pfs), m_nameIter(iter) { }
        Iterator& operator++() { m_nameIter++; return *this; }
        bool operator==(const Iterator& o) { return m_nameIter == o.m_nameIter; }
        bool operator!=(const Iterator& o) { return m_nameIter != o.m_nameIter; }
        
        const std::string& name() { return (*m_nameIter).first; }
        byte* data(uint32_t& len) { return m_pfs->getEntry((*m_nameIter).first, len); }
    };
    
public:
    PFS(const std::string& path);
    ~PFS();

    byte* getEntry(const std::string& name, uint32_t& len);

    bool hasWLD() const;
    bool hasTER() const;
    bool hasZON() const;

    Iterator begin()    { return Iterator(this, m_byName.begin()); }
    Iterator end()      { return Iterator(this, m_byName.end()); }
};

#endif//_ZEQ_PFS_HPP_
