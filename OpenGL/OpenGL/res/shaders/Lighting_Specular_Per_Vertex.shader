#shader vertex
#version 330

layout(location = 0) in vec3 position;
layout(location = 1) in vec2 texCoord;
layout(location = 2) in vec3 normal;

out vec2 v_TexCoord;
out vec3 v_Normal;
out vec3 v_View;

uniform mat4 u_Model;
uniform mat4 u_View;
uniform mat4 u_Proj;
uniform vec3 u_EyePosition;

void main()
{
	gl_Position = u_Proj * u_View * u_Model * vec4(position, 1.0);
	v_TexCoord = texCoord;

	v_Normal = transpose(inverse(mat3(u_Model))) * normal;

	// Á¤Á¡ÀÇ world space ÁÂÇ¥
	vec3 worldPos = (u_Model * vec4(position, 1.0)).xyz;
	v_View = normalize(u_EyePosition - worldPos);
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
in vec3 v_View;

uniform sampler2D u_Texture;
uniform DirectionalLight u_DirectionalLight;
uniform Material u_Material;

void main()
{
	vec3 normal = normalize(v_Normal);
	vec3 lightDir = normalize(-u_DirectionalLight.direction);
	vec3 view = normalize(v_View);

	vec3 lightAmbient = u_DirectionalLight.lightColor * u_DirectionalLight.ambientIntensity;

	float diffuseFactor = max(dot(normalize(normal), lightDir), 0.0);
	vec3 lightDiffuse = u_DirectionalLight.lightColor * u_DirectionalLight.diffuseIntensity * diffuseFactor;

	vec3 rVec = 2.0 * normal * dot(normal, lightDir) - lightDir;
	vec3 lightSpecular = pow(max(dot(rVec, view), 0.0), u_Material.shininess) * u_DirectionalLight.lightColor * u_Material.specularIntensity;

	color = texture(u_Texture, v_TexCoord) * vec4(lightAmbient + lightDiffuse + lightSpecular, 1.0);

	// color = vec4(1.0, 1.0, 1.0, 1.0);
}