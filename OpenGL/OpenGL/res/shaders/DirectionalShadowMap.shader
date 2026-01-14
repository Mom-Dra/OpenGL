#shader vertex
#version 330

layout(location = 0) in vec3 position;

uniform mat4 u_Model;
uniform mat4 u_DirectionalLightTransform;

void main()
{
	gl_Position = u_DirectionalLightTransform * u_Model * vec4(position, 1.0);
}

#shader fragment
#version 330

void main()
{

}