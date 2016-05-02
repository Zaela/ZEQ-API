
#ifndef _ZEQ_VERTEX_BUFFER_HPP_
#define _ZEQ_VERTEX_BUFFER_HPP_

#include "define.hpp"
#include "opengl.hpp"
#include "vertex.hpp"
#include "material.hpp"
#include "conv_vertex_buffer.hpp"
#include "conv_material.hpp"
#include "opengl.hpp"

class VertexBuffer : public zeq_material_t
{
private:
    uint32_t    m_count;
    uint32_t    m_vboId;

    // Must be the last field in this class!
    Vertex      m_vertices[1]; // Variable-length stub

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
    VertexBuffer()                          = delete;
    VertexBuffer(const VertexBuffer& vb)    = delete;

    static VertexBuffer* create(ConvVertexBuffer& cVB);
    static VertexBuffer* copy(const VertexBuffer& vb);
    static void destroy(VertexBuffer* vb); // Valgrind does not like operator delete overload for this class...
    virtual ~VertexBuffer();

    uint32_t count() const { return m_count; }
    Vertex* vertices() { return m_vertices; }

    void draw();
    void registerWithOpenGL(bool useVBO = false, bool isDynamic = false);

    Iterator begin() { return Iterator(m_vertices); }
    Iterator end()   { return Iterator(&m_vertices[m_count]); }
};

#endif//_ZEQ_VERTEX_BUFFER_HPP_
