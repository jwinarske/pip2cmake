#include "utilities.h"

#include <string>
#include <sstream>
#include <iostream>
#include <algorithm>
#include <stack>

namespace pip2cmake
{
    namespace utilities
    {
        const std::string WHITESPACE = " \n\r\t\f\v";

        std::string ltrim(const std::string& s)
        {
            size_t start = s.find_first_not_of(WHITESPACE);
            return (start == std::string::npos) ? "" : s.substr(start);
        }

        std::string rtrim(const std::string& s)
        {
            size_t end = s.find_last_not_of(WHITESPACE);
            return (end == std::string::npos) ? "" : s.substr(0, end + 1);
        }

        std::string trim(const std::string& s)
        {
            return rtrim(ltrim(s));
        }

        std::string remove(std::string &s, std::string chars)
        {
            for(char c : chars)
            {
                s.erase(std::remove(s.begin(), s.end(), c), s.end());
            }
            return s;
        }

        std::vector<std::string> split(const std::string& s, char delimiter)
        {
            std::vector<std::string> tokens;
            std::string token;
            std::istringstream tokenStream(s);
            while (std::getline(tokenStream, token, delimiter))
            {
                tokens.push_back(token);
            }
            return tokens;
        }

        std::string normalize_path(std::string &path)
        {
            std::string sep = (path.find("\\") != std::string::npos) ? "\\" : "/";

            std::stack<std::string> st;
            std::string dir;
            std::string res = sep;

            size_t len_path = path.length();

            for (size_t i = 0; i < len_path; i++)
            {
                dir.clear();
                while (path[i] == sep[0])
                    i++;
                while (i < len_path && path[i] != sep[0]) {
                    dir.push_back(path[i]);
                    i++;
                }

                if (dir.compare("..") == 0)
                {
                    if (!st.empty())
                        st.pop();
                }
                else if (dir.compare(".") == 0)
                    continue;
                else if (dir.length() != 0)
                    st.push(dir);
            }

            std::stack<std::string> st1;
            while (!st.empty())
            {
                st1.push(st.top());
                st.pop();
            }

            while (!st1.empty())
            {
                std::string temp = st1.top();
                
                if (st1.size() != 1)
                {
                    res.append(temp + sep);
                }
                else
                {
                    res.append(temp);
                }

                st1.pop();
            }

            return res;
        }
    }
}