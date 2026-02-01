#include <iostream>
#include <vector>
#include <spdlog/spdlog.h>
#include <GLFW/glfw3.h>

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

    SPDLOG_INFO("Create glfw window");
    GLFWwindow *window{glfwCreateWindow(WINDOW_WIDTH, WINDOW_HEIGHT, WINDOW_NAME, nullptr, nullptr)};

    if (!window)
    {
        SPDLOG_ERROR("failed to create glfw window");
        glfwTerminate();
        return -1;
    }

    SPDLOG_INFO("Start main loop");
    while (!glfwWindowShouldClose(window))
        glfwPollEvents();

    glfwTerminate();
    return 0;
}
