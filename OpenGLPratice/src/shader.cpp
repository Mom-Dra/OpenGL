#include "shader.h"

ShaderUPtr Shader::CreateFromFile(const std::string &fileName, GLenum shaderType)
{
    auto shader = ShaderUPtr(new Shader());
    if (!shader->LoadFile(fileName, shaderType))
        return nullptr;

    return std::move(shader);
}

Shader::~Shader()
{
    if (shader != 0)
    {
        glDeleteShader(shader);
    }
}

bool Shader::LoadFile(const std::string &fileName, GLenum shaderType)
{
    auto result{LoadTextFile(fileName)};
    if (!result.has_value())
        return false;

    auto &code{result.value()};
    const char *codePtr{code.c_str()};
    int32_t codeLength{static_cast<int32_t>(code.length())};

    shader = glCreateShader(shaderType);
    glShaderSource(shader, 1, (const GLchar *const *)&codePtr, &codeLength);
    glCompileShader(shader);

    int success{0};
    glGetShaderiv(shader, GL_COMPILE_STATUS, &success);

    if (success == 0)
    {
        int logLength{0};
        glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &logLength);

        std::string infoLog;
        infoLog.resize(logLength);

        glGetShaderInfoLog(shader, logLength, nullptr, infoLog.data());
        SPDLOG_ERROR("failed to compile shader: \"{}\"", fileName);
        SPDLOG_ERROR("reason: {}", infoLog);

        return false;
    }

    return true;
}