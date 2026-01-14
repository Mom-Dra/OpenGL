#include <GL/glew.h>
#include "Window.h"
#include <iostream>
#include <algorithm>
#include "Renderer.h"

void Window::HandleKeys(GLFWwindow* window, int key, int code, int action, int mode)
{
	// key: 어떤 키인지, code: 플랫폼 특정 scan code(사용안함)
	// action: 키에서 발생한 행동(눌림, 땜 등), mode: 컨트롤, 알트, 쉬프트 등 입력

	Window* windowObject = static_cast<Window*>(glfwGetWindowUserPointer(window));

	if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
	{
		glfwSetWindowShouldClose(window, true);
	}

	if (key >= 0 && key < 1024)
	{
		if (action == GLFW_PRESS)
		{
			// 특정 키가 눌렸으면, keys 배열의 해당 키에 해당하는 값을 true로 설정
			windowObject->keys[key] = true;
			//std::cout << "Pressed: " << key << std::endl;
		}
		else if (action == GLFW_RELEASE)
		{
			// 특정 키가 떼졌으면, m_Keys 배열의 해당 키에 해당하는 값을 false로 설정
			windowObject->keys[key] = false;
			//std::cout << "Release: " << key << std::endl;
		}
	}
}

void Window::HandleMouse(GLFWwindow* window, double xPos, double yPos)
{
	// xpos, ypos: 마우스의 윈도우 내에서의 좌표 x, y 좌표

	Window* windowObject = static_cast<Window*>(glfwGetWindowUserPointer(window));

	// 마우스가 처음 움직이기 시작했을 때의 
	if (windowObject->mouseFirstMoved)
	{
		windowObject->lastX = xPos;
		windowObject->lastY = yPos;
		windowObject->mouseFirstMoved = false;
	}
	else // 계속 움직인다면
	{
		// 이전 프레임 위치에서 현재 프레임 위치를 뺀 값을 Change에 저장
		// y좌표는 뒤집어줌 (또는 여기서 뒤집지 않고 구현 로직에서 처리해도 상관 없음)

		windowObject->xChange = xPos - windowObject->lastX;
		windowObject->yChange = windowObject->lastY - yPos;

		// 변화량을 계산했으므로, 최신 위치를 last로 저장
		windowObject->lastX = xPos;
		windowObject->lastY = yPos;
	}
}

float Window::GetXChange() noexcept
{
	float change{ xChange };
	xChange = 0.0f;
	return change;
}

float Window::GetYChange() noexcept
{
	float change{ yChange };
	yChange = 0.0f;
	return change;
}

void Window::ChangeViewPort(int width, int height)
{
	GLCall(glViewport(0, 0, width, height));
}

void Window::CreateCallback()
{
	// HandleKeys 함수와 HandleMouse 함수를 GLFW 윈도우의 키보드/마우스 입력 콜백
	glfwSetKeyCallback(mainWindow, HandleKeys);
	glfwSetCursorPosCallback(mainWindow, HandleMouse);
}

Window::Window() noexcept : Window{ 800, 600 }
{

}

Window::Window(int windowWidth, int windowHeight) noexcept : width{ windowWidth }, height{ windowHeight }, mouseFirstMoved{ true }
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

	CreateCallback();

	// 프로그램을 실행하고 있으면 마우스 커서가 창 밖으로 나가지 않게!
	glfwSetInputMode(mainWindow, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

	// callback은 static 함수로 선언되었으므로, static 함수 중에 어떤 윈도우에서
	// 이벤트가 발생했는지를 지정해 주어야 함
	glfwSetWindowUserPointer(mainWindow, this);

	// 알파 채널 처리 방법
	GLCall(glEnable(GL_BLEND));
	GLCall(glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA));

	GLCall(glEnable(GL_CULL_FACE));
	GLCall(glCullFace(GL_BACK));

	GLCall(glEnable(GL_DEPTH_TEST));
}
