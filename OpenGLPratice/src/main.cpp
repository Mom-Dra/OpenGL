#include <glad/gl.h>
#include <GLFW/glfw3.h>
#include <spdlog/spdlog.h>

#include <iostream>
#include <vector>

#include "context.h"

void OnFrameBufferSizeChange(GLFWwindow *window, int width, int height)
{
    SPDLOG_INFO("frame size changed: ({} x {})", width, height);
    auto context{reinterpret_cast<Context *>(glfwGetWindowUserPointer(window))};
    context->Reshape(width, height);
}

void OnKeyEvent(GLFWwindow *window, int key, int scancode, int action, int mods)
{
    SPDLOG_INFO("key: {}, scancode: {}, action: {}, mods: {}{}{}",
                key, scancode,
                action == GLFW_PRESS ? "Pressed" : action == GLFW_RELEASE ? "Released"
                                               : action == GLFW_REPEAT    ? "Repeat"
                                                                          : "Unknown",
                mods & GLFW_MOD_CONTROL ? "C" : "-",
                mods & GLFW_MOD_SHIFT ? "S" : "-",
                mods & GLFW_MOD_ALT ? "A" : "-");

    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);
}

void OnCursorPos(GLFWwindow *window, double x, double y)
{
    auto context{reinterpret_cast<Context *>(glfwGetWindowUserPointer(window))};
    context->MouseMove(x, y);
}

void OnMouseButton(GLFWwindow *window, int button, int action, int modifier)
{
    auto context{reinterpret_cast<Context *>(glfwGetWindowUserPointer(window))};
    double x, y;
    glfwGetCursorPos(window, &x, &y);
    context->MouseButton(button, action, x, y);
}

int main()
{
    SPDLOG_INFO("Start Program");

    SPDLOG_INFO("Initialize glfw");
    if (!glfwInit())
    {
        const char *description{nullptr};
        glfwGetError(&description);
        SPDLOG_ERROR("failed to initilaize glfw: {}", description);
        return -1;
    }

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    SPDLOG_INFO("Create glfw window");
    GLFWwindow *window{glfwCreateWindow(WINDOW_WIDTH, WINDOW_HEIGHT, WINDOW_NAME, nullptr, nullptr)};

    if (!window)
    {
        SPDLOG_ERROR("failed to create glfw window");
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);

    if (!gladLoadGL((GLADloadfunc)glfwGetProcAddress))
    {
        SPDLOG_ERROR("failed to initialize glad");
        glfwTerminate();
        return -1;
    }

    auto glVersion{glGetString(GL_VERSION)};
    SPDLOG_INFO("OpenGL context version: {}", reinterpret_cast<const char *>(glVersion));

    auto context{Context::Create()};
    if (!context)
    {
        SPDLOG_ERROR("failed to create context");
        glfwTerminate();
        return -1;
    }
    glfwSetWindowUserPointer(window, context.get());

    OnFrameBufferSizeChange(window, WINDOW_WIDTH, WINDOW_HEIGHT);
    glfwSetFramebufferSizeCallback(window, OnFrameBufferSizeChange);
    glfwSetKeyCallback(window, OnKeyEvent);
    glfwSetCursorPosCallback(window, OnCursorPos);
    glfwSetMouseButtonCallback(window, OnMouseButton);

    SPDLOG_INFO("Start main loop");
    while (!glfwWindowShouldClose(window))
    {
        context->ProcessInput(window);
        context->Render();
        glfwSwapBuffers(window);
        glfwPollEvents();
    }
    context.reset();

    glfwTerminate();
    return 0;
}
