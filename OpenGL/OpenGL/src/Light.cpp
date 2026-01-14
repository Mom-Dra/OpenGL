#include "Light.h"
#include "Shader.h"

Light::Light(const glm::vec3& aColor, float aIntensity, float dIntensity, const glm::mat4 lightProjection, int shadowWidth, int shadowHeight) : lightColor{ aColor }, ambientIntensity{ aIntensity },
diffuseIntensity{ dIntensity }, lightProjection{ lightProjection }, shadowMap{ std::make_unique<ShadowMap>(shadowWidth, shadowHeight) }
{

}
