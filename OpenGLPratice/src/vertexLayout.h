#ifndef VERTEXLAYOUT_H_
#define VERTEXLAYOTU_H_

#include "common.h"

CLASS_PTR(VertexLayout)
class VertexLayout
{
private:
    uint32_t vertexArrayObject{0};

public:
    static VertexLayoutUPtr Create();
    ~VertexLayout() noexcept;

    uint32_t Get() const noexcept { return vertexArrayObject; }
    void Bind() const;
    void SetAttrib(uint32_t attribIndex, int count, uint32_t type, bool normalized, size_t stride, uint64_t offset) const;
    void DisableAttrib(int attribIndex) const;

private:
    explicit VertexLayout() noexcept = default;
    void Init();
};

#endif