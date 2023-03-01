#include <iostream>
#include <fstream>

#include <vector>

#include <fmt/format.h>

#include <thread>

#include <filesystem>

namespace fs = std::filesystem;

constexpr uint32_t hash(const std::string &s) noexcept {
    uint32_t hash = 5381;
    for (const auto &c: s)
        hash = ((hash << 5) + hash) + (unsigned char) c;
    return hash;
}

constexpr inline uint32_t operator "" _(char const *p, size_t) {
    return hash(p);
}

struct ProjectInfo{
    std::string project_name;
    std::string cpp_version;
    std::string compiler_flags;
};

std::string generateCMakeStarter(const ProjectInfo &project_info) {
    std::string starter = fmt::format(
            "cmake_minimum_required(VERSION 3.16)\n"
            "project({})\n"
            "set(CMAKE_CXX_STANDARD {})\n"
            "set(CMAKE_CXX_FLAGS \"{}\")\n"
            "set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ../output)\n",
            project_info.project_name, project_info.cpp_version, project_info.compiler_flags
    );

    return starter;
}

std::string generateCMakeHeadersAndSources(const fs::path &project_folder){
    std::string headers = "set(HEADER_FILES ";
    std::string sources = "set(SOURCE_FILES ";

    for (auto const& file : std::filesystem::directory_iterator{project_folder / "src"})
        if (file.path().extension() == ".h"   ||
            file.path().extension() == ".hpp" ||
            file.path().extension() == ".hxx" ||
            file.path().extension() == ".hh"  ||
            file.path().extension() == ".cuh" )
            headers += "\n\t\t." + file.path().relative_path().string();
        else
        if (file.path().extension() == ".c"   ||
            file.path().extension() == ".cpp" ||
            file.path().extension() == ".cxx" ||
            file.path().extension() == ".cc"  ||
            file.path().extension() == ".cu" )
            sources += "\n\t\t." + file.path().relative_path().string();

    headers += ")\n";
    sources += "\n\t\t${HEADER_FILES})\n";
    return headers += "\n" + sources + "\n";
}

std::string generateCMakeEnder(const std::string &project_name){
    std::string Ender = "include_directories(src)\n\n" + fmt::format("add_executable({} ${{SOURCE_FILES}})\n\n", project_name) +
            fmt::format("target_precompile_headers({} PRIVATE ${{HEADER_FILES}})\n", project_name);
    return Ender;
}
void createCMakeLists(const ProjectInfo &project_info){
    std::ofstream stream;
    stream.open("build/CMakeLists.txt");
    stream << generateCMakeStarter(project_info);
    stream << "\n";
    stream << generateCMakeHeadersAndSources(".");
    stream << "\n";
    stream << generateCMakeEnder(project_info.project_name);
    stream.close();
}

ProjectInfo getProjectInfo(const fs::path &project_folder){
    std::ifstream CMakeLists(project_folder / "build" / "CMakeLists.txt");
    size_t size = std::filesystem::file_size(project_folder / "build" / "CMakeLists.txt");
    std::string content;

    std::string project_name, cpp_version, compiler_flags;

    std::getline(CMakeLists, content, '\n');
    std::getline(CMakeLists, content, '\n');

    const char* project_name_start = &content.c_str()[8];
    const char* project_name_end = static_cast<const char *>(memchr(project_name_start, ')', size));
    project_name = std::string(project_name_start, project_name_end);

    std::getline(CMakeLists, content, '\n');

    const char* cpp_version_start = &content.c_str()[23];
    const char* cpp_version_end = static_cast<const char *>(memchr(cpp_version_start, ')', size));
    cpp_version = std::string(cpp_version_start, cpp_version_end);

    std::getline(CMakeLists, content, '\n');

    const char* compiler_flags_start = &content.c_str()[21];
    const char* compiler_flags_end = static_cast<const char *>(memchr(compiler_flags_start, '\"', size));
    compiler_flags = std::string(compiler_flags_start, compiler_flags_end);


    return {project_name, cpp_version, compiler_flags};
}

