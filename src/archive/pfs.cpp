
#include "pfs.hpp"

PFS::PFS(const std::string& path)
: m_path(path),
  m_rawLen(0),
  m_rawData(nullptr)
{
    m_rawData = File::openBinary(path, m_rawLen);
    
    if (!m_rawData)
        throw 1; //fixme
    
    uint32_t p = sizeof(Header);
    uint32_t n;

    if (p > m_rawLen)
        throw 1; //fixme
    
    Header* h = (Header*)m_rawData;
    
    if (h->signature != SIGNATURE)
        throw 1; //fixme
    
    p = h->offset;
    n = *(uint32_t*)&m_rawData[p];
    
    p += sizeof(uint32_t);
    
    for (uint32_t i = 0; i < n; i++)
    {
        DirEntry* src = (DirEntry*)&m_rawData[p];
        p += sizeof(DirEntry);
        
        if (p > m_rawLen)
            throw 1; //fixme
        
        ProcessedEntry ent;
        ent.crc         = src->crc;
        ent.offset      = src->offset;
        ent.inflatedLen = src->inflatedLen;
        
        uint32_t q = ent.offset;
        
        if (q > m_rawLen)
            throw 1; //fixme
        
        uint32_t ilen       = 0;
        uint32_t totalLen   = ent.inflatedLen;
        
        while (ilen < totalLen)
        {
            BlockHeader* bh = (BlockHeader*)&m_rawData[q];
            q += sizeof(BlockHeader);
            
            if (q > m_rawLen)
                throw 1; //fixme
            
            q += bh->deflatedLen;
            
            if (q > m_rawLen)
                throw 1; //fixme
            
            ilen += bh->inflatedLen;
        }
        
        ent.deflatedLen = q - ent.offset;
        
        m_fileEntries.push_back(ent);
    }
    
    if (m_fileEntries.size() < 2) // Must have name entry and some kind of content, or it may as well not exist
        throw 1; //fixme
    
    std::sort(m_fileEntries.begin(), m_fileEntries.end(), [](const ProcessedEntry& a, const ProcessedEntry& b) { return a.offset < b.offset; });
    
    // Retrieve name data entry
    byte* nameData = decompressEntry(m_fileEntries.size() - 1, n);
    m_fileEntries.pop_back();
    
    n = *(uint32_t*)nameData;
    p = sizeof(uint32_t);
    
    for (uint32_t i = 0; i < n; i++)
    {
        uint32_t len = *(uint32_t*)&nameData[p];
        p += sizeof(uint32_t);
        
        const char* name = (const char*)&nameData[p];
        p += len;
        
        std::string entName(name, len - 1); // len includes null terminator
        
        m_byName[entName] = i;
        
        std::string ext = zeq_path_t::getExtension(entName);
        
        m_byExtension[ext].push_back(i);
    }
    
    delete[] nameData;
}

PFS::~PFS()
{
    if (m_rawData)
        delete[] m_rawData;
}

byte* PFS::getEntry(const std::string& name, uint32_t& len)
{
    if (!m_byName.count(name))
    {
        len = 0;
        return nullptr;
    }
    
    uint32_t i = m_byName[name];
    return decompressEntry(i, len);
}

bool PFS::hasWLD() const
{
    return m_byExtension.count("wld") != 0;
}

bool PFS::hasTER() const
{
    return m_byExtension.count("ter") != 0;
}

bool PFS::hasZON() const
{
    return m_byExtension.count("zon") != 0;
}

byte* PFS::decompressEntry(uint32_t i, uint32_t& len)
{
    if (i >= m_fileEntries.size())
        return nullptr;
    
    ProcessedEntry& ent = m_fileEntries[i];
    
    if (ent.offset > m_rawLen || (ent.offset + ent.deflatedLen) > m_rawLen)
        return nullptr;
    
    byte* src       = m_rawData + ent.offset;
    uint32_t ilen   = ent.inflatedLen;
    uint32_t read   = 0;
    uint32_t pos    = 0;
    byte* buffer    = new (std::nothrow) byte[ilen];
    
    if (!buffer)
        return nullptr;
    
    while (read < ilen)
    {
        BlockHeader* bh = (BlockHeader*)&src[pos];
        pos += sizeof(BlockHeader);
        
        if (!FreeImage_ZLibUncompress(buffer + read, ilen - read, src + pos, bh->deflatedLen))
            goto failure;
        
        read += bh->inflatedLen;
        pos  += bh->deflatedLen;
    }
    
    len = ilen;
    
    return buffer;
failure:
    delete[] buffer;
    return nullptr;
}
