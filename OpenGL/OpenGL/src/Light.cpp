#include "Light.h"
#include "Shader.h"

Light::Light() : Light{ glm::vec3{1.0f, 1.0f, 1.0f}, 0.2f,  1.0f }
{

}

Light::Light(const glm::vec3& aColor, float aIntensity, float dIntensity) : lightColor{ aColor }, ambientIntensity{ aIntensity },
	diffuseIntensity{ dIntensity }
{

}
