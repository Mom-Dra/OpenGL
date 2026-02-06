#include "common.h"
#include <fstream>
#include <sstream>

std::optional<std::string> LoadTextFile(std::string_view fileName)
{
    std::ifstream fin{fileName.data()};
    if (!fin.is_open())
    {
        SPDLOG_ERROR("failed to open file: {}", fileName);
        return {};
    }

    std::stringstream text;
    text << fin.rdbuf();
    return text.str();
}
