#include "program.h"

ProgramUPtr Program::Create(const std::vector<ShaderPtr> &shaders)
{
    auto program{ProgramUPtr(new Program{})};
    if (!program->Link(shaders))
        return nullptr;
    return std::move(program);
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
