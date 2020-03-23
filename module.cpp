#include "module.h"

#include <ostream>
#include <iostream>
#include <sstream>
#include <istream>
#include <iomanip>
#include <fstream>
#include <dirent.h>

#include "utilities.h"


namespace pip2cmake
{

void module::listFilesRecursively(std::string basePath, std::string filename)
{
    DIR *dir = opendir(basePath.c_str());
    if (!dir)
    {
        return;
    }

    std::string path;
    struct dirent *dp;
    while ((dp = readdir(dir)) != NULL)
    {
        if (strcmp(dp->d_name, ".") != 0 && strcmp(dp->d_name, "..") != 0)
        {
            if (strcmp(dp->d_name, filename.c_str()) == 0)
            {
                module_header = basePath + sepd + dp->d_name;
                break;
            }

            // Construct new path from our base path
            path = basePath + sepd + std::string(dp->d_name);

            listFilesRecursively(path, filename);
        }
    }

    closedir(dir);
}

module::module(std::string basePath, std::string module)
{
    base_path = basePath;
    sepd = (base_path.find("\\") != std::string::npos) ? "\\" : "/";

    std::cout << "looking for module: " << module << std::endl;
    listFilesRecursively(basePath, module + ".h");

    if(!module_header.empty())
    {
        getMetaData( utilities::normalize_path(module_header) );
    }
}

void module::print()
{
    std::cout << "ID : " << metaData["ID"] << std::endl;
    std::cout << "Vendor : " << metaData["vendor"] << std::endl;
    std::cout << "Version : " << metaData["version"] << std::endl;
    std::cout << "Name : " << metaData["name"] << std::endl;
    std::cout << "Description : " << metaData["description"] << std::endl;
    std::cout << "Website : " << metaData["website"] << std::endl;
    std::cout << "License : " << metaData["license"] << std::endl;
    auto deps = getDependencies();
    for(auto const& dep : deps)
    {
        std::cout << "<< dep >> " << dep << std::endl;
    }
}

std::string module::getID()
{
    return metaData["ID"];
}

std::string module::getVendor()
{
    return metaData["vendor"];
}

std::string module::getVersion()
{
    return metaData["version"];
}

std::string module::getName()
{
    return metaData["name"];
}

std::string module::getDescription()
{
    return metaData["description"];
}

std::string module::getWebsite()
{
    return metaData["website"];
}

std::string module::getLicense()
{
    return metaData["license"];
}

std::vector<std::string> module::getDependencies()
{
    std::string d = utilities::trim( metaData["dependencies"] );
    auto list = utilities::split(d,' ');
    for(auto &item : list)
    {
        std::string val = utilities::trim(item);
        item = utilities::remove(val, ",");
    }
    return list;
}

void module::getMetaData(std::string inpfile)
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
    
    std::ifstream is(inpfile);
    if (!is)
        throw std::runtime_error("Error opening file: " + inpfile);

    metaData.clear();

    std::string line;
    while (std::getline(is, line)) {

        if(state == start)
        {
            if(line.find("BEGIN_JUCE_MODULE_DECLARATION") != std::string::npos)
            {
                state = begin;
                continue;
            }
        }
        else if(state == begin)
        {
            if(line.find("END_JUCE_MODULE_DECLARATION") != std::string::npos)
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
    auto lines = utilities::split(ss.str(),'\n');
    for(auto &l : lines)
    {
        l = utilities::trim(l);
        if(l.find(':') != std::string::npos)
        {
            auto kv = utilities::split(l, ':');
            if(!kv.empty())
            {
                key = kv[0];
                metaData[key] = utilities::trim(kv[1]);
            }
        }
        else
        {
            std::string value = metaData[key];
            metaData[key] += l;
        }
    }
}

} //namespace pip2cmake