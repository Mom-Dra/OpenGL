#ifndef PROGRAM_H_
#define PROGRAM_H_

#include "common.h"
#include "shader.h"

CLASS_PTR(Program);
class Program
{
private:
    uint32_t program{0};

public:
    static ProgramUPtr Create(const std::vector<ShaderPtr> &shaders);

    ~Program() noexcept;

    uint32_t Get() const noexcept { return program; }
    void Use() const;

    void SetUniform(std::string_view name, int value) const;
    void SetUniform(std::string_view name, float value) const;
    void SetUniform(std::string_view name, const glm::vec3 &value) const;
    void SetUniform(std::string_view name, const glm::mat4 &value) const;

private:
    explicit Program() noexcept = default;
    explicit Program(const Program &other) noexcept = delete;
    const Program &operator=(const Program &other) noexcept = delete;

    bool Link(const std::vector<ShaderPtr> &shaders);
};

#endif