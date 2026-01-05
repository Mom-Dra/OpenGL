#pragma once

#include "Light.h"

class DirectionalLight : public Light
{
private:
	glm::vec3 direction;

public:
	explicit DirectionalLight();
	explicit DirectionalLight(const glm::vec3& aColor, float aIntensity,
		const glm::vec3& direction, float dIntensity);

	~DirectionalLight() = default;

	void UseLight(Shader& shader);
};