void refreshCMakeHeadersAndSources(const fs::path &project_folder){
    createCMakeLists(getProjectInfo(project_folder));
}
std::string refreshCMakeHeadersAndSourcesAndGetProjectName(const fs::path &project_folder){
    auto project_info = getProjectInfo(project_folder);
    createCMakeLists(project_info);
    return project_info.project_name;
}

void init() {
    std::string project_name, cpp_version;

    std::cout << "Enter Project Name: ";
    std::cin >> project_name;

    std::cout << "Enter C++ Version: ";
    std::cin  >> cpp_version;

    fs::create_directories("src");
    fs::create_directories("build");
    fs::create_directories("assets");
    fs::create_directories("output");
    fs::create_directories("tools");

    std::ofstream stream;
    stream.open("src/main.cpp");
    stream << "#include <iostream>\n"
              "\n"
              "int main(int argc, char *argv[]) {\n"
              "\tstd::cout << \"Hello World!\\n\";\n"
              "\treturn 0;\n"
              "}";
    stream.close();

    createCMakeLists({project_name, cpp_version, ""});
}

void createCppHpps(const std::vector<std::string> &filenames){
    std::ofstream stream;

    for (const auto& filename : filenames){
        stream.open("src/" + filename + ".h");
        stream << "#pragma once\n";
        stream.close();

        stream.open("src/" + filename + ".cpp");
        stream << "#include \"" + filename + ".h\"\n";
        stream.close();
    }

    refreshCMakeHeadersAndSources(".");
}

void deleteCppHpps(const std::vector<std::string> &filenames){
    for (const auto& filename : filenames){
        std::remove(fmt::format("src/{}.h", filename).c_str());
        std::remove(fmt::format("src/{}.cpp", filename).c_str());
    }

    refreshCMakeHeadersAndSources(".");
}

void printHelp(const char *error) {
    std::cout << "⠀⠀⣀⣤⡀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀     [init] - initializes a stitch project\n"
                 "⠀⢼⣿⠋⣡⣴⣶⠶⠶⠶⠶⣤⣀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀     [run] - builds and runs the project\n"
                 "⠀⠈⢿⣧⣀⠈⢿⣆⠀⠀⠀⠀⠙⠻⣦⡀⠀⠀⠀⠀⠀⠀⣠⣴⠾⠛⠛⠛⠉⠀          [debug] - builds and runs the project in debug mode\n"
                 "⠀⠀⢠⡈⠛⢿⣾⣿⣦⡀⠀⠀⠀⠀⠈⢿⣄⠀⠀⠀⢠⡾⠋⠀⠀⠀⠀⠀⠀⠀          [release] - builds and runs the project in release mode\n"
                 "⠀⠀⠘⣧⡀⠀⠈⠙⢿⣿⣦⡀⠀⠀⠀⠈⢿⡄⠀⢠⣿⠁⠀⠀⠀⠀⠀⠀⠀⠀     [build] - builds the project but does not run it\n"
                 "⠀⠀⠀⠘⠁⠀⠀⠀⠀⠙⢿⣿⣦⡀⠀⠀⢸⣷⠀⢸⡇⠀⠀⠀⠀⠀⠀⠀⠀⠀         [debug] - builds the project in debug mode\n"
                 "⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠙⢿⣿⣦⡀⠸⡟⠀⠘⣿⠀⠀⠀⠀⠀⠀⠀⠀⠀         [release] - builds the project in release mode\n"
                 "⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠙⢿⣿⣦⡀⠀⠀⠹⣧⠀⠀⠀⠀⠀⠀⠀⠀     [set] - sets a CMake/compiler variable\n"
                 "⠀⠀⢀⣠⣴⠶⠶⠶⠶⠶⣶⣤⣶⠶⠄⠙⢿⣿⣦⡀⠀⠹⣧⠀⠀⠀⠀⠀⠀⠀          [compiler-flags] - sets the compiler flags to passed arguments \n"
                 "⠀⢠⡿⠉⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠙⢿⣿⣦⡀⠻⠀⠀⠀⠀⠀⠀⠀          [cpp-version] - sets the C++ version to passed argument \n"
                 "⠀⢸⡇⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠙⢿⣿⣦⡀⠀⠀⠀⠀⠀⠀          [project-name] - sets the project name to passed argument \n"
                 "⠀⠈⢿⣄⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⢀⠙⢿⣷⣄⠀⠀⠀⠀⠀     [create] - creates .cpp + .h files for all passed arguments\n"
                 "⠀⠀⠀⠙⢷⣤⡀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⣀⣴⠿⠁⠀⠙⢿⣷⡄⠀⠀⠀     [delete] - deletes .cpp + .h files for all passed arguments\n"
                 "⠀⠀⠀⠀⠀⠈⠉⠛⠒⠶⠶⠶⠶⠶⠶⠶⠖⠛⠉⠁⠀⠀⠀⠀⠀⠈⠻⢦⠀⠀     [help] - prints out this help string\n"
                 "⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠑⠀     \n"
              << error << "\n";
    exit(127);
}

