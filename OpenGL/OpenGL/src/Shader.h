#pragma once

#include <GL/glew.h>
#include <string>
#include <unordered_map>

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
	void SetUniform4f(const std::string& name, float value);

private:
	ShaderProgramSource ParseShader(const std::string& filePath);
	GLuint CompileShader(unsigned int type, const std::string& source);
	GLuint CreateShader(const std::string& vertexShader, const std::string& fragShader);

	int GetUniformLocation(const std::string& name);
};
