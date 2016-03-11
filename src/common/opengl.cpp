
#include "opengl.hpp"

OpenGL gOpenGL;

void OpenGL::loadExtensions()
{
    if (gOpenGL.extensionsLoaded)
        return;
    
    gOpenGL.glGenerateMipmap = (GenerateMipmap)sf::Context::getFunction("glGenerateMipmap");
    
    gOpenGL.glGenBuffers    = (GenBuffers)sf::Context::getFunction("glGenBuffers");
    gOpenGL.glBindBuffer    = (BindBuffer)sf::Context::getFunction("glBindBuffer");
    gOpenGL.glBufferData    = (BufferData)sf::Context::getFunction("glBufferData");
    gOpenGL.glDeleteBuffers = (DeleteBuffers)sf::Context::getFunction("glDeleteBuffers");
    
    glFrontFace(GL_CCW);
    glClearColor(0.25f, 0.25f, 0.25f, 1.0f);
    
    gOpenGL.extensionsLoaded = true;
}

bool OpenGL::mipmapFlag()
{
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAX_ANISOTROPY, 2.0f);
    
    if (!glGetError())
    {
        glHint(GL_GENERATE_MIPMAP_HINT, GL_NICEST);
        glTexParameteri(GL_TEXTURE_2D, GL_GENERATE_MIPMAP, GL_TRUE);
        return !glGetError();
    }
    
    return false;
}

uint32_t OpenGL::loadTexture(byte* data, int width, int height)
{    
    uint32_t id;
    glGenTextures(1, &id);
    
    if (id == 0)
    {
        //throw
    }
    
    glBindTexture(GL_TEXTURE_2D, id);
    
    bool mipmapFlagged = mipmapFlag();
    
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, (uint32_t)width, (uint32_t)height, 0, GL_INTERNAL_PIXEL_FORMAT, GL_UNSIGNED_BYTE, data);//bitmap);
    
    if (!mipmapFlagged)
    {
        if (gOpenGL.glGenerateMipmap)
        {
            gOpenGL.glGenerateMipmap(GL_TEXTURE_2D);
            mipmapFlagged = !glGetError();
        }
    }
    
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, mipmapFlagged ? GL_LINEAR_MIPMAP_LINEAR : GL_LINEAR);
    
    return id;
}

uint32_t OpenGL::generateVBO(void* data, uint32_t len, bool isDynamic)
{
    if (!gOpenGL.glGenBuffers)
        return 0;
    
    uint32_t vbo;
    
    gOpenGL.glGenBuffers(1, &vbo);
    gOpenGL.glBindBuffer(GL_ARRAY_BUFFER, vbo);
    gOpenGL.glBufferData(GL_ARRAY_BUFFER, len, data, isDynamic ? GL_DYNAMIC_DRAW : GL_STATIC_DRAW);
    
    return vbo;
}

void OpenGL::bindVBO(uint32_t vbo)
{
    gOpenGL.glBindBuffer(GL_ARRAY_BUFFER, vbo);
}

void OpenGL::deleteVBO(uint32_t vbo)
{
    gOpenGL.glDeleteBuffers(1, &vbo);
}
