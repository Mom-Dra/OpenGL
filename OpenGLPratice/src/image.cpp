#include "image.h"
#define STB_IMAGE_IMPLEMENTATION
#include <stb/stb_image.h>

ImageUPtr Image::Load(std::string_view filePath)
{
    auto image{ImageUPtr{new Image{}}};
    if (!image->LoadWithStb(filePath))
        return nullptr;

    return image;
}

ImageUPtr Image::Create(int width, int height, int channelCount)
{
    auto image{ImageUPtr{new Image{}}};
    if (!image->Allocate(width, height, channelCount))
        return nullptr;

    return image;
}

Image::~Image() noexcept
{
    stbi_image_free(data);
}

void Image::SetCheckImage(int gridX, int gridY)
{
    for (int j{0}; j < height; ++j)
    {
        for (int i{0}; i < width; ++i)
        {
            int pos{(j * width + i) * channelCount};
            bool even{((i / gridX) + (j / gridY)) % 2 == 0};
            uint8_t value{static_cast<uint8_t>(even ? 255 : 0)};

            std::fill_n(data + pos, channelCount, value);

            if (channelCount > 3)
                data[pos + 3] = 255;
        }
    }
}

bool Image::LoadWithStb(std::string_view filePath)
{
    stbi_set_flip_vertically_on_load(true);
    data = stbi_load(filePath.data(), &width, &height, &channelCount, 0);

    if (!data)
    {
        SPDLOG_ERROR("failed to load image: {}", filePath);
        return false;
    }

    return true;
}

bool Image::Allocate(int width, int height, int channelCount)
{
    this->width = width;
    this->height = height;
    this->channelCount = channelCount;
    data = new uint8_t[width * height * channelCount];

    return data ? true : false;
}
