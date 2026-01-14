#shader vertex
#version 330

layout(location = 0) in vec3 position;
layout(location = 1) in vec2 texCoord;
layout(location = 2) in vec3 normal;

out vec2 v_TexCoord;
out vec3 v_Normal;
out vec3 v_WorldPosition;
out vec4 v_DirectionalLightSpacePos;

uniform mat4 u_Model;
uniform mat4 u_View;
uniform mat4 u_Projection;
uniform mat4 u_DirectionalLightTransform;

void main()
{
	gl_Position = u_Projection * u_View * u_Model * vec4(position, 1.0);
	v_TexCoord = texCoord;

	v_Normal = transpose(inverse(mat3(u_Model))) * normal;

	v_WorldPosition = (u_Model * vec4(position, 1.0)).xyz;

	v_DirectionalLightSpacePos = u_DirectionalLightTransform * u_Model * vec4(position, 1.0);
}

#shader fragment
#version 330

layout(location = 0) out vec4 color;

struct Light
{
	vec3 lightColor;
	float ambientIntensity;
	float diffuseIntensity;
};

struct DirectionalLight
{
	Light base;
	vec3 direction;
};

struct Material
{
	float specularIntensity;
	float shininess;
};

in vec2 v_TexCoord;
in vec3 v_Normal;
in vec3 v_WorldPosition;
in vec4 v_DirectionalLightSpacePos;

uniform vec3 u_EyePosition;
uniform sampler2D u_Texture;
uniform DirectionalLight u_DirectionalLight;
uniform Material u_Material;
uniform sampler2D u_DirectionalShadowMap;

float CalcDirectionalShadowFactor(DirectionalLight light)
{
	vec3 projCoords = v_DirectionalLightSpacePos.xyz / v_DirectionalLightSpacePos.w;
	projCoords = (projCoords * 0.5) + 0.5;

	float closest = texture(u_DirectionalShadowMap, projCoords.xy).r;
	float current = projCoords.z;

	float shadow = current > closest ? 1.0 : 0.0;
	
	return shadow;
}

vec3 CalcLight(Light light, vec3 direction, float shadowFactor)
{
	vec3 lightAmbient = light.lightColor * light.ambientIntensity;
	vec3 normal = normalize(v_Normal);

	vec3 lightDir = normalize(-direction);
	float diffuseFactor = max(dot(normalize(normal), lightDir), 0.0);
	vec3 lightDiffuse = light.lightColor * light.diffuseIntensity * diffuseFactor;

	vec3 fragToEye = normalize(u_EyePosition - v_WorldPosition);
	vec3 rVec = 2.0 * normal * dot(normal, lightDir) - lightDir;
	vec3 lightSpecular = pow(max(dot(rVec, fragToEye), 0.0), u_Material.shininess) * light.lightColor * u_Material.specularIntensity;

	return (lightAmbient + (1.0 - shadowFactor) * (lightDiffuse + lightSpecular));
}

vec3 CalcDirectionalLight()
{
	float shadowFactor = CalcDirectionalShadowFactor(u_DirectionalLight);
	return CalcLight(u_DirectionalLight.base, u_DirectionalLight.direction, shadowFactor);
}

void main()
{
	vec3 lightColor = CalcDirectionalLight();
	color = texture(u_Texture, v_TexCoord) * vec4(lightColor, 1.0);
}