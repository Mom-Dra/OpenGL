#shader vertex
#version 330 core

layout(location = 0) in vec3 position;
layout(location = 1) in vec2 texCoord;
layout(location = 2) in vec3 normal;

out vec2 v_TexCoord; // 버텍스 셰이더에서 추가적으로 출력하는 데이터(varying)
out vec3 v_Normal;

uniform mat4 u_Model;
uniform mat4 u_View;
uniform mat4 u_Proj;

void main()
{
	gl_Position = u_Proj * u_View * u_Model * vec4(position, 1.0);
	v_TexCoord = texCoord;
	v_Normal = normalize(transpose(inverse(mat3(u_Model))) * normal);
};

#shader fragment
#version 330 core
		
layout(location = 0) out vec4 color;

struct DirectionalLight
{
	vec3 lightColor;
	float ambientIntensity;

	vec3 direction;
	float diffuseIntensity;
};

in vec2 v_TexCoord; // 버텍스 쉐이더에서 넘겨받은 데이터
in vec3 v_Normal;

uniform sampler2D u_Texture; // texture는 sampler2D 타입
uniform DirectionalLight u_DirectionalLight;
		
void main()
{
	// c * a = s_a
	// color * ambient
	// 원래는 우리가 s_a 값으로 (0.5, 0.2, 0.3)을 넘겨주고 싶다..!
	vec3 lightAmbient = u_DirectionalLight.lightColor * u_DirectionalLight.ambientIntensity;

	float diffuseFactor = max(dot(normalize(v_Normal), normalize(-u_DirectionalLight.direction)), 0.0);
	vec3 lightDiffuse = u_DirectionalLight.lightColor * u_DirectionalLight.diffuseIntensity * diffuseFactor;

	color = texture(u_Texture, v_TexCoord) * vec4(lightAmbient + lightDiffuse, 1.0);

	// color = vec4(lightAmbient, 1.0);
	

	// color = texture(u_Texture, v_TexCoord);

	// color = vec4(1.0, 1.0, 0.0, 1.0);

	// normal을 색상으로 표시해 보기
	// vec3 normalColor = (v_Normal + 1.0) / 2.0;
	// color = vec4(normalColor, 1.0);

	// // uvchecker 입혀보기, 왜 검은색으로 보일까?
};