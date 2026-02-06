#ifndef BUFFER_H_
#define BUFFER_H_

#include "common.h"

CLASS_PTR(Buffer);
class Buffer
{
private:
    uint32_t buffer{0};
    uint32_t bufferType{0};
    uint32_t usage{0};

public:
    static BufferUPtr CreateWithData(uint32_t bufferType, uint32_t usage,
                                     const void *data, size_t dataSize);

    ~Buffer() noexcept;

    uint32_t Get() const noexcept { return buffer; }
    void Bind() const;

private:
    explicit Buffer() noexcept = default;
    explicit Buffer(const Buffer &other) noexcept = delete;
    const Buffer &operator=(const Buffer &other) noexcept = delete;

    bool Init(uint32_t bufferType, uint32_t usage, const void *data, size_t dataSize);
};

#endif