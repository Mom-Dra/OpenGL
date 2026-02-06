#include "context.h"
#include "image.h"

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
    std::array<float, 32> vertices{
        0.5f, 0.5f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f, 1.0f,
        0.5f, -0.5f, 0.0f, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f,
        -0.5f, -0.5f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f,
        -0.5f, 0.5f, 0.0f, 1.0f, 1.0f, 0.0f, 0.0f, 1.0f};

    std::array<uint16_t, 6> indices{
        0, 1, 3,
        1, 2, 3};

    vertexLayout = VertexLayout::Create();
    vertexBuffer = Buffer::CreateWithData(GL_ARRAY_BUFFER, GL_STATIC_DRAW, vertices.data(), sizeof(float) * vertices.size());

    vertexLayout->SetAttrib(0, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 8, 0);
    vertexLayout->SetAttrib(1, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 8, sizeof(float) * 3);
    vertexLayout->SetAttrib(2, 2, GL_FLOAT, GL_FALSE, sizeof(float) * 8, sizeof(float) * 6);

    indexBuffer = Buffer::CreateWithData(GL_ELEMENT_ARRAY_BUFFER, GL_STATIC_DRAW, indices.data(), sizeof(uint16_t) * indices.size());

    ShaderPtr vertexShader{Shader::CreateFromFile("./OpenGLPratice/shader/texture.vs", GL_VERTEX_SHADER)};
    ShaderPtr fragmentShader{Shader::CreateFromFile("./OpenGLPratice/shader/texture.fs", GL_FRAGMENT_SHADER)};

    if (!vertexShader || !fragmentShader)
        return false;

    SPDLOG_INFO("vertex shader id: {}", vertexShader->Get());
    SPDLOG_INFO("fragment shader id: {}", fragmentShader->Get());

    program = Program::Create({fragmentShader, vertexShader});
    if (program == 0)
        return false;

    SPDLOG_INFO("program id: {}", program->Get());

    glClearColor(0.0f, 0.1f, 0.2f, 0.0f);

    auto image{Image::Load("./OpenGLPratice/texture/container.jpg")};
    if (!image)
        return false;
    SPDLOG_INFO("image: {}x{}, {} channels", image->GetWidth(), image->GetHeight(), image->GetChannelCount());

    glGenTextures(1, &texture);
    glBindTexture(GL_TEXTURE_2D, texture);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, image->GetWidth(), image->GetHeight(), 0, GL_RGB, GL_UNSIGNED_BYTE,
                 image->GetData());

    return true;
}
