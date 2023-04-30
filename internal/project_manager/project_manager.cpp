#include "project_manager.h"
#include <iostream>
#include <string_view>
#include <cstring>
#include <thread>

Stitch::ProjectManager::ProjectManager() {
        m_file_handler = nullptr;
        m_project_directory = fs::current_path();
        
        m_cmake_version.reserve(10);
        m_cxx_version.reserve(10);
        m_project_name.reserve(100);
        m_cxx_flags.reserve(100);
        m_export_compile_commands.reserve(3);
        
        m_vendor_headers.reserve(10000);
        m_vendor_directories.reserve(10000);
}

void Stitch::ProjectManager::initializeProject() noexcept {
        fs::create_directory(m_project_directory / "assets");
        fs::create_directory(m_project_directory / "build");
        fs::create_directory(m_project_directory / "output");
        fs::create_directory(m_project_directory / "src");
        fs::create_directory(m_project_directory / "internal");
        fs::create_directory(m_project_directory / "tools");
        fs::create_directory(m_project_directory / "vendor");
        
        createMain();
        obtainProjectInfo();
        createCMakeLists();
}

void Stitch::ProjectManager::obtainProjectInfo() noexcept {
        std::cout << "Enter CMake Version: ";
        std::cin  >> m_cmake_version;
        std::cout << "Enter C++ Version: ";
        std::cin  >> m_cxx_version;
        std::cout << "Enter Project Name: ";
        std::cin  >> m_project_name;
}

void Stitch::ProjectManager::updateProjectInfo() noexcept {
        m_file_handler = fopen((m_project_directory / "build/CMakeLists.txt").c_str(), "r");
        size_t line_size = 0;
        char * line = nullptr;
        
        getline(&line, &line_size, m_file_handler);
        m_cmake_version = std::string_view (line + 31, ((size_t)memchr(line + 31, ')', line_size) - (size_t)line) - 31);
        getline(&line, &line_size, m_file_handler);
        m_project_name = std::string_view (line + 8, ((size_t)memchr(line + 8, ')', line_size) - (size_t)line) - 8);
        getline(&line, &line_size, m_file_handler);
        m_cxx_version = std::string_view (line + 23, ((size_t)memchr(line + 23, ')', line_size) - (size_t)line) - 23);
        getline(&line, &line_size, m_file_handler);
        m_cxx_flags = std::string_view (line + 21, ((size_t)memchr(line + 21, '\"', line_size) - (size_t)line) - 21);
        getline(&line, &line_size, m_file_handler);
        m_export_compile_commands = std::string_view (line + 34, ((size_t)memchr(line + 34, ')', line_size) - (size_t)line) - 34);
        
        
        free(line);
        fclose(m_file_handler);
}

void Stitch::ProjectManager::obtainVendorInfo() noexcept {
        for(const auto& p: std::filesystem::recursive_directory_iterator(m_project_directory / "vendor")){
                if (!std::filesystem::is_directory(p)){
                        if (p.path().extension() == ".h" || p.path().extension() == ".hpp" || p.path().extension() == ".hxx"){
                                m_vendor_headers += fs::relative(p.path(), m_project_directory / "build").c_str();
                                m_vendor_headers += "\n                ";
                        }
                        else if (p.path().extension() == ".cpp" || p.path().extension() == ".c" || p.path().extension() == ".cxx"){
                                m_vendor_sources += fs::relative(p.path(), m_project_directory / "build").c_str();
                                m_vendor_sources += "\n                ";
                        }
                        else{
                                continue;
                        }
                } else {
                        m_vendor_directories += fs::relative(p.path(), m_project_directory / "build").c_str();
                        m_vendor_directories += "\n                ";
                }
        }
}

void Stitch::ProjectManager::obtainInternalInfo() noexcept {
        for(const auto& p: std::filesystem::recursive_directory_iterator(m_project_directory / "internal")){
                if (p.path().filename() == "main.cpp")
                        continue;
                
                if (!std::filesystem::is_directory(p)){
                        if (p.path().extension() == ".h" || p.path().extension() == ".hpp" || p.path().extension() == ".hxx"){
                                m_internal_headers += fs::relative(p.path(), m_project_directory / "build").c_str();
                                m_internal_headers += "\n                ";
                        }
                        else if (p.path().extension() == ".cpp" || p.path().extension() == ".c" || p.path().extension() == ".cxx"){
                                m_internal_sources += fs::relative(p.path(), m_project_directory / "build").c_str();
                                m_internal_sources += "\n                ";
                        }
                        else{
                                continue;
                        }
                } else {
                        m_internal_directories += fs::relative(p.path(), m_project_directory / "build").c_str();
                        m_internal_directories += "\n                ";
                }
        }
}

