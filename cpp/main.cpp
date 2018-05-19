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

void print_file_info(File& file) {
    // Type of file
    if (fs::is_regular_file(file.path)) {
        std::cout << "Found regular file..\n";
    } else if (fs::is_directory(file.path)) {
        std::cout << "Found directory..\n";
    } else {
        std::cout << "Found something else\n";
    }

    // File path
    std::cout << " path      : "  << file.path            << '\n'
              << " absolute  : "  << absolute(file.path)  << '\n'
              << " canonical : "  << canonical(file.path) << '\n'
              << " rootdir   : "  << file.path.root_directory() << '\n'
              << " parentpath: "  << file.path.parent_path() << '\n'
              << " parentstem: "  << file.path.parent_path().stem() <<'\n'
              << " filename  : "  << file.path.filename() << '\n'
              << " stem      : "  << file.path.stem() << '\n'
              << " extension : "  << file.path.extension() << '\n'; 

    // Last write time
    auto last_write_time = fs::last_write_time(file.path);
    auto sys_last_write_time = decltype(last_write_time)::clock::to_time_t(last_write_time);
    std::cout << " Last write: " << sys_last_write_time << '\n';
    std::cout << " localtime : " << std::localtime(&sys_last_write_time) << '\n'; 
    std::cout << " ascprint  : " << std::asctime(std::localtime(&sys_last_write_time)); 


    // Size of file
    try {
        std::cout << " Filesize  : " << fs::file_size(file.path) << '\n';
    }
    catch (fs::filesystem_error& e) {
        std::cout << " " << e.what() << '\n';
        std::cout << " Filesize  : You cannot take the file size of a directory...\n";
    }
    std::cout << '\n';
}

std::vector<fs::path> files;

void discover_files(std::string folderpath) {

    std::cout << "Discovering files in " << folderpath << "...\n";
    std::string path = folderpath;
    for (auto & p : fs::directory_iterator(path)) {

        File file{p.path()};
        print_file_info(file);
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
