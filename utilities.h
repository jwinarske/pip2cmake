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
        bool replace(std::string& str, const std::string& from, const std::string& to);
        std::vector<std::string> getValueList(std::string value);
        std::string normalize_path(std::string &path);
        std::string remove(std::string &s, std::string chars);
    }
}