void buildProject(const std::filesystem::path& project_folder, const std::string& project_name, const std::string& build_type){
    std::string binary_path = project_folder / "output" / project_name;
    if(std::filesystem::exists(binary_path))
        std::filesystem::remove(binary_path);
    std::string cmake_command = "cd build && cmake -DCMAKE_BUILD_TYPE=" + build_type + " -DCMAKE_MAKE_PROGRAM=ninja -G Ninja . > /dev/null";
    std::system(cmake_command.c_str());
    std::string compile_command = "cd build && ninja -j ";
    compile_command += std::to_string(std::thread::hardware_concurrency());
    compile_command += " --quiet || exit";
    std::system(compile_command.c_str());
}

void runProject(const std::filesystem::path& binary_path){
    if(std::filesystem::exists(binary_path))
        std::system(binary_path.c_str());
}

int main(int argc, char *argv[]) {
    if (argc < 2)
        printHelp("");

    switch (hash(argv[1])) {
        case "init"_:
            init();
            break;

        case "run"_:
            {
                if (argc < 3)
                    printHelp("CHOOSE DEBUG OR RELEASE");
                std::string project_name = refreshCMakeHeadersAndSourcesAndGetProjectName(".");
                switch (hash(argv[2])) {
                    case "debug"_:
                        buildProject(".", project_name, "Debug");
                        break;
                    case "release"_:
                        buildProject(".", project_name, "Release");
                        break;
                    default:
                        printHelp("CHOOSE DEBUG OR RELEASE");
                }
                runProject(fmt::format("./output/{}", project_name));
            }
            break;

        case "build"_:
            {
                if (argc < 3)
                    printHelp("CHOOSE DEBUG OR RELEASE");
                std::string project_name = refreshCMakeHeadersAndSourcesAndGetProjectName(".");
                switch (hash(argv[2])) {
                    case "debug"_:
                        buildProject(".", project_name, "Debug");
                        break;
                    case "release"_:
                        buildProject(".", project_name, "Release");
                        break;
                    default:
                        printHelp("CHOOSE DEBUG OR RELEASE");
                }
            }
            break;

        case "create"_:
            {
                if (argc < 3)
                    printHelp("NO FILES CREATED");
                std::vector<std::string> filenames;
                for (int i = 2; i < argc; i++)
                    filenames.emplace_back(argv[i]);
                createCppHpps(filenames);
            }
            break;

        case "delete"_:
            {
                if (argc < 3)
                    printHelp("NO FILES DELETED");
                std::vector<std::string> filenames;
                for (int i = 2; i < argc; i++)
                    filenames.emplace_back(argv[i]);
                deleteCppHpps(filenames);
            }
            break;

        case "set"_:
            {
                if (argc < 4)
                    printHelp("PASS IN VALUE");

                auto project_info = getProjectInfo(".");
                switch (hash(argv[2])) {
                    case "project-name"_:
                        createCMakeLists({argv[3], project_info.cpp_version, project_info.compiler_flags});
                        break;
                    case "cpp-version"_:
                        createCMakeLists({project_info.project_name, argv[3], project_info.compiler_flags});
                        break;
                    case "compiler-flags"_:
                        createCMakeLists({project_info.project_name, project_info.cpp_version, argv[3]});
                        break;
                    default:
                        printHelp("INVALID VALUE");
                }
            }
            break;
        case "help"_:
            printHelp("");
            break;
        default:
            printHelp("INVALID COMMAND\n");
            break;
    }
}