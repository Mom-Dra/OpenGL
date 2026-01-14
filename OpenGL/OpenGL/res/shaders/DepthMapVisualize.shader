#shader vertex
#version 330

layout(location = 0) in vec3 position;
layout(location = 1) in vec2 texCoord;
layout(location = 2) in vec3 normal;

uniform mat4 u_Model;
uniform mat4 u_View;
uniform mat4 u_Projection;

out vec2 v_TexCoord;

void main()
{
	gl_Position = u_Projection * u_View * u_Model * vec4(position, 1.0);
	v_TexCoord = texCoord;
}

#shader fragment
#version 330

layout(location = 0) out vec4 color;

in vec2 v_TexCoord;

uniform sampler2D u_DepthMap;

void main()
{
	float depthValue = texture(u_DepthMap, v_TexCoord).r;
	color = vec4(vec3(depthValue), 1.0);
}