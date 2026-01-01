#include "Light.h"
#include "Shader.h"

Light::Light() : Light{ glm::vec3{1.0f, 1.0f, 1.0f}, 1.0f }
{

}

Light::Light(const glm::vec3& aColor, float aIntensity) : lightColor{ aColor }, ambientIntensity{ aIntensity }
{

}

Light::~Light()
{

}

void Light::useLight(Shader& shader)
{
	// alpha 값은 사실 사용 안함, 구조체 멤버도 일반 c코드 문법 이름으로 접근 가능
	shader.SetUniform3f("u_DirectionalLight.lightColor", lightColor.r, lightColor.g, lightColor.b);
	shader.SetUniform1f("u_DirectionalLight.ambientIntensity", ambientIntensity);
}
