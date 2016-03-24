
#include "eqg_model.hpp"

EqgModel::EqgModel(PFS* pfs, byte* data, uint32_t len)
: ConvModel(pfs),
  m_data(data),
  m_len(len),
  m_version(0)
{
    m_propertyHandlers["e_texturediffuse0"] = handleDiffuse;
}

void EqgModel::loadCommon()
{
    
}

void EqgModel::loadMDS()
{
    MdsHeader* h    = (MdsHeader*)m_data;
    uint32_t p      = sizeof(MdsHeader);
    
    checkLength(p);
    
    setVersion(h->version);
    setStringBlockLength(h->stringBlockLen);
    
    p = extractStrings(p);
    p = extractMaterials(p);
}

uint32_t EqgModel::extractStrings(uint32_t p)
{
    byte* data      = m_data;
    uint32_t end    = p + m_stringBlockLen;
    
    checkLength(end);
    setStringBlock((const char*)data + p);
    
    for (uint32_t i = p; i < end; i++)
    {
        data[i] = std::tolower(data[i]);
    }
    
    return end;
}

uint32_t EqgModel::extractMaterials(uint32_t p)
{
    return p;
}

const char* EqgModel::getString(uint32_t index)
{
    if (index >= m_stringBlockLen)
        return nullptr;
    
    return m_stringBlock + index;
}

void EqgModel::readProperty(EqgMaterial* mat, EqgProperty& prop)
{
    const char* name = getString(prop.typeNameIndex);
    
    if (!name || !m_propertyHandlers.count(name))
        return;
    
    m_propertyHandlers[name](mat, prop, this);
}

void EqgModel::handleDiffuse(EqgMaterial* mat, EqgProperty& prop, EqgModel* model)
{
    
}
