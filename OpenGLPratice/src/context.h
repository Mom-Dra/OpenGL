#ifndef CONTEXT_H_
#define CONTEXT_H_

#include "common.h"
#include "shader.h"
#include "program.h"

CLASS_PTR(Context);
class Context
{
private:
    ProgramUPtr program;

public:
    static ContextUPtr Create();
    void Render();

private:
    explicit Context() noexcept = default;
    bool Init();
};

#endif