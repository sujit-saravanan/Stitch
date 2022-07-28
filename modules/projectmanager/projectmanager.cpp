#include "projectmanager.h"


// parses all module headers and automatically creates a CMakeLists.txt file that builds the project
std::string createCMake(const std::filesystem::path& current_dir, const std::string& project_name, const std::string& cpp_version, const std::string& compiler_flags){
    std::string CMakeLists;
    CMakeLists += "cmake_minimum_required(VERSION 3.23)\n"
                  "project(" + project_name + ")\n"
                  "set(CMAKE_CXX_STANDARD " + cpp_version + ")\n"
                  "set(CMAKE_CXX_FLAGS " + compiler_flags + ")\n"
                  "set(CMAKE_RUNTIME_OUTPUT_DIRECTORY " + current_dir.string() + "/output)\n\n\n";
    CMakeLists += getLibraries(current_dir);
    CMakeLists += "\n\nadd_executable(${PROJECT_NAME} ../source/main.cpp)\n\n";
    CMakeLists += getTargetLinkLibraries(current_dir);
    CMakeLists += getMainIncludes(current_dir);


    return CMakeLists;
}

void updateCMake(const std::filesystem::path& current_dir, std::string& project_name, std::string& cpp_version, std::string& compiler_flags){
    if (std::filesystem::exists(current_dir / "build" / "CMakeLists.txt")){
        get_cmake_file_info(current_dir, project_name, cpp_version, compiler_flags);
    } else {
        std::cout << "Could not find initialized project, please run stitch init\n";
    }

    std::ofstream filehandler;
    filehandler.open(current_dir / "build" / "CMakeLists.txt");
    filehandler << createCMake(current_dir, project_name, cpp_version, compiler_flags);
    filehandler.close();
}

void updateCMake(const std::filesystem::path& current_dir){
    std::string project_name;
    std::string cpp_version;
    std::string compiler_flags;
    if (std::filesystem::exists(current_dir / "build" / "CMakeLists.txt")){
        get_cmake_file_info(current_dir, project_name, cpp_version);
    } else {
        std::cout << "Could not find initialized project, please run stitch init\n";
    }

    std::ofstream filehandler;
    filehandler.open(current_dir / "build" / "CMakeLists.txt");
    filehandler << createCMake(current_dir, project_name, cpp_version, compiler_flags);
    filehandler.close();
}

void buildProject(const std::filesystem::path& current_dir, const std::string& project_name, const std::string& build_type){
    std::string binary_path = current_dir / "output" / project_name;
    if(std::filesystem::exists(binary_path))
        std::filesystem::remove(binary_path);
    std::string cmake_command = "cd build && cmake -DCMAKE_BUILD_TYPE=" + build_type + " -DCMAKE_MAKE_PROGRAM=ninja -G Ninja . > /dev/null";
    std::system(cmake_command.c_str());
    std::system("cd build && ninja -j 20 --quiet|| exit");
}

void runProject(const std::filesystem::path& binary_path){
    if(std::filesystem::exists(binary_path))
        std::system(binary_path.c_str());
}