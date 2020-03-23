#pragma once

#include <string>
#include <vector>
#include <map>

namespace pip2cmake
{

    class module
    {
    public:
        module(std::string basePath, std::string module);
        void print();

        std::string getID();
        std::string getVendor();
        std::string getVersion();
        std::string getName();
        std::string getDescription();
        std::string getWebsite();
        std::string getLicense();
        std::vector<std::string> getDependencies();

    private:
        std::map<std::string, std::string> metaData;
        void getMetaData(std::string inpfile);
        void listFilesRecursively(std::string basePath, std::string filename);

        std::string normalizePath(std::string &path);

        std::string base_path;
        std::string sepd;
        std::string output_path;
        std::string module_header;

        std::string ID;
        std::string vendor;
        std::string version;
        std::string name;
        std::string description;
        std::string website;
        std::string license;
        std::vector<std::string> dependencies;
    };

}