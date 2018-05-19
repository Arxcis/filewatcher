#include <iostream>
#include <cstdio>
#include <cstdlib>  
#include <vector>
#include <sstream>
#include <chrono>
#include <thread>
#include <set>
#include <iomanip>

using OverkillTag = std::string;

struct FileTags 
{
    std::set<OverkillTag> textures;
    std::set<OverkillTag> shaders;
    std::set<OverkillTag> materials;
    std::set<OverkillTag> models;
    std::set<OverkillTag> scenes;
} fileTags;

struct ModifiedTags {
    std::set<OverkillTag> textures;
    std::set<OverkillTag> shaders;
    std::set<OverkillTag> materials;
    std::set<OverkillTag> models;
    std::set<OverkillTag> scenes;
} modifiedTags;

struct PipeResult 
{
    std::string event_type;
    std::string collection;
    std::string tag;
    std::string extension;
    std::string filepath;
};

// @doc http://man7.org/linux/man-pages/man3/popen.3.html - 18.04.2018
// FILE *popen(const char *command, const char *type);
void listenToWatcher()
{
    const int DATA_SIZE =  128;
    char data[DATA_SIZE] = "\n";
    int closestatus;
    
#ifdef WIN32
    const auto& popen = _popen;
    const auto& pclose = _pclose;
    FILE* pipestream = popen("C:\\Python36\\python.exe watcher.py", "r");
#else
    FILE* pipestream = popen("/usr/local/bin/python3 watcher.py", "r");
#endif

    if(!pipestream){
        return;
    }

    while (fgets(data, DATA_SIZE, pipestream) != nullptr) {

        PipeResult res;
        std::stringstream ss;
        ss << data;
        ss >> res.event_type >> res.collection >> res.tag >> res.extension >> res.filepath;
        std::cout << std::left
                  << "{ " << std::setw(8) << res.event_type 
                  << ", " << std::setw(8) << res.collection 
                  << ", " << std::setw(8) << res.tag 
                  << ", " << std::setw(8) << res.extension 
                  << ", " << std::setw(8) << res.filepath << " }\n";


        // @note turn this on for debugging
        //  Only necesarry if the pipe break, and the fgets go crazy
        using namespace std::chrono_literals;
        std::this_thread::sleep_for(.1s);
    }

    closestatus = pclose(pipestream);
    if (closestatus)
        printf("FAILED CLOSING the pipe\n");
    printf("CLOSING the pipe\n");
};

int main() 
{
    listenToWatcher();
    return 0;
}
