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

    glm::vec4 clearColor{glm::vec4{0.1f, 0.2f, 0.3f, 0.0f}};

    glm::vec3 lightColor{glm::vec3{1.0f, 1.0f, 1.0f}};
    glm::vec3 objectColor{glm::vec3{1.0f, 0.5f, 0.0f}};
    float ambientStrength{0.1f};

    glm::vec3 cameraPos{glm::vec3{0.0f, 0.0f, 3.0f}};
    glm::vec3 cameraFront{glm::vec3{0.0f, 0.0f, -1.0f}};
    glm::vec3 cameraUp{glm::vec3{0.0f, 1.0f, 0.0f}};

    float cameraPitch{0.0f};
    float cameraYaw{0.0f};

    bool cameraControl{false};
    glm::vec2 prevMousePos{glm::vec2{0.0f}};

    int width{WINDOW_WIDTH};
    int height{WINDOW_HEIGHT};

public:
    static ContextUPtr Create();

    void Render();
    void ProcessInput(GLFWwindow *window);
    void Reshape(int width, int height);
    void MouseMove(double x, double y);
    void MouseButton(int button, int action, double x, double y);

private:
    explicit Context() noexcept = default;
    explicit Context(const Context &other) noexcept = delete;
    Context &operator=(const Context &other) noexcept = delete;

    bool Init();
};

#endif