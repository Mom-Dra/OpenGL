#ifndef IMAGE_H_
#define IMAGE_H_

#include "common.h"

CLASS_PTR(Image)
class Image
{
private:
    int width{0};
    int height{0};
    int channelCount{0};
    uint8_t *data{nullptr};

public:
    static ImageUPtr Load(std::string_view filePath);

    ~Image() noexcept;

    const uint8_t *GetData() const noexcept { return data; }
    int GetWidth() const noexcept { return width; }
    int GetHeight() const noexcept { return height; }
    int GetChannelCount() const noexcept { return channelCount; }

private:
    explicit Image() noexcept = default;
    explicit Image(const Image &other) noexcept = delete;
    const Image &operator=(const Image &other) noexcept = delete;

    bool LoadWithStb(std::string_view filePath);
};

#endif