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

    ~Program();
    uint32_t Get() const noexcept { return program; }

private:
    explicit Program() = default;
    bool Link(const std::vector<ShaderPtr> &shaders);
};

#endif