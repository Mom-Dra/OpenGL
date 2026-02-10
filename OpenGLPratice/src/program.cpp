#include "program.h"

ProgramUPtr Program::Create(const std::vector<ShaderPtr> &shaders)
{
    auto program{ProgramUPtr{new Program{}}};
    if (!program->Link(shaders))
        return nullptr;

    return program;
}

Program::~Program()
{
    if (program != 0)
        glDeleteProgram(program);
}

void Program::Use() const
{
    glUseProgram(program);
}

void Program::SetUniform(std::string_view name, int value) const
{
    auto loc{glGetUniformLocation(program, name.data())};
    glUniform1i(loc, value);
}

void Program::SetUniform(std::string_view name, const glm::mat4 &value) const
{
    auto loc{glGetUniformLocation(program, name.data())};
    glUniformMatrix4fv(loc, 1, GL_FALSE, glm::value_ptr(value));
}

bool Program::Link(const std::vector<ShaderPtr> &shaders)
{
    program = glCreateProgram();
    for (auto &shader : shaders)
        glAttachShader(program, shader->Get());

    glLinkProgram(program);

    int success{0};
    glGetProgramiv(program, GL_LINK_STATUS, &success);

    if (success == 0)
    {
        int logLength{0};
        glGetProgramiv(program, GL_INFO_LOG_LENGTH, &logLength);

        std::string infoLog;
        infoLog.resize(logLength);

        glGetProgramInfoLog(program, logLength, nullptr, infoLog.data());
        SPDLOG_ERROR("failed to link program: {}", infoLog);
        return false;
    }

    return true;
}
