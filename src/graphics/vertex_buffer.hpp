
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
    VertexBuffer()                          = delete;
    VertexBuffer(const VertexBuffer& vb)    = delete;

    static VertexBuffer* create(ConvVertexBuffer& cVB);
    static void destroy(VertexBuffer* vb); // Valgrind does not like operator delete overload for this class...
    virtual ~VertexBuffer();

    void draw();
    void registerWithOpenGL(bool useVBO = false, bool isDynamic = false);
};

#endif//_ZEQ_VERTEX_BUFFER_HPP_
