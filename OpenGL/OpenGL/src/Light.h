#pragma once
#include "vendor/glm/glm.hpp"
#include "ShadowMap.h"
#include <memory>

namespace LightingSettings
{
	const glm::vec3 COLOR{ 1.0f, 1.0f, 1.0f };
	constexpr float AMBIENT{ 0.2f };
	constexpr float DIFFUSE{ 1.0f };
	constexpr float ORTHO_SIZE{ 20.0f };
	constexpr float NEAR_PLANE{ 0.1f };
	constexpr float FAR_PLANE{ 100.0f };
	constexpr int SHADOW_WIDTH{ 1024 };
	constexpr int SHADOW_HEIGHT{ 1024 };
}

class Shader;

class Light
{
protected:
	glm::vec3 lightColor{ LightingSettings::COLOR };

	float ambientIntensity{ LightingSettings::AMBIENT };
	float diffuseIntensity{ LightingSettings::DIFFUSE };

	glm::mat4 lightProjection;
	std::unique_ptr<ShadowMap> shadowMap;

public:
	explicit Light(const glm::vec3& aColor, float aIntensity, float dIntensity, const glm::mat4 lightProjection, int shadowWidth, int shadowHeight);
	virtual ~Light() = default;

	ShadowMap& GetShadowMap() { return *shadowMap; }
};
