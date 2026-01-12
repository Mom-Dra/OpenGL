#shader vertex
#version 330

layout(location = 0) in vec3 position;
layout(location = 1) in vec2 texCoord;
layout(location = 2) in vec3 normal;
layout(location = 3) in vec3 tangent;
layout(location = 4) in vec3 bitangent;

out vec2 v_TexCoord;
out vec3 v_Normal;
out vec3 v_WorldPosition;
out mat3 v_TBN;

uniform mat4 u_Model;
uniform mat4 u_View;
uniform mat4 u_Projection;

void main()
{
	gl_Position = u_Projection * u_View * u_Model * vec4(position, 1.0);
	v_TexCoord = texCoord;

	mat3 ivtr = transpose(inverse(mat3(u_Model)));
	v_Normal = normalize(ivtr * normal);
	vec3 tan = normalize(ivtr * tangent);
	vec3 bitan = cross(v_Normal, tan);

	v_WorldPosition = (u_Model * vec4(position, 1.0)).xyz;
	
	// world space -> tan space ∫Ø»Ø ∫§≈Õ
	v_TBN = transpose(mat3(tan, bitan, v_Normal));
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
in vec3 v_TBN;

uniform vec3 u_EyePosition;
uniform sampler2D u_Texture;
uniform DirectionalLight u_DirectionalLight;
uniform Material u_Material;

uniform sampler2D u_Normal;

vec3 ToTangentSpaceVector(vec3 dir)
{
	return (v_TBN * dir);	
}

vec3 CalcLight(Light light, vec3 direction)
{
	vec3 normal = 2.0 * texture(u_Normal, v_TexCoord).rgb - 1.0;
	normal = normalize(normal);

	vec3 lightAmbient = light.lightColor * light.ambientIntensity;

	vec3 lightDir = normalize(-ToTangentSpaceVector(direction));
	float diffuseFactor = max(dot(normal, lightDir), 0.0);
	vec3 lightDiffuse = light.lightColor * light.diffuseIntensity * diffuseFactor;

	vec3 fragToEye = normalize(ToTangentSpaceVector(u_EyePosition - v_WorldPosition));
	vec3 rVec = 2.0 * normal * dot(normal, lightDir) - lightDir;
	vec3 lightSpecular = pow(max(dot(rVec, fragToEye), 0.0), u_Material.shininess) * light.lightColor * u_Material.specularIntensity;

	return (lightAmbient + lightDiffuse + lightSpecular);
}

vec3 CalcDirectionalLight()
{
	return CalcLight(u_DirectionalLight.base, u_DirectionalLight.direction);
}

void main()
{
	vec3 lightColor = CalcDirectionalLight();
	color = texture(u_Texture, v_TexCoord) * vec4(lightColor, 1.0);
}
