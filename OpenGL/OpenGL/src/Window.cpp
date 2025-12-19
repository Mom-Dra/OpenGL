#include <GL/glew.h>
#include "Window.h"
#include <iostream>
#include "Renderer.h"

Window::Window() : Window{ 800, 600 }
{
}

Window::Window(int windowWidth, int windowHeight) : width{ windowWidth }, height{ windowHeight }
{
}

Window::~Window()
{
	glfwDestroyWindow(mainWindow);
	glfwTerminate();
}

int Window::Initialize()
{
	if (!glfwInit())
		return -1;

	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	mainWindow = glfwCreateWindow(width, height, "Hello Wolrd", nullptr, nullptr);

	if (!mainWindow)
	{
		glfwTerminate();
		return -1;
	}

	// 추가된 코드, 실제 버퍼 크기를 가져옴(현재는 윈도우 크기와 같지만, 상황에 따라 다를 수 있음)
	glfwGetFramebufferSize(mainWindow, &bufferWidth, &bufferHeight);

	glfwMakeContextCurrent(mainWindow);

	glfwSwapInterval(1);

	if (glewInit() != GLEW_OK)
	{
		std::cout << "Error\n";
	}

	glViewport(0, 0, bufferWidth, bufferHeight);

	std::cout << glGetString(GL_VERSION) << std::endl; // 내 플랫폼의 GL_Version 출력해보기

	// 알파 채널 처리 방법
	GLCall(glEnable(GL_BLEND));
	GLCall(glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA));
}
