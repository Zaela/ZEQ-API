
#include "eqg_weight_buffer.hpp"

void EqgWeightBuffer::add(uint32_t vertIndex, uint32_t boneIndex, float weight)
{
    Weight wt;
    wt.vertIndex    = vertIndex;
    wt.boneIndex    = boneIndex;
    wt.weight       = weight;
    
    m_weights.push_back(wt);
}

void EqgWeightBuffer::sort()
{
    std::sort(m_weights.begin(), m_weights.end(), [](const Weight& a, const Weight& b) { return a.vertIndex < b.vertIndex; });
}
