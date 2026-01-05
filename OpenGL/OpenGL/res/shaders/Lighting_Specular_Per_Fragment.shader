#shader vertex
#version 330

layout(location = 0) in vec3 position;
layout(location = 1) in vec2 texCoord;
layout(location = 2) in vec3 normal;

out vec2 v_TexCoord;
out vec3 v_Normal;
out vec3 v_WorldPosition;

uniform mat4 u_Model;
uniform mat4 u_View;
uniform mat4 u_Proj;

void main()
{
	gl_Position = u_Proj * u_View * u_Model * vec4(position, 1.0);
	v_TexCoord = texCoord;

	v_Normal = transpose(inverse(mat3(u_Model))) * normal;

	v_WorldPosition = (u_Model * vec4(position, 1.0)).xyz;
}

#shader fragment
#version 330

layout(location = 0) out vec4 color;

struct DirectionalLight
{
	vec3 lightColor;
	float ambientIntensity;

	vec3 direction;
	float diffuseIntensity;
};

struct Material
{
	float specularIntensity;
	float shininess;
};

in vec2 v_TexCoord;
in vec3 v_Normal;
in vec3 v_WorldPosition;

uniform vec3 u_EyePosition;
uniform sampler2D u_Texture;
uniform DirectionalLight u_DirectionalLight;
uniform Material u_Material;

void main()
{
	vec3 lightAmbient = u_DirectionalLight.lightColor * u_DirectionalLight.ambientIntensity;

	vec3 lightDir = normalize(-u_DirectionalLight.direction);
	float diffuseFactor = max(dot(normalize(v_Normal), lightDir), 0.0);
	vec3 lightDiffuse = u_DirectionalLight.lightColor * u_DirectionalLight.diffuseIntensity * diffuseFactor;

	vec3 fragToEye = normalize(u_EyePosition - v_WorldPosition);
	vec3 rVec = 2.0 * v_Normal * dot(v_Normal, lightDir) - lightDir;
	vec3 lightSpecular = pow(max(dot(rVec, fragToEye), 0.0), u_Material.shininess) * u_DirectionalLight.lightColor * u_Material.specularIntensity;

	color = texture(u_Texture, v_TexCoord) * vec4(lightAmbient + lightDiffuse + lightSpecular, 1.0);
	// color = texture(u_Texture, v_TexCoord) * vec4(lightAmbient + lightDiffuse, 1.0) + vec4(lightSpecular, 1.0);
}