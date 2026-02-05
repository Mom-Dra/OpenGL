#include "buffer.h"

BufferUPtr Buffer::CreateWithData(uint32_t bufferType, uint32_t usage, const void *data, size_t dataSize)
{
    auto buffer{BufferUPtr(new Buffer{})};
    if (!buffer->Init(bufferType, usage, data, dataSize))
        return nullptr;

    return std::move(buffer);
}

Buffer::~Buffer()
{
    if (buffer)
        glDeleteBuffers(1, &buffer);
}

void Buffer::Bind() const
{
    glBindBuffer(bufferType, buffer);
}

bool Buffer::Init(uint32_t bufferType, uint32_t usage, const void *data, size_t dataSize)
{
    this->bufferType = bufferType;
    this->usage = usage;
    glGenBuffers(1, &buffer);
    Bind();
    glBufferData(bufferType, dataSize, data, usage);

    return true;
}