#ifndef CONTEXT_H_
#define CONTEXT_H_

#include "common.h"
#include "shader.h"
#include "program.h"
#include "buffer.h"
#include "vertexLayout.h"
#include "texture.h"

CLASS_PTR(Context);
class Context
{
private:
    ProgramUPtr program;
    VertexLayoutUPtr vertexLayout;
    BufferUPtr vertexBuffer;
    BufferUPtr indexBuffer;
    TextureUPtr texture;
    TextureUPtr texture2;

public:
    static ContextUPtr Create();

    void Render();

private:
    explicit Context() noexcept = default;
    explicit Context(const Context &other) noexcept = delete;
    const Context &operator=(const Context &other) noexcept = delete;

    bool Init();
};

#endif