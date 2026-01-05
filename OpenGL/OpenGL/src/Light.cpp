#include "Light.h"
#include "Shader.h"

Light::Light() : Light{ glm::vec3{1.0f, 1.0f, 1.0f}, 0.2f, glm::vec3(0.0f, -1.0f, 0.0f), 1.0f    }
{

}

Light::Light(const glm::vec3& aColor, float aIntensity,
	const glm::vec3& lightDir, float dIntensity) : lightColor{ aColor }, ambientIntensity{ aIntensity },
	direction{ lightDir }, diffuseIntensity{ dIntensity }
{

}

Light::~Light()
{

}

void Light::UseLight(Shader& shader)
{
	// alpha 값은 사실 사용 안함, 구조체 멤버도 일반 c코드 문법 이름으로 접근 가능
	shader.SetUniform3f("u_DirectionalLight.lightColor", lightColor.r, lightColor.g, lightColor.b);
	shader.SetUniform1f("u_DirectionalLight.ambientIntensity", ambientIntensity);

	shader.SetUniform3f("u_DirectionalLight.direction", direction.x, direction.y, direction.z);
	shader.SetUniform1f("u_DirectionalLight.diffuseIntensity", diffuseIntensity);
}
