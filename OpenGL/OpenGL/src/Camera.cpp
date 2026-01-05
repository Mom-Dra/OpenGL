#include "Camera.h"
#include <GLFW/glfw3.h>
#include "Manager/TimeManager.h"

Camera::Camera() noexcept
{
}

Camera::Camera(const glm::vec3& initEye, const glm::vec3& initUp, float initYaw, float initPitch, float initMoveSpeed, float initTurnSpeed) noexcept
	: eye{ initEye }, front{ glm::vec3{0.0f, 0.0f, -1.0f} }, worldUp{ initUp },
	pitch{ initPitch }, yaw{ initYaw }, moveSpeed{ initMoveSpeed }, turnSpeed{ initTurnSpeed }
{
	Update();
}

Camera::~Camera()
{

}

void Camera::KeyControl(const bool* keys)
{
	// moveSpeed는 WASD 키를 눌렀을 때 카메라의 이동 속도,
	// deltaTime은 이전 프레임과 현재 프레임 사이의 소요 시간
	float velocity{ moveSpeed * TimeManager::GetInstance().GetDeltaTime() };

	if (keys[GLFW_KEY_W])
	{
		eye += front * velocity;
	}

	if (keys[GLFW_KEY_S])
	{
		eye -= front * velocity;
	}

	if (keys[GLFW_KEY_D])
	{
		eye += right * velocity;
	}

	if (keys[GLFW_KEY_A])
	{
		eye -= right * velocity;
	}
}

void Camera::MouseControl(float xChange, float yChange)
{
	// 마우스를 1픽셀만큼 움직였을 때 얼마나 각도를 변경할 것인가
	xChange *= turnSpeed;
	yChange *= turnSpeed;

	yaw += xChange;
	pitch += yChange;

	if (pitch > 89.0f)
		pitch = 89.0f;

	if (pitch < -89.0f)
		pitch = -89.0f;

	Update();
}

glm::mat4 Camera::CalculateViewMatrix() const noexcept
{
	return glm::lookAt(eye, eye + front, up);
}

void Camera::Update()
{
	// 마우스 입력에 따른 front 벡터의 변화 계산 (구면 좌표 -> 카르테시안 좌표)
	front.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
	front.y = sin(glm::radians(pitch));
	front.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
	front = glm::normalize(front);

	right = glm::normalize(glm::cross(front, worldUp));
	up = glm::normalize(glm::cross(right, front));
}
