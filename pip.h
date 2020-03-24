#pragma once

#include <string>
#include <ostream>
#include <vector>
#include <list>
#include <map>
#include <dirent.h>
#include <memory>
#include <pugixml.hpp>

#include "module.h"


namespace pip2cmake
{
    class pip
    {
    public:
        pip(std::string file, std::string outpath);
        void print();

        void gen_cmake();
        std::string get_cmake_file();
        std::vector<std::string> getExporters();
        std::vector<std::string> getModuleFlags();
        std::string getDescription();
        std::string getMainClass();
        std::string getName();
        std::string getType();
        std::string getVendor();
        std::string getVersion();
        std::string getWebsite();

    private:
        void getMetaData(std::string inpfile);
        void getModules();
        void getModulesBasePath();
        void addModulesRecursively(std::string module);
        std::vector<std::string> dirSearch(const char* dir_path);
        std::string scandir_filter_value;
        static int scandir_filter_modules(const struct dirent* dir_ent);

        std::string base_path;
        std::string sepd;
        std::string output_path;
        std::string module_base_path;
        std::map<std::string, std::string> metaData;
        std::vector<std::string> dependencies;
        std::vector<std::string> exporters;
        std::vector<std::string> moduleFlags;
        std::string version;

        std::map<std::string, std::unique_ptr<module>> modules;
    };
}
