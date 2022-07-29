#include "modules.h"

// creates directories inside the modules/ folder equivalent to the arguments passed after the sub-command
// the structure of the module directories is as follows:
//      /module-name
//           /include
//                module-name.h
//           module-name.cpp
void createModule(int argc, char* argv[], const std::filesystem::path& current_dir){
    std::ofstream filewriter;
    for (int i = 3; i < argc; i++){
        std::string header_name = argv[i];
        header_name += ".h";

        std::string class_name = argv[i];
        class_name += ".cpp";

        // creates a
        std::filesystem::create_directories(current_dir / "modules" / argv[i]);
        std::filesystem::create_directories(current_dir / "modules" / argv[i] / "include");

        filewriter.open(current_dir / "modules" / argv[i] / "include" / header_name);
        filewriter << "#pragma once";
        filewriter.close();

        filewriter.open(current_dir / "modules" / argv[i] / class_name);
        filewriter << "#include \"" << header_name << "\"";
        filewriter.close();
    }
    updateCMake(current_dir);
}

// deletes directories inside the modules/ folder equivalent to the arguments passed after the sub-command
void deleteModule(int argc, char* argv[], const std::filesystem::path& current_dir){
    std::ofstream filehandler;
    for (int i = 3; i < argc; i++){
        std::filesystem::remove_all(current_dir / "modules" / argv[i]);
    }
    updateCMake(current_dir);
}