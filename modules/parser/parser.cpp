#include "parser.h"

// returns a string with the appropriate CMake text to add all the modules in the modules subdirectory as libraries
std::string getLibraries(const std::filesystem::path& current_dir){
    std::string libraries;
    for(auto& module : std::filesystem::directory_iterator(current_dir / "modules")){
        std::string module_name = module.path().filename().string();
        if (module.path().stem().string() == "pch"){
            continue;
        }
        for (const auto & file : std::filesystem::directory_iterator(module))
            if(!std::filesystem::is_directory(file)){
                std::string source_name = file.path().stem().string();
                libraries += "add_library(";
                libraries += source_name;
                libraries += " STATIC ../modules/";
                libraries += module_name;
                libraries += "/";
                libraries += source_name;
                libraries += file.path().extension().string();
                libraries += ")\ntarget_include_directories(";
                libraries += source_name;
                libraries += " PUBLIC ../modules/";
                libraries += module_name;
                libraries += "/include)\n\n";
            }
    }
    return libraries;
}

// parses a module header and returns a string with the appropriate CMake text to link all the required modules the target module depends on.
std::string getIncludes(const std::filesystem::path& filepath){
    std::ifstream file;
    std::string line;
    std::string includes;
    file.open(filepath);
    while (std::getline(file  >> std::ws, line)) {
        if(line.compare(0, 10, "#include \"") == 0 ){
            std::string module;
            if (line.substr(line.length()-5, 1) == ".")
                module = line.substr(10, line.length() - 15);
            else
                module = line.substr(10, line.length() - 13);
            if (std::strcmp(module.c_str(), "pch") == 0)
                includes = std::string("target_include_directories(" + filepath.stem().string() + " PUBLIC ../modules/pch/include)\ntarget_precompile_headers(" + filepath.stem().string() + " PUBLIC ../modules/pch/include/pch.h)\n").append(includes);
            else
                includes += "target_link_libraries(" + filepath.stem().string() + " PUBLIC " + module + ")\n";
        }
    }
    file.close();
    if (!includes.empty())
        includes += "\n";
    return includes;
}

// parses the source/main.cpp and returns a string with the appropriate CMake text to link all the required modules the file depends on.
std::string getMainIncludes(const std::filesystem::path& current_dir){
    std::ifstream file;
    std::string line;
    std::string includes;

    file.open(current_dir / "source" / "main.cpp");
    while (std::getline(file  >> std::ws, line)) {
        if(line.compare(0, 10, "#include \"") == 0 ){
            std::string module;
            if (line.substr(line.length()-5, 1) == ".")
                module = line.substr(10, line.length() - 15);
            else
                module = line.substr(10, line.length() - 13);
            if (std::strcmp(module.c_str(), "pch") == 0)
                includes = std::string("target_precompile_headers(${PROJECT_NAME} PUBLIC ../modules/pch/include/pch.h)\n").append(includes);
            else
                includes += "target_link_libraries(${PROJECT_NAME} PUBLIC " + module + ")\n";
        }
    }
    file.close();
    if (!includes.empty())
        includes += "\n";
    return includes;
}

// asynchronously parses all of the modules' header files and returns a complete CMake text to link all of the modules with their required modules.
std::string getTargetLinkLibraries(const std::filesystem::path& current_dir){
    std::vector<std::future<std::string>> m_futures;
    std::string target_link_libraries;
    for(auto& module : std::filesystem::directory_iterator(current_dir / "modules")){
        m_futures.push_back(std::async(std::launch::async, getIncludes, module.path().string() + "/include/" + module.path().filename().string() + ".h"));
    }
    for(auto & future : m_futures){
        target_link_libraries += future.get();
    }
    return target_link_libraries;
}

void get_cmake_file_info(const std::filesystem::path& current_dir, std::string& project_name, std::string& cpp_version, std::string& compiler_flags){
    std::ifstream file;
    file.open(current_dir / "build" / "CMakeLists.txt");
    file.seekg(37);
    std::string test;

    std::getline(file, test);
    project_name += test.substr(8, test.length()-9);
    std::getline(file, test);
    cpp_version += test.substr(23, test.length()-24);
    std::getline(file, test);
    compiler_flags += test.substr(20, test.length()-21);
}

void get_cmake_file_info(const std::filesystem::path& current_dir, std::string& project_name, std::string& cpp_version){
    std::ifstream file;
    file.open(current_dir / "build" / "CMakeLists.txt");
    file.seekg(37);
    std::string test;

    std::getline(file, test);
    project_name += test.substr(8, test.length()-9);
    std::getline(file, test);
    cpp_version += test.substr(23, test.length()-24);
}