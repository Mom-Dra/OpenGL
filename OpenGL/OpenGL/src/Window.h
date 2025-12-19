#pragma once

#include <GLFW/glfw3.h>

class Window
{
private:
	GLFWwindow* mainWindow;
	int bufferWidth, bufferHeight;
	int width, height;

public:
	explicit Window();
	explicit Window(int windowWidth, int windowHeight);
	~Window();

	int Initialize();

	int GetBufferWidth() { return bufferWidth; }
	int GetBufferHeight() { return bufferHeight; }

	bool GetShouldClose() { return glfwWindowShouldClose(mainWindow); }
	void SwapBuffers() { glfwSwapBuffers(mainWindow); }
};
