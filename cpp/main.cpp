#include <string>
#include <iostream>
#include <experimental/filesystem>
#include <cstdlib>
#include <vector>
namespace fs = std::experimental::filesystem;


struct File {
    fs::path path;
    time_t   last_write_time; 
};

std::vector<File> watchedFiles;

void print_verbose(File& file) {
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

void print(File& file) {
    std::cout << "filename: " << file.path.filename() << '\n'; 
}


void discover_files(std::string folderpath) {

    std::cout << "Discovering files in " << folderpath << "...\n";


    for (auto & p : fs::directory_iterator(folderpath)) {

        // If direcotry entry is another direectory, just continue
        if (fs::is_directory(p.path())) {
            continue;
        }

        // if it is a file but the stem is empty
        if (p.path().stem().string().empty()) {
            continue;
        }

        // if extension is empty
        if (p.path().extension().string().empty()) {
            continue;
        }


        auto last_write_time = fs::last_write_time(p.path());
        auto unix_last_write_time = decltype(last_write_time)::clock::to_time_t(last_write_time);

        auto file = watchedFiles.emplace_back(
            File { 
                p.path(),
                unix_last_write_time,
            }
        );
        print_verbose (file);
        //print(file);
    }
}

void get_modified_files(std::string folderpath) {
    std::cout << "\n\nGetting modified files in " << folderpath << "...\n";
    for (const auto & p : fs::directory_iterator(folderpath)) {

        // If direcotry entry is not a regular file
        if (!fs::is_regular_file(p.path())) {
            continue;
        }

        // We have found a file, let's compare it to our watchedFiles
        for (auto& file: watchedFiles) {

            // Find first file that matches
            if (fs::canonical(p.path()).string() == fs::canonical(file.path).string()) {

                auto last_write_time = fs::last_write_time(p.path());
                auto unix_last_write_time = decltype(last_write_time)::clock::to_time_t(last_write_time);

                // Check if file has been modified
                if (file.last_write_time != unix_last_write_time) {

                    // Cache the update write time
                    std::cout << p.path() << " was modified....\n";
                    file.last_write_time = unix_last_write_time;
                } else {
              //      std::cout << p.path() << " not modified....\n";
                }
                break;
            }
        }       
    }
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
        std::cout << "\n\nD : Discovered files\n";
        std::cout << "M : Modified   files\n";

        std::cin >> inCommand;
        if (inCommand.size() == 0) continue;
        command  = inCommand[0];
        std::cout << "\nCommand: " << int(command) << '\n';

        switch (toupper(command)) {

            case 'D':{
                // Rediscover files - making sure the cache is clear every time
                watchedFiles.clear();

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
