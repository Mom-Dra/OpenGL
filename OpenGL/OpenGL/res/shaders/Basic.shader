#shader vertex
#version 330 core

layout(location = 0) in vec4 position;
layout(location = 1) in vec2 texCoord;
// layout(location = 1) in vec4 color;

out vec2 v_TexCoord; // 버텍스 셰이더에서 추가적으로 출력하는 데이터(varying)

uniform mat4 u_Model;
uniform mat4 u_View;
uniform mat4 u_Proj;

// out vec4 v_Color;

void main()
{
	gl_Position = u_Proj * u_View * u_Model * position;
	v_TexCoord = texCoord;
	// v_Color = color;
};

#shader fragment
#version 330 core
		
layout(location = 0) out vec4 color;

in vec2 v_TexCoord; // 버텍스 쉐이더에서 넘겨받은 데이터

uniform vec4 u_Color; // uniform에는 id가 할당됨
uniform sampler2D u_Texture; // texture는 sampler2D 타입
		
void main()
{
	// texture 함수는 texcoord위치의 픽셀 색상 값을 정의된 방법에 따라 샘플링
	vec4 texColor = texture(u_Texture, v_TexCoord);

	// 이제 텍스처 이미지에서 샘플링한 색상을 fragment 색상으로 결정
	color = texColor;
};