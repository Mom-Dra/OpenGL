#include "TimeManager.h"
#include <GLFW/glfw3.h>
#include <iostream>

TimeManager& TimeManager::GetInstance() noexcept
{
	static TimeManager instance;
	return instance;
}

void TimeManager::Initialize() noexcept
{
	lastTime = glfwGetTime();
}

float TimeManager::GetDeltaTime() const noexcept
{
	return static_cast<float>(deltaTime);
}

void TimeManager::Update() noexcept
{
	double currentTime{ glfwGetTime() };

	deltaTime = currentTime - lastTime;
	lastTime = currentTime;
}
