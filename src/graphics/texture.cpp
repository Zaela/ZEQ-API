
#include "texture.hpp"

Texture::Texture()
: m_name(std::string()),
  m_id(0),
  m_ptr(nullptr)
{
    
}

Texture::Texture(const std::string& name, byte* data, uint32_t len, bool masked)
: m_name(name),
  m_id(0),
  m_ptr(nullptr)
{
    toBitmap(data, len, masked);
    delete[] data;
}

Texture::Texture(Texture&& old)
{
    *this = std::move(old);
}

Texture::~Texture()
{
    if (m_id)
        glDeleteTextures(1, &m_id);
    
    if (m_ptr)
        FreeImage_Unload(m_ptr);
}

Texture& Texture::operator=(Texture&& old)
{
    m_name      = std::move(old.m_name);
    m_id        = old.m_id;
    m_ptr       = old.m_ptr;
    
    old.m_id    = 0;
    old.m_ptr   = nullptr;
    
    return *this;
}

void Texture::toBitmap(byte* data, uint32_t len, bool masked)
{
    FIMEMORY* mem           = FreeImage_OpenMemory(data, len);
    FREE_IMAGE_FORMAT fmt   = FreeImage_GetFileTypeFromMemory(mem, 0);
    FIBITMAP* ptr           = FreeImage_LoadFromMemory(fmt, mem, 0);
    
    FreeImage_CloseMemory(mem);
    
    if (!ptr) throw 1; //fixme
    
    if (masked && fmt == FIF_BMP)
    {
        toMaskedBitmap(ptr);
        return;
    }
    
    if (fmt != FIF_DDS)
        FreeImage_FlipVertical(ptr);
    
    if (32 != FreeImage_GetBPP(ptr))
    {
        FIBITMAP* copy = FreeImage_ConvertTo32Bits(ptr);
        FreeImage_Unload(ptr);
        ptr = copy;
    }
    
    m_ptr = ptr;
}

void Texture::toMaskedBitmap(FIBITMAP* ptr)
{
    byte* indices       = FreeImage_GetBits(ptr);
    RGBQUAD* palette    = FreeImage_GetPalette(ptr);
    
    uint32_t width  = FreeImage_GetWidth(ptr);
    uint32_t height = FreeImage_GetHeight(ptr);
    
    FIBITMAP* out = FreeImage_Allocate((int)width, (int)height, 32,
        FI_RGBA_RED_MASK, FI_RGBA_GREEN_MASK, FI_RGBA_BLUE_MASK);
    
    if (!out) throw 1; //fixme
    
    byte* o = FreeImage_GetBits(out);
    
    // Assuming first color in the palette is the mask color
    uint32_t n = width * height;
    for (uint32_t i = 0; i < n; i++)
    {
        byte index = indices[i];
        
        if (index == 0)
        {
            o[FI_RGBA_BLUE]     = 0;
            o[FI_RGBA_GREEN]    = 0;
            o[FI_RGBA_RED]      = 0;
            o[FI_RGBA_ALPHA]    = 0;
        }
        else
        {
            RGBQUAD p           = palette[index];
            o[FI_RGBA_BLUE]     = p.rgbBlue;
            o[FI_RGBA_GREEN]    = p.rgbGreen;
            o[FI_RGBA_RED]      = p.rgbRed;
            o[FI_RGBA_ALPHA]    = 255;
        }
        
        o += 4;
    }
    
    FreeImage_Unload(ptr);
    FreeImage_FlipVertical(out);
    
    m_ptr = out;
}

void Texture::registerWithOpenGL()
{
    if (!m_ptr)
        return;
    
    byte* data  = (byte*)FreeImage_GetBits(m_ptr);
    int width   = (int)FreeImage_GetWidth(m_ptr);
    int height  = (int)FreeImage_GetHeight(m_ptr);
    
    m_id = OpenGL::loadTexture(data, width, height);
    
    if (m_id)
    {
        FreeImage_Unload(m_ptr);
        m_ptr = nullptr;
    }
}
