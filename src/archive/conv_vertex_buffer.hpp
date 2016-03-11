
#ifndef _ZEQ_CONV_VERTEX_BUFFER_HPP_
#define _ZEQ_CONV_VERTEX_BUFFER_HPP_

#include "define.hpp"
#include "vertex.hpp"
#include "conv_material.hpp"
#include <vector>

class ConvVertexBuffer
{
private:
    ConvMaterial*   m_material;
    Vertex*         m_vertices;
    uint32_t        m_count;
    uint32_t        m_capacity;

    static const uint32_t DEFAULT_CAPACITY = 32;

private:
    void initBuffer();
    void growBuffer();

public:
    class Iterator
    {
    private:
        Vertex*  m_ptr;
    
    public:
        Iterator(Vertex* v) : m_ptr(v) { }
        Iterator& operator++() { m_ptr++; return *this; }
        bool operator==(const Iterator& o) { return m_ptr == o.m_ptr; }
        bool operator!=(const Iterator& o) { return m_ptr != o.m_ptr; }
        Vertex& operator*() { return *m_ptr; }
    };

public:
    ConvVertexBuffer();
    ConvVertexBuffer(ConvMaterial* mat);
    ConvVertexBuffer(ConvVertexBuffer&& old);
    ~ConvVertexBuffer();
    
    ConvVertexBuffer& operator=(ConvVertexBuffer&& old);
    
    Iterator    begin() { return Iterator(m_vertices); }
    Iterator    end() { return Iterator(m_vertices + m_count); }
    void        push_back(Vertex& v);
    uint32_t    size() const { return m_count; }
    bool        empty() const { return m_count == 0; }
    void        reserve(uint32_t n);
    Vertex*     data() const { return m_vertices; }
    void        clear();
    
    uint32_t    triangleCount() { return m_count / 3; }

    ConvMaterial* getMaterial() const { return m_material; }
    void setMaterial(ConvMaterial* mat) { m_material = mat; }
};

#endif//_ZEQ_CONV_VERTEX_BUFFER_HPP_
