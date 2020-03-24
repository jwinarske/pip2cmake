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
        std::cout << module << " : metaData " << metaData.size() << std::endl;
        std::cout << "dependencies length: " << metaData["dependencies"].length() << std::endl;
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

    auto list = utilities::getValueList( metaData["dependencies"] );
    for(auto const& item : list)
    {
        std::cout << "<< dep >> " << item << std::endl;
    }

    list = utilities::getValueList( metaData["OSXFrameworks"] );
    for(auto const& item : list)
    {
        std::cout << "<< OSXFramework >> " << item << std::endl;
    }

    list = utilities::getValueList( metaData["iOSFrameworks"] );
    for(auto const& item : list)
    {
        std::cout << "<< iOSFrameworks >> " << item << std::endl;
    }

    list = utilities::getValueList( metaData["linuxLibs"] );
    for(auto const& item : list)
    {
        std::cout << "<< linuxLibs >> " << item << std::endl;
    }

    list = utilities::getValueList( metaData["linuxPackages"] );
    for(auto const& item : list)
    {
        std::cout << "<< linuxPackages >> " << item << std::endl;
    }

    auto deps = utilities::getValueList( metaData["mingwLibs"] );
    for(auto const& dep : deps)
    {
        std::cout << "<< mingwLibs >> " << dep << std::endl;
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

std::string module::getDependencies()
{
    return metaData["dependencies"];
}

std::string module::getOSXFrameworks()
{
    return metaData["OSXFrameworks"];
}

std::string module::getiOSFrameworks()
{
    return metaData["iOSFrameworks"];
}

std::string module::getLinuxLibs()
{
    return metaData["linuxLibs"];
}

std::string module::getLinuxPackages()
{
    return metaData["linuxPackages"];
}

std::string module::getMingwLibs()
{
    return metaData["mingwLibs"];
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
                ss << line << " \n";
            }
        }
    }

    metaData.clear();

    std::string key;
    auto lines = utilities::split(ss.str(),'\n');
    for(auto &l : lines)
    {
        if(l.find(':') != std::string::npos)
        {
            auto kv = utilities::split(l, ':');
            
            std::string key = utilities::trim(kv[0]);
            std::string value = utilities::trim(kv[1]);

            metaData[key] = value;
        }
        else
        {
            if(l.length() > 2)
            {
                std::replace( l.begin(), l.end(), ',', ' ');
                metaData[key] += " " + l;
            }
        }
    }

    std::cout << "[dependencies] = [" << metaData["dependencies"] << "]" << std::endl;
}

} //namespace pip2cmake