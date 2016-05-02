
#include "vertex_buffer.hpp"

VertexBuffer* VertexBuffer::create(ConvVertexBuffer& cVB)
{
    size_t vlen = cVB.size() * sizeof(Vertex);
    size_t len  = sizeof(VertexBuffer) - sizeof(Vertex) + vlen;
    
    VertexBuffer* vb = (VertexBuffer*)(new byte[len]);
    
    new (vb) zeq_material_t(*cVB.getMaterial());
    
    vb->m_count = cVB.size();
    vb->m_vboId = 0;
    
    memcpy(vb->m_vertices, cVB.data(), vlen);
    
    return vb;
}

VertexBuffer* VertexBuffer::copy(const VertexBuffer& src)
{
    size_t vlen = src.m_count * sizeof(Vertex);
    size_t len  = sizeof(VertexBuffer) - sizeof(Vertex) + vlen;
    
    VertexBuffer* vb = (VertexBuffer*)(new byte[len]);
    
    new (vb) zeq_material_t(src);
    
    vb->m_count = src.m_count;
    vb->m_vboId = 0;
    
    memcpy(vb->m_vertices, src.m_vertices, vlen);
    
    return vb;
}

VertexBuffer::~VertexBuffer()
{
    if (m_vboId)
        OpenGL::deleteVBO(m_vboId);
}

void VertexBuffer::destroy(VertexBuffer* vb)
{
    vb->~VertexBuffer();
    delete[] ((byte*)vb);
}

void VertexBuffer::draw()
{
    byte* ptr;
    
    if (m_vboId)
    {
        OpenGL::bindVBO(m_vboId);
        ptr = nullptr;
    }
    else
    {
        OpenGL::bindVBO(0);
        ptr = (byte*)m_vertices;
    }
    
    glVertexPointer(3, GL_FLOAT,    sizeof(Vertex), ptr);
    glNormalPointer(GL_FLOAT,       sizeof(Vertex), ptr + 3 * sizeof(float));
    glTexCoordPointer(2, GL_FLOAT,  sizeof(Vertex), ptr + 6 * sizeof(float));
    
    glDrawArrays(GL_TRIANGLES, 0, m_count);
    assert(!glGetError());
}

void VertexBuffer::registerWithOpenGL(bool useVBO, bool isDynamic)
{
    zeq_material_t::registerWithOpenGL();
    
    if (useVBO)
        m_vboId = OpenGL::generateVBO(m_vertices, sizeof(Vertex) * m_count, isDynamic);
}
