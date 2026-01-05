#pragma once

#include "vendor/glm/glm.hpp"

class Shader;

class Light
{
private:
	glm::vec3 lightColor; // s_a & s_d & s_s
	glm::vec3 direction;

	float ambientIntensity;
	float diffuseIntensity;

public:
	explicit Light();
	explicit Light(const glm::vec3& aColor, float aIntensity,
		const glm::vec3& lightDir, float dIntensity);
	~Light();

	void UseLight(Shader& shader);
};

