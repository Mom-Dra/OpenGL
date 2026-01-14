#pragma once
#include "Light.h"

class DirectionalLight : public Light
{
public:
	static inline const glm::vec3 DEFAULT_DIRECTION{ 0.0f, -1.0f, 0.0f };

private:
	glm::vec3 direction{ DEFAULT_DIRECTION };

public:
	explicit DirectionalLight();
	explicit DirectionalLight(const glm::vec3& aColor, float aIntensity, float dIntensity, int shadowWidth, int shadowHeight, const glm::vec3& direction);

	~DirectionalLight() = default;

	void UseLight(Shader& shader, int slot);
	void UseLightForShadow(Shader& shader);

	glm::mat4 CalculateLightTransform();
};
