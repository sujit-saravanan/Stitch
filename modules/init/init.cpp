#include "init.h"

// creates module, source, assets, tools, and build folders within the project directory
void createBaseDirectories(const std::filesystem::path& current_dir){
    std::filesystem::create_directories(current_dir / "modules");
    std::filesystem::create_directories(current_dir / "source");
    std::filesystem::create_directories(current_dir / "assets");
    std::filesystem::create_directories(current_dir / "tools");
    std::filesystem::create_directories(current_dir / "build");
    std::filesystem::create_directories(current_dir / "output");
}

// creates a hello world program titled main.cpp inside the source directory
void createMainFile(const std::filesystem::path& current_dir){
    std::ofstream filewriter;
    filewriter.open(current_dir / "source/main.cpp");
    filewriter << "#include <iostream>\n"
                   "\n"
                   "int main() {\n"
                   "    std::cout << \"Hello World!\\n\";\n"
                   "    return 0;\n"
                   "}";
    filewriter.close();
}

// Takes in the project name and C++ version, then initializes a Stitch project in the active directory
void init(const std::filesystem::path& current_dir){
    std::string project_name;
    std::string cpp_version;
    std::string compiler_flags;

    std::cout << "Enter project name: ";
    getline (std::cin, project_name);

    std::cout << "Enter C++ version: ";
    std::cin >> cpp_version;

    createBaseDirectories(current_dir);
    if(!std::filesystem::exists(current_dir / "source" / "main.cpp"))
        createMainFile(current_dir);
    std::ofstream filewriter;
    filewriter.open(current_dir / "build" / "CMakeLists.txt");
    filewriter << createCMake(current_dir, project_name, cpp_version, "");
    filewriter.close();

    std::filesystem::create_directories(current_dir / "modules" / "pch");
    std::filesystem::create_directories(current_dir / "modules" / "pch" / "include");

    filewriter.open(current_dir / "modules" / "pch" / "include" / "pch.h");
    filewriter << "#pragma once";
    filewriter.close();

    filewriter.open(current_dir / "modules" / "pch" / "pch.cpp");
    filewriter << "#include \"" << "pch.h" << "\"";
    filewriter.close();
}