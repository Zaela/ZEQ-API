
#ifndef _ZEQ_WLD_HPP_
#define _ZEQ_WLD_HPP_

#include "define.hpp"
#include "fragments.hpp"
#include "model_listing.hpp"
#include "pfs.hpp"
#include <vector>
#include <unordered_map>

class WLD
{
private:
    struct Header
    {
        uint32_t signature;
        uint32_t version;
        uint32_t fragCount;
        uint32_t unknownA[2];
        uint32_t stringBlockLen;
        uint32_t unknownB;
    };
    
private:
    byte*       m_rawData;
    uint32_t    m_rawLen;
    char*       m_stringBlock;
    uint32_t    m_stringBlockLen;
    uint32_t    m_version;

    static const uint32_t VERSION1  = 0x00015500;
    static const uint32_t VERSION2  = 0x1000C800;
    static const uint32_t SIGNATURE = 0x54503d02;

    std::vector<Fragment*>                                  m_fragsByIndex;
    std::unordered_map<uint32_t, std::vector<Fragment*>>    m_fragsByType;
    std::unordered_map<int, Fragment*>                      m_fragsByNameIndex;
    
public:
    WLD(byte* data, uint32_t len);
    ~WLD();

    static void decodeString(char* str, uint32_t len);

    uint32_t    version() const { return m_version; }
    const char* stringBlock() const { return m_stringBlock; }
    uint32_t    stringBlockLen() const { return m_stringBlockLen; }
    
    Fragment*   getFrag(int ref);
    const char* getFragName(Fragment* frag);
    const char* getFragName(int nameRef);
    
    bool isZone();
    
    void listModels(std::vector<ModelListing>& list, zeq_model_type_t expectedType);
    
    std::vector<Fragment*>& getFragsByType(uint32_t type) { return m_fragsByType[type]; }
};

#endif//_ZEQ_WLD_HPP_
