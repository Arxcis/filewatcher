#include <iostream>
#include <iostream>
#include <cstdio>
#include <cstdlib>  

#ifdef WIN32
const auto& popen = _popen;
const auto& pclose = _pclose;
constexpr char PYTHON_COMMAND[] = "C:\\Python36\\python.exe watcher.py";
#else
constexpr char PYTHON_COMMAND[] = "./usr/local/bin/python3 watcher.py";
#endif

#include <thread>
#include <vector>
#include <sstream>
#include <chrono>
#include <set>
// @doc http://man7.org/linux/man-pages/man3/popen.3.html - 18.04.2018
// FILE *popen(const char *command, const char *type);
struct ExecuteResult 
{
    std::string msg;
    int statusode;
};

using OverkillTag = std::string;

struct FileTags 
{
    std::set<OverkillTag> materials;
    std::set<OverkillTag> scenes;
    std::set<OverkillTag> shaders;
    std::set<OverkillTag> models;
} fileTags;

struct ModifiedTags {
    std::set<OverkillTag> materials;
    std::set<OverkillTag> scenes;
    std::set<OverkillTag> shaders;
    std::set<OverkillTag> models;
} modifiedTags;

struct PipeResult 
{
    std::string directory;
    std::string filename;
    std::string tag;
    std::string modtype;
    std::string filepath;
};

auto executeRead(std::string command) -> ExecuteResult
{
    const int DATA_SIZE =  128;
    char data[DATA_SIZE] = "\n";
    int closestatus;
    
    FILE* pipestream = popen(command.data(), "r");
    if(!pipestream){
        return {"Could not open pipestream.", 1};
    }

    while (true) {
        fgets(data, DATA_SIZE, pipestream);
        printf("%s", data);

        PipeResult res;
        std::stringstream ss;
        ss << data;
        ss >> res.directory >> res.filename >> res.tag >> res.modtype >> res.filepath;
        std::cout << "{ " << res.directory 
                  << ", " << res.filename 
                  << ", " << res.tag 
                  << ", " << res.modtype 
                  << ", " << res.filepath << " }\n";
    }

    closestatus = pclose(pipestream);
    if (closestatus != 0) {
        return {"Could not close pipestream.", 1};
    }

    return {std::string(data), 0};
};

void logError(const std::string& command, const std::string& text) 
{
    printf("-------> ERROR   %s: %s" ,command.data(), text.data());
    std::cin.get();
    exit(1);
}

void logSuccess(const std::string& command, const std::string& text) 
{
    printf("-> %s:\n %s\n",command.data(), text.data());    
}

int main() 
{

    if (auto [resultstring, err] = executeRead(PYTHON_COMMAND); err) {
        logError(PYTHON_COMMAND, resultstring);
    } else {
        logSuccess(PYTHON_COMMAND, resultstring);
    }
    
    return 0;
}
