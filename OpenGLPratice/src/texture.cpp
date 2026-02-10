#include "texture.h"
#include "image.h"

TextureUPtr Texture::CreateFromImage(const Image &image)
{
    auto texture{TextureUPtr{new Texture{}}};
    texture->CreateTexture();
    texture->SetTextureFromImage(image);

    return texture;
}

Texture::~Texture() noexcept
{
    if (texture != 0)
        glDeleteTextures(1, &texture);
}

void Texture::Bind() const noexcept
{
    glBindTexture(GL_TEXTURE_2D, texture);
}

void Texture::SetFilter(uint32_t minFilter, uint32_t magFilter) const noexcept
{
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, minFilter);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, magFilter);
}

void Texture::SetWrap(uint32_t sWrap, uint32_t tWrap) const noexcept
{
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, sWrap);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, tWrap);
}

void Texture::CreateTexture()
{
    glGenTextures(1, &texture);
    Bind();
    SetFilter(GL_LINEAR_MIPMAP_LINEAR, GL_LINEAR);
    SetWrap(GL_CLAMP_TO_EDGE, GL_CLAMP_TO_EDGE);
}

void Texture::SetTextureFromImage(const Image &image)
{
    GLenum format{GL_RGBA};
    switch (image.GetChannelCount())
    {
    case 1:
        format = GL_RED;
        break;
    case 2:
        format = GL_RG;
        break;
    case 3:
        format = GL_RGB;
        break;
    }

    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, image.GetWidth(), image.GetHeight(), 0, format,
                 GL_UNSIGNED_BYTE, image.GetData());

    glGenerateMipmap(GL_TEXTURE_2D);
}
