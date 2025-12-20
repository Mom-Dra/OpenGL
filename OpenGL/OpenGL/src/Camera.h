#pragma once

//#include "vendor/glm/glm.hpp"
#include "vendor/glm/gtc/matrix_transform.hpp"

class Camera
{
private:
	glm::vec3 eye, front, up, right, worldUp;
	float yaw, pitch;
	float moveSpeed, turnSpeed;

public:
	explicit Camera() noexcept;
	explicit Camera(const glm::vec3& initEye, const glm::vec3& initUp, float initYaw, float initPitch,
		float initMoveSpeed, float initTurnSpeed) noexcept;

	~Camera();

	void KeyControl(const bool* keys);
	void MouseControl(float xChange, float yChange);

	glm::mat4 calculateViewMatrix() const noexcept;

private:
	void Update();
};

