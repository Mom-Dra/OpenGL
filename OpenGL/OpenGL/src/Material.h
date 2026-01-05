#pragma once

class Shader;

class Material
{
private:
	float specularIntensity;
	float shininess;

public:
	explicit Material();
	explicit Material(float sIntensity, float shine);
	~Material();

	void UseMaterial(Shader& shader);
};
