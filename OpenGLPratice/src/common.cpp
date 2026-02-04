#include "common.h"
#include <fstream>
#include <sstream>

std::optional<std::string> LoadTextFile(const std::string &fileName)
{
    std::ifstream fin{fileName};
    if (!fin.is_open())
    {
        SPDLOG_ERROR("failed to open file: {}", fileName);
        return {};
    }

    std::stringstream text;
    text << fin.rdbuf();
    return text.str();
}
