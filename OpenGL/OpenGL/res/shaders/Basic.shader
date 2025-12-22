#shader vertex
#version 330 core

layout(location = 0) in vec4 position;
layout(location = 1) in vec2 texCoord;
layout(location = 2) in vec3 normal;

out vec2 v_TexCoord; // 버텍스 셰이더에서 추가적으로 출력하는 데이터(varying)
out vec3 v_Normal;

uniform mat4 u_Model;
uniform mat4 u_View;
uniform mat4 u_Proj;

void main()
{
	gl_Position = u_Proj * u_View * u_Model * position;
	v_TexCoord = texCoord;
	v_Normal = normal;
};

#shader fragment
#version 330 core
		
layout(location = 0) out vec4 color;

in vec2 v_TexCoord; // 버텍스 쉐이더에서 넘겨받은 데이터
in vec3 v_Normal;

uniform vec4 u_Color; // uniform에는 id가 할당됨
uniform sampler2D u_Texture; // texture는 sampler2D 타입
		
void main()
{
	color = vec4(1.0, 0.0, 0.0, 1.0);

	// normal을 색상으로 표시해 보기
	// vec3 normalColor = (v_Normal + 1.0) / 2.0;
	// color = vec4(normalColor, 1.0);

	// // uvchecker 입혀보기, 왜 검은색으로 보일까?
	// color = texture(u_Texture, v_TexCoord);
};