void Stitch::ProjectManager::createMain() noexcept {
        m_file_handler = fopen((m_project_directory / "src/main.cpp").c_str(), "w");
        fprintf(m_file_handler, "#include <iostream>\n\n"
                                "int main(int argc, char *argv[]) {\n"
                                "\tstd::cout << \"Hello World!\\n\";\n"
                                "\treturn 0;\n"
                                "}");
        fclose(m_file_handler);
}

void Stitch::ProjectManager::createCMakeLists() noexcept {
        obtainVendorInfo();
        obtainInternalInfo();
        m_file_handler = fopen((m_project_directory / "build/CMakeLists.txt").c_str(), "w");
        fprintf(m_file_handler,
                "cmake_minimum_required(VERSION %s)\n"
                "project(%s)\n"
                "set(CMAKE_CXX_STANDARD %s)\n"
                "set(CMAKE_CXX_FLAGS \"%s\")\n"
                "set(CMAKE_EXPORT_COMPILE_COMMANDS %s)\n"
                "set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ../output)\n\n"
                "set(VENDOR_HEADER_FILES\n"
                "                %s)\n\n"
                "set(INTERNAL_HEADER_FILES\n"
                "                %s)\n\n"
                "set(VENDOR_SOURCE_FILES\n"
                "                %s)\n\n"
                "set(INTERNAL_SOURCE_FILES\n"
                "                %s)\n\n"
                "set(SOURCE_FILES ../src/main.cpp\n"
                "                ${INTERNAL_SOURCE_FILES}\n"
                "                ${VENDOR_SOURCE_FILES}\n"
                "                ${INTERNAL_HEADER_FILES}\n"
                "                ${VENDOR_HEADER_FILES}\n"
                "                )\n\n"
                "include_directories(../vendor\n"
                "                %s%s)\n\n"
                "add_executable(%s ${SOURCE_FILES})\n"
                "target_precompile_headers(%s PRIVATE ${VENDOR_HEADER_FILES})\n",
                m_cmake_version.c_str(), m_project_name.c_str(), m_cxx_version.c_str(), m_cxx_flags.c_str(),
                m_export_compile_commands.c_str(), m_vendor_headers.c_str(), m_internal_headers.c_str(),
                m_vendor_sources.c_str(), m_internal_sources.c_str(), m_vendor_directories.c_str(),
                m_internal_directories.c_str(), m_project_name.c_str(), m_project_name.c_str()
        );
        fclose(m_file_handler);
}


