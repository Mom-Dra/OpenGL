#include "context.h"
#include "image.h"
#include <imgui.h>

ContextUPtr Context::Create()
{
    auto context{ContextUPtr{new Context{}}};
    if (!context->Init())
        return nullptr;

    return context;
}

void Context::Render()
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glEnable(GL_DEPTH_TEST);

    if (ImGui::Begin("setting"))
    {
        if (ImGui::ColorEdit4("clear color", glm::value_ptr(clearColor)))
            glClearColor(clearColor.r, clearColor.g, clearColor.b, clearColor.a);

        ImGui::Separator();
        ImGui::DragFloat3("camera pos", glm::value_ptr(cameraPos), 0.01f);
        ImGui::DragFloat("camera yaw", &cameraYaw, 0.5f);
        ImGui::DragFloat("camera pitch", &cameraPitch, 0.5f, -89.0f, 89.0f);
        ImGui::Separator();
        if (ImGui::Button("reset camera"))
        {
            cameraYaw = 0.0f;
            cameraPitch = 0.0f;
            cameraPos = glm::vec3{0.0f, 0.0f, 3.0f};
        }
    }
    ImGui::End();

    std::vector<glm::vec3> cubePositions{
        glm::vec3{0.0f, 0.0f, 0.0f},
        glm::vec3{-1.5f, -2.2f, -2.5f},
        glm::vec3{1.5f, 2.2f, -2.5f},
        glm::vec3{-1.5f, 2.2f, -2.5f},
        glm::vec3{1.5f, -2.2f, -5.0f},
    };

    // float angle{static_cast<float>(glfwGetTime()) * glm::pi<float>() * 0.5f};
    // auto x{sinf(angle) * 10.0f};
    // auto z{cosf(angle) * 10.0f};

    // auto cameraPos{glm::vec3{x, 0.0f, z}};
    // auto cameraTarget{glm::vec3{0.0f, 0.0f, 0.0f}};
    // auto cameraUp{glm::vec3{0.0f, 1.0f, 0.0f}};

    cameraFront = glm::rotate(glm::mat4{1.0f}, glm::radians(cameraYaw), glm::vec3(0.0f, 1.0f, 0.0f)) *
                  glm::rotate(glm::mat4{1.0f}, glm::radians(cameraPitch), glm::vec3(1.0f, 0.0f, 0.0f)) *
                  glm::vec4{0.0f, 0.0f, -1.0f, 0.0f};

    auto view{glm::lookAt(cameraPos, cameraPos + cameraFront, cameraUp)};

    auto projection{glm::perspective(glm::radians(45.0f), static_cast<float>(width) / height, 0.01f, 100.0f)};

    for (size_t i{0}; i < cubePositions.size(); ++i)
    {
        const auto &pos{cubePositions[i]};
        auto model{glm::translate(glm::mat4{1.0f}, pos)};
        model = glm::rotate(model, glm::radians(static_cast<float>(glfwGetTime()) * 120.0f + 20.0f * i), glm::vec3{1.0f, 0.5f, 0.0f});
        auto transform{projection * view * model};
        program->SetUniform("transform", transform);
        glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_SHORT, 0);
    }
}

void Context::ProcessInput(GLFWwindow *window)
{
    if (!cameraControl)
        return;

    constexpr float cameraSpeed{0.05f};

    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
        cameraPos += cameraSpeed * cameraFront;

    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
        cameraPos -= cameraSpeed * cameraFront;

    auto cameraRight{glm::normalize(glm::cross(cameraUp, -cameraFront))};

    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
        cameraPos += cameraSpeed * cameraRight;

    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
        cameraPos -= cameraSpeed * cameraRight;

    auto cameraUp{glm::normalize(glm::cross(-cameraFront, cameraRight))};

    if (glfwGetKey(window, GLFW_KEY_E) == GLFW_PRESS)
        cameraPos += cameraSpeed * cameraUp;

    if (glfwGetKey(window, GLFW_KEY_Q) == GLFW_PRESS)
        cameraPos -= cameraSpeed * cameraUp;
}

void Context::Reshape(int width, int height)
{
    this->width = width;
    this->height = height;

    glViewport(0, 0, width, height);
}

void Context::MouseMove(double x, double y)
{
    if (!cameraControl)
        return;

    auto pos{glm::vec2{x, y}};
    auto deltaPos{pos - prevMousePos};

    constexpr float cameraRotateSpeed{0.8f};
    cameraYaw -= deltaPos.x * cameraRotateSpeed;
    cameraPitch -= deltaPos.y * cameraRotateSpeed;

    if (cameraYaw < 0.0f)
        cameraYaw += 360.0f;

    if (cameraYaw > 360.0f)
        cameraYaw -= 360.0f;

    if (cameraPitch > 89.0f)
        cameraPitch = 89.0f;

    if (cameraPitch < -89.0f)
        cameraPitch = -89.0f;

    prevMousePos = pos;
}

