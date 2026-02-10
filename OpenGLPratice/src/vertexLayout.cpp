#include "vertexLayout.h"

VertexLayoutUPtr VertexLayout::Create()
{
    auto vertexLayout{VertexLayoutUPtr(new VertexLayout{})};
    vertexLayout->Init();

    return vertexLayout;
}

VertexLayout::~VertexLayout()
{
    if (vertexArrayObject != 0)
        glDeleteVertexArrays(1, &vertexArrayObject);
}

void VertexLayout::Bind() const
{
    glBindVertexArray(vertexArrayObject);
}

void VertexLayout::SetAttrib(uint32_t attribIndex, int count, uint32_t type, bool normalized, size_t stride, uint64_t offset) const
{
    glEnableVertexAttribArray(attribIndex);
    glVertexAttribPointer(attribIndex, count, type, normalized, stride, reinterpret_cast<const void *>(offset));
}

void VertexLayout::Init()
{
    glGenVertexArrays(1, &vertexArrayObject);
    Bind();
}
