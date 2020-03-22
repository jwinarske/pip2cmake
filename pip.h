#include <string>
#include <ostream>
#include <vector>
#include <list>
#include <map>
#include <pugixml.hpp>

class pip
{
public:
    pip(std::string file, std::string outpath);
    void print();

    void gen_cmake();
    std::string get_cmake_file();
    std::vector<std::string> getDependencies();
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
    std::vector<std::string> split(const std::string& s, char delimiter);

    std::string base_path;
    std::string output_path;
    std::map<std::string, std::string> metaData;
    std::vector<std::string> dependencies;
    std::vector<std::string> exporters;
    std::vector<std::string> moduleFlags;
    std::string version;
};

