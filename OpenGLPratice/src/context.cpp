#include "context.h"

ContextUPtr Context::Create()
{
    auto context{ContextUPtr(new Context{})};
    if (!context->Init())
        return nullptr;

    return std::move(context);
}

void Context::Render()
{
    glClear(GL_COLOR_BUFFER_BIT);
    program->Use();
    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_SHORT, 0);

    // static float time{0.0f};
    // float t{sinf(time) * 0.5f * 0.5f};
    // auto loc{glGetUniformLocation(program->Get(), "color")};
    // program->Use();
    // glUniform4f(loc, t * t, 2.0f * t * (1.0f - t), (1.0f - t) * (1.0f - t), 1.0f);
    // glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_SHORT, 0);

    // time += 0.00016f;
}

bool Context::Init()
{
    std::array<float, 24> vertices{
        0.5f, 0.5f, 0.0f, 1.0f, 0.0f, 0.0f,
        0.5f, -0.5f, 0.0f, 0.0f, 1.0f, 0.0f,
        -0.5f, -0.5f, 0.0f, 0.0f, 0.0f, 1.0f,
        -0.5f, 0.5f, 0.0f, 1.0f, 1.0f, 0.0f};

    std::array<uint16_t, 6> indices{
        0, 1, 3,
        1, 2, 3};

    vertexLayout = VertexLayout::Create();
    vertexBuffer = Buffer::CreateWithData(GL_ARRAY_BUFFER, GL_STATIC_DRAW, vertices.data(), sizeof(float) * vertices.size());

    vertexLayout->SetAttrib(0, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 6, 0);
    vertexLayout->SetAttrib(1, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 6, sizeof(float) * 3);

    indexBuffer = Buffer::CreateWithData(GL_ELEMENT_ARRAY_BUFFER, GL_STATIC_DRAW, indices.data(), sizeof(uint16_t) * indices.size());

    ShaderPtr vertexShader{Shader::CreateFromFile("./OpenGLPratice/shader/perVertexColor.vs", GL_VERTEX_SHADER)};
    ShaderPtr fragmentShader{Shader::CreateFromFile("./OpenGLPratice/shader/perVertexColor.fs", GL_FRAGMENT_SHADER)};

    if (!vertexShader || !fragmentShader)
        return false;

    SPDLOG_INFO("vertex shader id: {}", vertexShader->Get());
    SPDLOG_INFO("fragment shader id: {}", fragmentShader->Get());

    program = Program::Create({fragmentShader, vertexShader});
    if (program == 0)
        return false;

    SPDLOG_INFO("program id: {}", program->Get());

    glClearColor(0.0f, 0.1f, 0.2f, 0.0f);

    return true;
}
