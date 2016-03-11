
#include "wld.hpp"

WLD::WLD(byte* data, uint32_t len)
: m_rawData(data),
  m_rawLen(len)
{
    Header* h = (Header*)data;
    
    uint32_t p = sizeof(Header);
    
    if (p > len)
        throw 1; //fixme
    
    if (h->signature != SIGNATURE)
        throw 1; //fixme
    
    m_version = h->version & 0xfffffffe;
    if (m_version != VERSION1 && m_version != VERSION2)
        throw 1; //fixme
    
    m_version           = (m_version == VERSION1) ? 1 : 2;
    m_stringBlock       = (char*)&data[p];
    m_stringBlockLen    = h->stringBlockLen;
    uint32_t n          = h->fragCount;
    
    p += m_stringBlockLen;
    
    if (p > len)
        throw 1; //fixme
    
    decodeString(m_stringBlock, m_stringBlockLen);
    
    int minRef = -((int)m_stringBlockLen);
    // Zeroth ref is null -- saves us a lot of off-by-one corrections later
    m_fragsByIndex.push_back(nullptr);
    
    for (uint32_t i = 0; i < n; i++)
    {
        Fragment* frag = (Fragment*)&data[p];
        
        if (p > len)
            throw 1; //fixme
        
        m_fragsByIndex.push_back(frag);
        m_fragsByType[frag->type()].push_back(frag);
        
        int nameRef = frag->nameRef();
        if (nameRef < 0 && nameRef > minRef)
            m_fragsByNameIndex[nameRef] = frag;
        
        p += frag->length();
    }
    
    // Decode strings for any 0x03 fragments
    if (m_fragsByType.count(0x03))
    {
        for (Fragment* frag : m_fragsByType[0x03])
        {
            Frag03* f03 = (Frag03*)frag;
            
            if (f03->stringLen > 0)
                decodeString(f03->string, f03->stringLen);
        }
    }
}

WLD::~WLD()
{
    if (m_rawData)
        delete[] m_rawData;
}

void WLD::decodeString(char* str, uint32_t len)
{
    static const byte hash[8] = {0x95, 0x3A, 0xC5, 0x2A, 0x95, 0x7A, 0x95, 0x6A};
    
    for (uint32_t i = 0; i < len; i++)
    {
        str[i] ^= hash[i & 7];
        str[i] = std::tolower(str[i]);
    }
}

Fragment* WLD::getFrag(int ref)
{
    if (ref > 0)
    {
        if (m_fragsByIndex.size() > (uint32_t)ref)
            return m_fragsByIndex[ref];
        return nullptr;
    }
    
    if (ref == 0) ref = -1;
    
    if (m_fragsByNameIndex.count(ref))
        return m_fragsByNameIndex[ref];
    
    return nullptr;
}

const char* WLD::getFragName(Fragment* frag)
{
    return getFragName(frag->nameRef());
}

const char* WLD::getFragName(int nameRef)
{
    if (nameRef < 0 && nameRef > -((int)m_stringBlockLen))
        return m_stringBlock - nameRef;
    return nullptr;
}

bool WLD::isZone()
{
    if (m_fragsByType.count(0x14) && m_fragsByType[0x14].size() == 1)
        return strcmp(getFragName(m_fragsByType[0x14][0]), "player_1") == 0;
    return false;
}

void WLD::listModels(std::vector<ModelListing>& list, zeq_model_type_t expectedType)
{
    if (!m_fragsByType.count(0x14))
        return;
    
    for (Fragment* f14 : m_fragsByType[0x14])
    {
        const char* name        = getFragName(f14);
        zeq_model_type_t type   = expectedType;
        
        if (type == ZEQ_MODEL_ITEM && (name[0] != 'i' || name[1] != 't' || !(name[2] >= '0' && name[2] <= '9')))
            type = ZEQ_MODEL_OBJECT;
        
        list.push_back(ModelListing(name, type));
    }
}
