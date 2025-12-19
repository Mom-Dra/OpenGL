#shader vertex
#version 330 core

layout(location = 0) in vec4 position;
layout(location = 1) in vec4 color;

uniform mat4 u_Model;
uniform mat4 u_View;
uniform mat4 u_Proj;

// out vec4 v_Color;

void main()
{
	gl_Position = u_Proj * u_Model * position;
	// v_Color = color;
};

#shader fragment
#version 330 core
		
layout(location = 0) out vec4 color;

// uniform vec4 u_Color;
// in vec4 v_Color;
		
void main()
{
	color = vec4(1.0, 0.0, 0.0, 1.0);
};