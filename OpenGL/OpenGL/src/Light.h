#pragma once

#include "vendor/glm/glm.hpp"

class Shader;

class Light
{
protected:
	glm::vec3 lightColor; // s_a & s_d & s_s

	float ambientIntensity;
	float diffuseIntensity;

public:
	explicit Light();
	explicit Light(const glm::vec3& aColor, float aIntensity, float dIntensity);
	~Light() = default;
};

