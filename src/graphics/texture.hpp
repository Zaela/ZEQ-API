
#ifndef _ZEQ_TEXTURE_HPP_
#define _ZEQ_TEXTURE_HPP_

#include "define.hpp"
#include "ref_counter.hpp"
#include "opengl.hpp"
#include <FreeImage.h>
#include <string>

class Texture : public RefCounter
{
private:
    std::string m_name;
    uint32_t    m_id;
    FIBITMAP*   m_ptr;

private:
    void toBitmap(byte* data, uint32_t len, bool masked);
    void toMaskedBitmap(FIBITMAP* ptr);

public:
    Texture();
    Texture(const std::string& name, byte* data, uint32_t len, bool masked);
    Texture(Texture&& old);
    ~Texture();

    Texture& operator=(Texture&& old);

    uint32_t getDiffuseId() const { return m_id; }
    
    void registerWithOpenGL();
};

#endif//_ZEQ_TEXTURE_HPP_
