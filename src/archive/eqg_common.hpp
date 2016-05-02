
#ifndef _ZEQ_EQG_COMMON_HPP_
#define _ZEQ_EQG_COMMON_HPP_

#include "define.hpp"

class EqgCommon
{
private:
    byte*           m_data;
    uint32_t        m_len;
    uint32_t        m_version;
    uint32_t        m_stringBlockLen;
    const char*     m_stringBlock;

private:
    void setStringBlockLength(uint32_t len) { m_stringBlockLen = len; }
    void setStringBlock(const char* strings) { m_stringBlock = strings; }
    
protected:
    void setVersion(uint32_t ver) { m_version = ver; }
    
    inline void checkLength(uint32_t p)
    {
        if (p > m_len)
            throw 1; //fixme
    }
    
    uint32_t extractStrings(uint32_t p, uint32_t length);
    
public:
    EqgCommon(byte* data, uint32_t len) : m_data(data), m_len(len), m_version(0), m_stringBlockLen(0), m_stringBlock(nullptr) { }
    
    byte*       getData() const { return m_data; }
    uint32_t    length() const { return m_len; }
    uint32_t    version() const { return m_version; }
    uint32_t    stringsLength() const { return m_stringBlockLen; }
    const char* strings() const { return m_stringBlock; }
    
    const char* getString(uint32_t index);
};

#endif//_ZEQ_EQG_COMMON_HPP_
