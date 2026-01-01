#pragma once

#include "vendor/glm/glm.hpp"

class Shader;

class Light
{
private:
	glm::vec3 lightColor; // s_a & s_d & s_s
	float ambientIntensity;

public:
	explicit Light();
	explicit Light(const glm::vec3& aColor, float aIntensity);
	~Light();

	void useLight(Shader& shader);
};