void Stitch::ProjectManager::printHelp(const char *error) noexcept{
        std::cout << "⠀⠀⣀⣤⡀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀     [init] - initializes a stitch project\n"
                     "⠀⢼⣿⠋⣡⣴⣶⠶⠶⠶⠶⣤⣀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀     [run] - builds and runs the project\n"
                     "⠀⠈⢿⣧⣀⠈⢿⣆⠀⠀⠀⠀⠙⠻⣦⡀⠀⠀⠀⠀⠀⠀⣠⣴⠾⠛⠛⠛⠉⠀          [debug] - builds and runs the project in debug mode\n"
                     "⠀⠀⢠⡈⠛⢿⣾⣿⣦⡀⠀⠀⠀⠀⠈⢿⣄⠀⠀⠀⢠⡾⠋⠀⠀⠀⠀⠀⠀⠀          [release] - builds and runs the project in release mode\n"
                     "⠀⠀⠘⣧⡀⠀⠈⠙⢿⣿⣦⡀⠀⠀⠀⠈⢿⡄⠀⢠⣿⠁⠀⠀⠀⠀⠀⠀⠀⠀     [build] - builds the project but does not run it\n"
                     "⠀⠀⠀⠘⠁⠀⠀⠀⠀⠙⢿⣿⣦⡀⠀⠀⢸⣷⠀⢸⡇⠀⠀⠀⠀⠀⠀⠀⠀⠀          [debug] - builds the project in debug mode\n"
                     "⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠙⢿⣿⣦⡀⠸⡟⠀⠘⣿⠀⠀⠀⠀⠀⠀⠀⠀⠀          [release] - builds the project in release mode\n"
                     "⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠙⢿⣿⣦⡀⠀⠀⠹⣧⠀⠀⠀⠀⠀⠀⠀⠀     [set] - sets a CMake/compiler variable\n"
                     "⠀⠀⢀⣠⣴⠶⠶⠶⠶⠶⣶⣤⣶⠶⠄⠙⢿⣿⣦⡀⠀⠹⣧⠀⠀⠀⠀⠀⠀⠀          [compiler-flags] - sets the compiler flags to passed arguments \n"
                     "⠀⢠⡿⠉⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠙⢿⣿⣦⡀⠻⠀⠀⠀⠀⠀⠀⠀          [cpp-version] - sets the C++ version to passed argument \n"
                     "⠀⢸⡇⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠙⢿⣿⣦⡀⠀⠀⠀⠀⠀⠀          [project-name] - sets the project name to passed argument \n"
                     "⠀⠈⢿⣄⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⢀⠙⢿⣷⣄⠀⠀⠀⠀⠀          [export-compiler-commands] - sets compiler_commands.json generation \n"
                     "⠀⠀⠀⠙⢷⣤⡀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⣀⣴⠿⠁⠀⠙⢿⣷⡄⠀⠀⠀               [ON] - enables compiler_commands.json generation\n"
                     "⠀⠀⠀⠀⠀⠈⠉⠛⠒⠶⠶⠶⠶⠶⠶⠶⠖⠛⠉⠁⠀⠀⠀⠀⠀⠈⠻⢦⠀⠀               [OFF] - enables compiler_commands.json generation\n"
                     "⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠑⠀     [create] - creates .cpp + .h files for all passed arguments\n"
                     "                                        [internal] - creates the module in the 'internal' directory\n"
                     "                                        [vendor] - creates the module in the 'vendor'\n"
                     "                                   [delete] - deletes .cpp + .h files for all passed arguments\n"
                     "                                        [internal] - deletes the module in the 'internal' directory\n"
                     "                                        [vendor] - deletes the module in the 'vendor'\n"
                     "                                   [help] - prints out this help string\n"
                << error << "\n";
        exit(127);
}

void Stitch::ProjectManager::build(std::string_view build_mode) const noexcept {
        auto binary_path = m_project_directory / "output" / m_project_name;
        if(std::filesystem::exists(binary_path))
                std::filesystem::remove(binary_path);
        
        std::string cmake_command = "cd build && cmake -DCMAKE_BUILD_TYPE=";
        cmake_command += build_mode;
        cmake_command += " -DCMAKE_MAKE_PROGRAM=ninja -G Ninja . > /dev/null";
        std::system(cmake_command.c_str());
        
        std::string compile_command = "cd build && ninja -j ";
        compile_command += std::to_string(std::thread::hardware_concurrency());
        compile_command += " --quiet || exit";
        std::system(compile_command.c_str());
}

void Stitch::ProjectManager::run() const noexcept {
        auto binary_path = m_project_directory / "output" / m_project_name;
        if(std::filesystem::exists(binary_path))
                std::system(binary_path.c_str());
}

void Stitch::ProjectManager::createModules(const std::vector<std::string_view> &modules) noexcept {
        for (int32_t i = 3; i < modules.size(); i++) {
                fs::create_directory(m_project_directory / modules[2] / modules[i]);
                m_file_handler = fopen( ((m_project_directory / modules[2]  / modules[i] / modules[i]).replace_extension(".h")).c_str(), "w");
                fprintf(m_file_handler, "#pragma once\n");
                fclose(m_file_handler);
                
                m_file_handler = fopen( ((m_project_directory / modules[2]  / modules[i] / modules[i]).replace_extension(".cpp")).c_str(), "w");
                fprintf(m_file_handler, "#include \"%s.h\"\n", modules[i].data());
                fclose(m_file_handler);
        }
        updateProjectInfo();
        createCMakeLists();
}

void Stitch::ProjectManager::deleteModules(const std::vector<std::string_view> &modules) noexcept {
        for (int32_t i = 3; i < modules.size(); i++)
                fs::remove_all(m_project_directory / modules[2]  / modules[i]);
        updateProjectInfo();
        createCMakeLists();
}
