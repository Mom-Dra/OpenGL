#include "DirectionalLight.h"
#include "Shader.h"
#include "vendor/glm/gtc/matrix_transform.hpp"

DirectionalLight::DirectionalLight() : DirectionalLight{ LightingSettings::COLOR, LightingSettings::AMBIENT, LightingSettings::DIFFUSE, LightingSettings::SHADOW_WIDTH, LightingSettings::SHADOW_HEIGHT, DEFAULT_DIRECTION }
{

}

DirectionalLight::DirectionalLight(const glm::vec3& aColor, float aIntensity, float dIntensity, int shadowWidth, int shadowHeight, const glm::vec3& direction)
	: Light{ aColor, aIntensity, dIntensity, glm::ortho(-LightingSettings::ORTHO_SIZE, LightingSettings::ORTHO_SIZE, -LightingSettings::ORTHO_SIZE, LightingSettings::ORTHO_SIZE, LightingSettings::NEAR_PLANE, LightingSettings::FAR_PLANE), shadowWidth, shadowHeight }, direction{ direction }
{

}

void DirectionalLight::UseLight(Shader& shader, int slot)
{
	shader.SetUniform3f("u_DirectionalLight.base.lightColor", lightColor.r, lightColor.g, lightColor.b);
	shader.SetUniform1f("u_DirectionalLight.base.ambientIntensity", ambientIntensity);
	shader.SetUniform1f("u_DirectionalLight.base.diffuseIntensity", diffuseIntensity);

	shader.SetUniform3f("u_DirectionalLight.direction", direction.x, direction.y, direction.z);

	shader.SetUniform1i("u_DirectionalShadowMap", slot);
	shader.SetUniformMat4f("u_DirectionalLightTransform", CalculateLightTransform());
}

void DirectionalLight::UseLightForShadow(Shader& shader)
{
	shader.SetUniformMat4f("u_DirectionalLightTransform", CalculateLightTransform());
}

void DirectionalLight::SetDirection(const glm::vec3& dir)
{
	direction = dir;
}

glm::mat4 DirectionalLight::CalculateLightTransform()
{
	return lightProjection * glm::lookAt(-direction, glm::vec3{ 0.0f, 0.0f, 0.0f }, glm::vec3{ 0.0f, 1.0f, 0.0f });
}
