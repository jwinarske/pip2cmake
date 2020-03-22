#include "pip.h"

#include <iostream>
#include <sstream>
#include <istream>
#include <iomanip>
#include <fstream>
#include <sys/stat.h>
#include <algorithm>


inline std::string trim(const std::string &s)
{
   auto wsfront=std::find_if_not(s.begin(),s.end(),[](int c){return std::isspace(c);});
   auto wsback=std::find_if_not(s.rbegin(),s.rend(),[](int c){return std::isspace(c);}).base();
   return (wsback<=wsfront ? std::string() : std::string(wsfront,wsback));
}

void pip::getMetaData(std::string inpfile)
{
    enum metadata_state
    {
        start = 0,
        begin,
        end
    };

    std::vector<std::string> tokens;
    metadata_state state = start;
    std::stringstream ss;
    
    std::ifstream is(inpfile, std::ios::binary);
    if (!is)
        throw std::runtime_error("Error opening file: " + inpfile);

    metaData.clear();

    std::string line;
    while (std::getline(is, line)) {

        if(state == start)
        {
            if(line.find("BEGIN_JUCE_PIP_METADATA") != std::string::npos)
            {
                state = begin;
                continue;
            }
        }
        else if(state == begin)
        {
            if(line.find("END_JUCE_PIP_METADATA") != std::string::npos)
            {
                state = end;
            }
            else
            {
                ss << line << "\n";
            }
        }
    }

    std::string key;
    auto lines = split(ss.str(),'\n');
    for(auto &l : lines)
    {
        l = trim(l);
        if(l.find(':') != std::string::npos)
        {
            auto kv = split(l, ':');
            if(!kv.empty())
            {
                key = kv[0];
                metaData[key] = trim(kv[1]);
            }
        }
        else
        {
            std::string value = metaData[key];
            metaData[key] += l;
        }
    }

    dependencies = split(metaData["dependencies"],',');
    for(auto &d : dependencies)
    {
        d = trim(d);
    }

    exporters = split(metaData["exporters"],',');
    for(auto &e : exporters)
    {
        e = trim(e);
    }

    moduleFlags = split(metaData["moduleFlags"],',');
    for(auto &m : moduleFlags)
    {
        m = trim(m);
    }
}

std::vector<std::string> pip::getDependencies()
{
    return dependencies;
}

std::vector<std::string> pip::getExporters()
{
    return exporters;
}

std::vector<std::string> pip::getModuleFlags()
{
    return moduleFlags;
}

std::string pip::getDescription()
{
    return metaData["description"];
}

std::string pip::getMainClass()
{
    return metaData["mainClass"];
}

std::string pip::getName()
{
    return metaData["name"];
}

std::string pip::getType()
{
    return metaData["type"];
}

std::string pip::getVendor()
{
    return metaData["vendor"];
}

std::string pip::getVersion()
{
    return metaData["version"];
}

std::string pip::getWebsite()
{
    return metaData["website"];
}

pip::pip(std::string infile, std::string outpath)
{
    static constexpr char DEFAULT_VERSION[] = { '1', '.', '0', '.', '0', 0 };

    version = DEFAULT_VERSION;

    base_path = infile.substr(0, infile.find_last_of("\\/"));

    if(outpath.empty())
    {
        output_path = base_path;
    }
    else
    {
        output_path = outpath;
    }

    getMetaData(infile);
}

void pip::print()
{
    std::cout << "[Dependencies]\n";
    for(auto const& item : getDependencies())
    {
        std::cout << item << std::endl;
    }

    std::cout << "[Exporters]\n";
    for(auto const& item : getExporters())
    {
        std::cout << item << std::endl;
    }

    std::cout << "[ModuleFlags]\n";
    for(auto const& item : getModuleFlags())
    {
        std::cout << item << std::endl;
    }

    std::cout << "[Description]\n";
    std::cout << getDescription() << std::endl;
    std::cout << "[MainClass]\n";
    std::cout << getMainClass() << std::endl;
    std::cout << "[Name]\n";
    std::cout << getName() << std::endl;
    std::cout << "[Type]\n";
    std::cout << getType() << std::endl;
    std::cout << "[Vendor]\n";
    std::cout << getVendor() << std::endl;
    std::cout << "[Version]\n";
    std::cout << getVersion() << std::endl;
    std::cout << "[Website]\n";
    std::cout << getWebsite() << std::endl;
}

std::string pip::get_cmake_file()
{
    std::string sepd = (base_path.find("\\") != std::string::npos) ? "\\" : "/";
    std::string path = output_path + sepd + "CMakeLists.txt";
    return path;
}

void pip::gen_cmake()
{
    std::ofstream outfile (get_cmake_file(), std::ofstream::binary);
#if 0
    outfile << get_header();
    outfile << get_dependencies();
    outfile << get_cpp_standard();
    outfile << get_defines();
    outfile << get_include_dirs();
    outfile << get_autogen_vars();
    outfile << get_resource_files();
    outfile << get_source_list();
    //outfile << get_source_groups();
    outfile << get_executable();
    outfile << get_common_options();
    outfile << get_target_config();
#endif
    outfile.close();
}

std::vector<std::string> pip::split(const std::string& s, char delimiter)
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
