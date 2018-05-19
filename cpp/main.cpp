#include <string>
#include <iostream>
#include <filesystem>
#include <cstdlib>
#include <vector>
namespace fs = std::experimental::filesystem;

enum FileState { 
    DISCOVERED,
    MODIFIED
};

struct File {
    fs::path  path;
    FileState state;
};

std::vector<fs::path> files;

void discover_files(std::string folderpath) {

    std::cout << "Discovering files in " << folderpath << "...\n";
    std::string path = folderpath;
    for (auto & p : fs::directory_iterator(path)) {
        std::cout << "  " << p << " - ";

        if (fs::is_regular_file(p)) {
            std::cout << " Is regular file..";
        } else if (fs::is_directory(p)) {
            std::cout << " Is directory..";
        } else {
            std::cout << " Is something else";
        }
        std::cout << '\n';
    }
}

void get_modified_files(std::string folderpath) {
    std::cout << "Getting modified files in " << folderpath << "...\n";
}

int main()
{
    std::vector<std::string> foldersToWatch = {
        "./assets",
        "./assets/textures",
        "./assets/materials",
        "./assets/shaders",
        "./assets/models",
        "./assets/scenes",
    };

    std::string inCommand = "Q";
    char command = 'Q';

    do {
        std::cin >> inCommand;
        if (inCommand.size() == 0) continue;
        command  = inCommand[0];
        std::cout << "\nCommand: " << int(command) << '\n';

        switch (toupper(command)) {

            case 'D':{
                for (const auto& folder : foldersToWatch) {
                    discover_files(folder);
                }
            } break;
 
            case 'M':{
                for (const auto& folder : foldersToWatch) {
                    get_modified_files(folder);
                }               
            } break;

            default:{
                std::cout << "Command not supported...\n";
            }
       }
    } while(toupper(command) != 'Q');
}
