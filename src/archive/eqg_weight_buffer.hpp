
#ifndef _ZEQ_EQG_WEIGHT_BUFFER_HPP_
#define _ZEQ_EQG_WEIGHT_BUFFER_HPP_

#include "define.hpp"
#include "conv_vertex_buffer.hpp"
#include <vector>
#include <algorithm>

class EqgWeightBuffer
{
private:
    struct Weight
    {
        uint32_t    vertIndex;
        uint32_t    boneIndex;
        float       weight;
    };
    
private:
    std::vector<Weight> m_weights;
    ConvVertexBuffer*   m_vertexBuffer;
    
public:
    EqgWeightBuffer() : m_vertexBuffer(nullptr) { }
    
    void add(uint32_t vertIndex, uint32_t boneIndex, float weight);
    void sort();
    
    void setVertexBuffer(ConvVertexBuffer& vb) { m_vertexBuffer = &vb; }
    ConvVertexBuffer* getVertexBuffer() const { return m_vertexBuffer; }
};

#endif//_ZEQ_EQG_WEIGHT_BUFFER_HPP_
