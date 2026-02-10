#ifndef TEXTURE_H_
#define TEXTURE_H_

#include "common.h"

class Image;

CLASS_PTR(Texture);
class Texture
{
private:
    uint32_t texture{0};

public:
    static TextureUPtr CreateFromImage(const Image &image);

    ~Texture() noexcept;

    uint32_t Get() const noexcept { return texture; }
    void Bind() const noexcept;
    void SetFilter(uint32_t minFilter, uint32_t magFilter) const noexcept;
    void SetWrap(uint32_t sWrap, uint32_t tWrap) const noexcept;

private:
    explicit Texture() noexcept = default;
    explicit Texture(const Texture &other) noexcept = delete;
    const Texture &operator=(const Texture &other) noexcept = delete;

    void CreateTexture();
    void SetTextureFromImage(const Image &image);
};

#endif
