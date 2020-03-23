#pragma once

#include <string>
#include <vector>

namespace pip2cmake
{
    namespace utilities
    {
        std::string trim(const std::string &s);
        std::vector<std::string> split(const std::string& s, char delimiter);
        std::string normalize_path(std::string &path);
        std::string remove(std::string &s, std::string chars);
    }
}