void Context::MouseButton(int button, int action, double x, double y)
{
    if (button == GLFW_MOUSE_BUTTON_RIGHT)
    {
        if (action == GLFW_PRESS)
        {
            prevMousePos = glm::vec2{x, y};
            cameraControl = true;
        }
        else if (action == GLFW_RELEASE)
        {
            cameraControl = false;
        }
    }
}

bool Context::Init()
{
    std::array<float, 120> vertices{
        -0.5f, -0.5f, 0.5f, 0.0f, 0.0f, // 0: 좌하단
        0.5f, -0.5f, 0.5f, 1.0f, 0.0f,  // 1: 우하단
        0.5f, 0.5f, 0.5f, 1.0f, 1.0f,   // 2: 우상단
        -0.5f, 0.5f, 0.5f, 0.0f, 1.0f,  // 3: 좌상단

        // --- 뒷면 (Back Face) ---
        0.5f, -0.5f, -0.5f, 0.0f, 0.0f,  // 4: 우하단 (뒤에서 볼 때 좌하단)
        -0.5f, -0.5f, -0.5f, 1.0f, 0.0f, // 5: 좌하단 (뒤에서 볼 때 우하단)
        -0.5f, 0.5f, -0.5f, 1.0f, 1.0f,  // 6: 좌상단 (뒤에서 볼 때 우상단)
        0.5f, 0.5f, -0.5f, 0.0f, 1.0f,   // 7: 우상단 (뒤에서 볼 때 좌상단)

        // --- 왼쪽면 (Left Face) ---
        -0.5f, -0.5f, -0.5f, 0.0f, 0.0f, // 8
        -0.5f, -0.5f, 0.5f, 1.0f, 0.0f,  // 9
        -0.5f, 0.5f, 0.5f, 1.0f, 1.0f,   // 10
        -0.5f, 0.5f, -0.5f, 0.0f, 1.0f,  // 11

        // --- 오른쪽면 (Right Face) ---
        0.5f, -0.5f, 0.5f, 0.0f, 0.0f,  // 12
        0.5f, -0.5f, -0.5f, 1.0f, 0.0f, // 13
        0.5f, 0.5f, -0.5f, 1.0f, 1.0f,  // 14
        0.5f, 0.5f, 0.5f, 0.0f, 1.0f,   // 15

        // --- 윗면 (Top Face) ---
        -0.5f, 0.5f, 0.5f, 0.0f, 0.0f,  // 16
        0.5f, 0.5f, 0.5f, 1.0f, 0.0f,   // 17
        0.5f, 0.5f, -0.5f, 1.0f, 1.0f,  // 18
        -0.5f, 0.5f, -0.5f, 0.0f, 1.0f, // 19

        // --- 아랫면 (Bottom Face) ---
        -0.5f, -0.5f, -0.5f, 0.0f, 0.0f, // 20
        0.5f, -0.5f, -0.5f, 1.0f, 0.0f,  // 21
        0.5f, -0.5f, 0.5f, 1.0f, 1.0f,   // 22
        -0.5f, -0.5f, 0.5f, 0.0f, 1.0f   // 23
    };

    std::array<uint16_t, 36> indices{
        // 앞면
        0, 1, 2, 2, 3, 0,
        // 뒷면
        4, 5, 6, 6, 7, 4,
        // 왼쪽
        8, 9, 10, 10, 11, 8,
        // 오른쪽
        12, 13, 14, 14, 15, 12,
        // 위쪽
        16, 17, 18, 18, 19, 16,
        // 아래쪽
        20, 21, 22, 22, 23, 20};

    vertexLayout = VertexLayout::Create();
    vertexBuffer = Buffer::CreateWithData(GL_ARRAY_BUFFER, GL_STATIC_DRAW, vertices.data(), sizeof(float) * vertices.size());

    vertexLayout->SetAttrib(0, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 5, 0);
    // vertexLayout->SetAttrib(1, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 5, sizeof(float) * 3);
    vertexLayout->SetAttrib(2, 2, GL_FLOAT, GL_FALSE, sizeof(float) * 5, sizeof(float) * 3);

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

    // auto image{Image::Create(512, 512)};
    // image->SetCheckImage(16, 16);

    texture = Texture::CreateFromImage(*image.get());

    auto image2{Image::Load("./OpenGLPratice/texture/awesomeface.png")};
    texture2 = Texture::CreateFromImage(*image2.get());

    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, texture->Get());
    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_2D, texture2->Get());

    program->Use();
    program->SetUniform("tex", 0);
    program->SetUniform("tex2", 1);

    // auto model{glm::rotate(glm::mat4{1.0f}, glm::radians(-55.0f), glm::vec3{1.0f, 0.0f, 0.0f})};
    // auto view{glm::translate(glm::mat4{1.0f}, glm::vec3{0.0f, 0.0f, -3.0f})};

    // auto projection{glm::perspective(glm::radians(45.0f), static_cast<float>(WINDOW_WIDTH) / WINDOW_HEIGHT, 0.01f, 10.0f)};
    // auto transform{projection * view * model};

    // program->SetUniform("transform", transform);

    return true;
}
