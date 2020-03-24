#include "pip.h"

#include <iostream>
#include <sstream>
#include <istream>
#include <iomanip>
#include <fstream>
#include <sys/stat.h>
#include <algorithm>
#include <cstring>
#include <dirent.h>
#include <memory>
#include "utilities.h"

namespace pip2cmake
{

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

    metaData.clear();

    std::string key;
    std::string value;
    auto lines = utilities::split(ss.str(),'\n');
    for(auto &l : lines)
    {
        if(l.find(':') != std::string::npos)
        {
            auto kv = utilities::split(l, ':');
            
            key = utilities::trim(kv[0]);
            value = utilities::trim(kv[1]);

            if(value.find_last_of(",") == (value.length()-1))
            {
                value.append(" ");
            }

            metaData[key] = value;
        }
        else
        {
            if(l.length() > 2)
            {
                value = utilities::trim(l);
                if(value.find_last_of(",") == (value.length()-1))
                {
                    value.append(" ");
                }
                metaData[key].append(value);    
            }
        }
    }

#if defined(DEBUG)
    std::cout << "[dependencies] = [" << metaData["dependencies"] << "]" << std::endl;
#endif

    dependencies = utilities::split(metaData["dependencies"],',');
    for(auto &d : dependencies)
    {
        d = utilities::ltrim(d);
    }

    exporters = utilities::split(metaData["exporters"],',');
    for(auto &e : exporters)
    {
        e = utilities::ltrim(e);
    }

    moduleFlags = utilities::split(metaData["moduleFlags"],',');
    for(auto &m : moduleFlags)
    {
        m = utilities::ltrim(m);
    }
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
    return metaData [ META_DESCRIPTION ];
}

std::string pip::getMainClass()
{
    return metaData [ META_MAIN_CLASS ];
}

std::string pip::getName()
{
    return metaData [ META_NAME ];
}

std::string pip::getType()
{
    return metaData [ META_TYPE ];
}

std::string pip::getVendor()
{
    return metaData [ META_VENDOR ];
}

std::string pip::getVersion()
{
    return metaData [ META_VERISON ];
}

std::string pip::getWebsite()
{
    return metaData [ META_WEBSITE ];
}

pip::pip(std::string infile, std::string outpath)
{
    static constexpr char DEFAULT_VERSION[] = { '1', '.', '0', '.', '0', 0 };
    version = DEFAULT_VERSION;

    base_path = infile.substr(0, infile.find_last_of("\\/"));
    sepd = (base_path.find("\\") != std::string::npos) ? "\\" : "/";

    if(outpath.empty())
    {
        output_path = base_path;
    }
    else
    {
        output_path = outpath;
    }

    getMetaData(infile);

    getModules();
}

std::string pip::get_cmake_file()
{
    std::string path = output_path + sepd + "CMakeLists.txt";
    return path;
}

int pip::scandir_filter_modules(const struct dirent* dir_ent)
{
    if (!strcmp(dir_ent->d_name, ".") || !strcmp(dir_ent->d_name, "..")) return 0;
    std::string fname = dir_ent->d_name;

    if (fname.find("modules") == std::string::npos) return 0;

    return 1;
}

std::vector<std::string> pip::dirSearch(const char* dir_path)
{
    struct dirent **namelist;
    std::vector<std::string> res;

    int n = scandir( dir_path , &namelist, &pip::scandir_filter_modules, alphasort );
    for (int i=0; i<n; i++)
    {
        std::string fname = namelist[i]->d_name;
        res.push_back(fname);
        free(namelist[i]);
    }
    free(namelist);

    return res;
}

void pip::getModulesBasePath()
{
    struct stat info;

    std::string path = base_path + sepd + "../";
    scandir_filter_value = "modules";
    for(int i=0; i<5; i++)
    {
        auto list = dirSearch(path.c_str());
        if(!list.empty())
        {
            module_base_path = path + scandir_filter_value;
            return;
        }
        else
        {
            path += "../";
        }
    }
}

void pip::addModulesRecursively(std::string dependencies)
{
    if(dependencies.empty())
    {
        return;
    }

    auto list = utilities::getValueList(dependencies);

    for(auto const& item : list)
    {
        if ( modules.find(item) == modules.end() )
        {
            modules[item] = std::make_unique<module>(module_base_path, item);
            addModulesRecursively( modules[item]->getDependencies() );
        }
    }
}

void pip::getModules()
{
    if(module_base_path.empty())
    {
        getModulesBasePath();
    }

    std::cout << "Module Base: " << module_base_path << std::endl;

    modules.clear();

    addModulesRecursively( metaData["dependencies"] );
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

std::ostream& operator<<(std::ostream& os, const pip& p)
{
    auto m = p.metaData;

    os << "\n** PIP **\n";
    os << "[Exporters]\n";
    for(auto const& item : p.exporters)
    {
        os << item << "\n";
    }

    os << "[ModuleFlags]\n";
    for(auto const& item : p.moduleFlags)
    {
        os << item << "\n";
    }

    os << "Description: " << m[ pip::META_DESCRIPTION ] << "\n";
    os << "MainClass: " << m[ pip::META_MAIN_CLASS ] << "\n";
    os << "Name: " << m[ pip::META_NAME ] << "\n";
    os << "Type: " << m[ pip::META_TYPE ] << "\n";
    os << "Vendor: " << m[ pip::META_VENDOR ] << "\n";
    os << "Version: " << m[ pip::META_VERISON ] << "\n";
    os << "Website: " << m[ pip::META_WEBSITE ] << "\n";
    os << "Dependencies[]\n\n";
    for(auto &mod : p.modules)
    {
        os << *mod.second;
    }

    return os;
}

} //namespace pip2cmake