
#include "conv_vertex_buffer.hpp"

ConvVertexBuffer::ConvVertexBuffer()
: m_material(nullptr),
  m_vertices(nullptr),
  m_count(0),
  m_capacity(0)
{
    
}

ConvVertexBuffer::ConvVertexBuffer(ConvMaterial* mat)
: m_material(mat),
  m_vertices(nullptr),
  m_count(0),
  m_capacity(0)
{
    
}

ConvVertexBuffer::ConvVertexBuffer(ConvVertexBuffer&& old)
{
    *this = std::move(old);
}

ConvVertexBuffer::~ConvVertexBuffer()
{
    if (m_vertices)
        delete[] m_vertices;
}

ConvVertexBuffer& ConvVertexBuffer::operator=(ConvVertexBuffer&& old)
{
    m_material  = old.m_material;
    m_vertices  = old.m_vertices;
    m_count     = old.m_count;
    m_capacity  = old.m_capacity;
    
    old.m_vertices  = nullptr;
    old.m_count     = 0;
    old.m_capacity  = 0;
    
    return *this;
}

void ConvVertexBuffer::initBuffer()
{
    m_vertices = new Vertex[DEFAULT_CAPACITY];
    m_capacity = DEFAULT_CAPACITY;
}

void ConvVertexBuffer::growBuffer()
{
    uint32_t cap    = m_capacity * 2;
    Vertex* array   = new Vertex[cap];
    
    memcpy(array, m_vertices, m_capacity * sizeof(Vertex));
    
    delete[] m_vertices;
    
    m_vertices = array;
    m_capacity = cap;
}

void ConvVertexBuffer::push_back(Vertex& v)
{
    uint32_t index = m_count++;
    
    if (!m_vertices)
        initBuffer();
    else if (index >= m_capacity)
        growBuffer();
    
    m_vertices[index] = v;
}

void ConvVertexBuffer::reserve(uint32_t n)
{
    if (m_capacity >= n)
        return;
    
    Vertex* array = new Vertex[n];
    
    if (m_vertices)
    {
        if (m_count)
            memcpy(array, m_vertices, m_count);
        delete[] m_vertices;
    }
    
    m_vertices = array;
    m_capacity = n;
}

void ConvVertexBuffer::clear()
{
    if (!m_vertices)
        return;
    
    delete[] m_vertices;
    m_vertices  = nullptr;
    m_count     = 0;
    m_capacity  = 0;
}
