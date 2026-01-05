#include "Material.h"
#include "Shader.h"

Material::Material() : Material{ 0.0f, 0.0f }
{

}

Material::Material(float sIntensity, float shine) : specularIntensity{ sIntensity }, shininess{ shine }
{

}

Material::~Material()
{

}

void Material::UseMaterial(Shader& shader)
{
	shader.SetUniform1f("u_Material.specularIntensity", specularIntensity);
	shader.SetUniform1f("u_Material.shininess", shininess);
}
