#pragma once

#include <GLFW/glfw3.h>
#include <array>

class Window
{
private:
	GLFWwindow* mainWindow;
	int bufferWidth, bufferHeight;
	int width, height;

	std::array<bool, 1024> keys{};
	float lastX, lastY;
	float xChange, yChange;
	bool mouseFirstMoved;

	static void HandleKeys(GLFWwindow* window, int key, int code, int action, int mode);
	static void HandleMouse(GLFWwindow* window, double xPos, double yPos);

public:
	explicit Window() noexcept;
	explicit Window(int windowWidth, int windowHeight) noexcept;
	~Window() noexcept;

	int Initialize();

	int GetBufferWidth() const noexcept { return bufferWidth; }
	int GetBufferHeight() const noexcept { return bufferHeight; }

	bool GetShouldClose() { return glfwWindowShouldClose(mainWindow); }
	void SwapBuffers() { glfwSwapBuffers(mainWindow); }

	const bool* GetKeys() const noexcept { return keys.data(); }
	float GetXChange() noexcept;
	float GetYChange() noexcept;

	void ChangeViewPort(int width, int height);

private:
	void CreateCallback();
};
