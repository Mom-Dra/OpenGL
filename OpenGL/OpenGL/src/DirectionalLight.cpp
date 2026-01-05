#include "DirectionalLight.h"
#include "Shader.h"

DirectionalLight::DirectionalLight() : direction{ glm::vec3{ 0.0f, -1.0f, 0.0f } }
{

}

DirectionalLight::DirectionalLight(const glm::vec3& aColor, float aIntensity, const glm::vec3& direction, float dIntensity)
	: Light{ aColor, aIntensity, dIntensity }, direction{ direction }
{

}

void DirectionalLight::UseLight(Shader& shader)
{
	shader.SetUniform3f("u_DirectionalLight.base.lightColor", lightColor.r, lightColor.g, lightColor.b);
	shader.SetUniform1f("u_DirectionalLight.base.ambientIntensity", ambientIntensity);
	shader.SetUniform1f("u_DirectionalLight.base.diffuseIntensity", diffuseIntensity);

	shader.SetUniform3f("u_DirectionalLight.direction", direction.x, direction.y, direction.z);
}
