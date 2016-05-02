
#include "eqg_common.hpp"

uint32_t EqgCommon::extractStrings(uint32_t p, uint32_t length)
{
    byte* data      = m_data;
    uint32_t end    = p + length;
    
    checkLength(end);
    
    setStringBlockLength(length);
    setStringBlock((const char*)data + p);
    
    for (uint32_t i = p; i < end; i++)
    {
        data[i] = std::tolower(data[i]);
    }
    
    return end;
}

const char* EqgCommon::getString(uint32_t index)
{
    if (index >= m_stringBlockLen)
    return nullptr;

    return m_stringBlock + index;
}
