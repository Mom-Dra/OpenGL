#pragma once

#include <GL/glew.h>
#include <string>
#include <unordered_map>
#include "vendor/glm/glm.hpp"

struct ShaderProgramSource
{
	std::string VertexSource;
	std::string FragSource;
};

class Shader
{
private:
	std::string filePath;
	GLuint rendererID;
	std::unordered_map<std::string, int> uniformLocationCache;

public:
	explicit Shader(const std::string& filePath);
	~Shader();

	void Bind() const;
	void UnBind() const;

	void SetUniform4f(const std::string& name, float v0, float v1, float v2, float v3);
	void SetUniform1f(const std::string& name, float value);

	void SetUniformMat4f(const std::string& name, const glm::mat4& matrix);

private:
	ShaderProgramSource ParseShader(const std::string& filePath);
	GLuint CompileShader(unsigned int type, const std::string& source);
	GLuint CreateShader(const std::string& vertexShader, const std::string& fragShader);

	int GetUniformLocation(const std::string& name);
};
