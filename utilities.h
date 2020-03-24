#pragma once

#include <string>
#include <vector>

namespace pip2cmake
{
    namespace utilities
    {
        std::string ltrim(const std::string& s);
        std::string rtrim(const std::string& s);
        std::string trim(const std::string &s);
        std::vector<std::string> split(const std::string& s, char delimiter);
        std::vector<std::string> getValueList(std::string value);
        std::string normalize_path(std::string &path);
    }
}