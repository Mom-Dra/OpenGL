#ifndef SHADER_H_
#define SHADER_H_

#include "common.h"

CLASS_PTR(Shader);
class Shader
{
private:
    uint32_t shader{0};

public:
    static ShaderUPtr CreateFromFile(std::string_view fileName, GLenum shaderType);

    ~Shader();

    uint32_t Get() const noexcept { return shader; }

private:
    explicit Shader() = default;
    explicit Shader(const Shader &other) noexcept = delete;
    const Shader &operator=(const Shader &other) noexcept = delete;

    bool LoadFile(std::string_view fileName, GLenum shaderType);
};

#endif