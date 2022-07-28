#include "assist.h"

void printhelp(const std::string& error){
    std::cout <<"⠀⠀⣀⣤⡀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀     [init] - initializes a stitch project\n"
                "⠀⢼⣿⠋⣡⣴⣶⠶⠶⠶⠶⣤⣀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀     [run] - builds and runs the project\n"
                "⠀⠈⢿⣧⣀⠈⢿⣆⠀⠀⠀⠀⠙⠻⣦⡀⠀⠀⠀⠀⠀⠀⣠⣴⠾⠛⠛⠛⠉⠀          [dev] - builds and runs the project in debug mode\n"
                "⠀⠀⢠⡈⠛⢿⣾⣿⣦⡀⠀⠀⠀⠀⠈⢿⣄⠀⠀⠀⢠⡾⠋⠀⠀⠀⠀⠀⠀⠀          [release] - builds and runs the project in release mode\n"
                "⠀⠀⠘⣧⡀⠀⠈⠙⢿⣿⣦⡀⠀⠀⠀⠈⢿⡄⠀⢠⣿⠁⠀⠀⠀⠀⠀⠀⠀⠀     [modules] - creates or deletes modules\n"
                "⠀⠀⠀⠘⠁⠀⠀⠀⠀⠙⢿⣿⣦⡀⠀⠀⢸⣷⠀⢸⡇⠀⠀⠀⠀⠀⠀⠀⠀⠀          [create] - creates all modules passed as arguments\n"
                "⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠙⢿⣿⣦⡀⠸⡟⠀⠘⣿⠀⠀⠀⠀⠀⠀⠀⠀⠀          [delete] - deletes all modules passed as arguments\n"
                "⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠙⢿⣿⣦⡀⠀⠀⠹⣧⠀⠀⠀⠀⠀⠀⠀⠀     [set] - sets a CMake/compiler variable\n"
                "⠀⠀⢀⣠⣴⠶⠶⠶⠶⠶⣶⣤⣶⠶⠄⠙⢿⣿⣦⡀⠀⠹⣧⠀⠀⠀⠀⠀⠀⠀          [compiler-flags] - sets the compiler flags to passed arguments \n"
                "⠀⢠⡿⠉⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠙⢿⣿⣦⡀⠻⠀⠀⠀⠀⠀⠀⠀          [cpp-version] - sets the C++ version to passed argument \n"
                "⠀⢸⡇⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠙⢿⣿⣦⡀⠀⠀⠀⠀⠀⠀          [project-name] - sets the project name to passed argument \n"
                "⠀⠈⢿⣄⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⢀⠙⢿⣷⣄⠀⠀⠀⠀⠀     [build] - builds the project but does not run it\n"
                "⠀⠀⠀⠙⢷⣤⡀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⣀⣴⠿⠁⠀⠙⢿⣷⡄⠀⠀⠀          [dev] - builds the project in debug mode\n"
                "⠀⠀⠀⠀⠀⠈⠉⠛⠒⠶⠶⠶⠶⠶⠶⠶⠖⠛⠉⠁⠀⠀⠀⠀⠀⠈⠻⢦⠀⠀          [release] - builds the project in release mode\n"
                "⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠑⠀     [help] - prints out this help string\n"
                << error;
}