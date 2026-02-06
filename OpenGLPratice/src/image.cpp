#include "image.h"
#define STB_IMAGE_IMPLEMENTATION
#include <stb/stb_image.h>

ImageUPtr Image::Load(std::string_view filePath)
{
    auto image{ImageUPtr{new Image{}}};
    if (!image->LoadWithStb(filePath))
        return nullptr;

    return std::move(image);
}

Image::~Image() noexcept
{
    stbi_image_free(data);
}

bool Image::LoadWithStb(std::string_view filePath)
{
    data = stbi_load(filePath.data(), &width, &height, &channelCount, 0);

    if (!data)
    {
        SPDLOG_ERROR("failed to load image: {}", filePath);
        return false;
    }

    return true;
}
