#include "init.h"
#include "assist.h"
#include <bits/stdc++.h>

int main(int argc, char* argv[]) {

    // obtains the current active directory
    auto current_dir = std::filesystem::current_path();

    // checks to make sure a command has been input
    std::string command;
    if (argc > 1)
        command = argv[1];
    else{
        printhelp("\nNo command has been entered\n");
        return 0;
    }

    // initializes the project directory with a modules, source, assets, tools, and build directory
    // creates a main.cpp file with a hello world program in the source folder if such a file doesn't already exist
    // takes the project name and C++ version as inputs, then creates a CMakeList.txt file with the appropriate values
    if (command == "init"){
        init(current_dir);
    }

    else if (command == "run"){
        std::string sub_command;
        if(argc >= 3)
            sub_command = argv[2];
        else{
            printhelp("\nNo subcommand entered(pick between [dev] or [release]\n");
            return 0;
        }

        std::string project_name;
        std::string cpp_version;
        std::string compiler_flags;
        updateCMake(current_dir, project_name, cpp_version, compiler_flags);

        // builds and runs the project in debug mode
        if (sub_command == "dev")
            buildProject(current_dir, project_name, "Debug");
        // builds and runs the project in release mode
        else if (sub_command == "release")
            buildProject(current_dir, project_name, "Release");

        runProject(current_dir / "output" / project_name);
    }

    else if (command == "modules"){
        std::string sub_command;
        if(argc >= 3)
            sub_command = argv[2];
        else{
            printhelp("\nNo subcommand entered(pick between [create] or [delete]\n");
            return 0;
        }

        if (argc < 4){
            printhelp("\nNo modules entered\n");
            return 0;
        }

        // creates directories inside the modules/ folder equivalent to the arguments passed after the sub-command
        // the structure of the module directories is as follows:
        //      /module-name
        //           /include
        //                module-name.h
        //           module-name.cpp
        if (sub_command == "create"){
            createModule(argc, argv, current_dir);
        }
        // deletes directories inside the modules/ folder equivalent to the arguments passed after the sub-command
        else if (sub_command == "delete"){
            deleteModule(argc, argv, current_dir);
        }
    }

    else if (command == "set"){
        std::string sub_command;
        if(argc >= 3)
            sub_command = argv[2];
        else{
            printhelp("\nNo subcommand entered(currently the only subcommand for this command is [compiler-flags])\n");
            return 0;
        }

        std::string project_name;
        std::string cpp_version;
        std::string compiler_flags;

        // Sets the compiler flags to the passed in arguments
        if (sub_command == "compiler-flags"){
            get_cmake_file_info(current_dir, project_name, cpp_version, compiler_flags);

            compiler_flags = " ";
            for (int i = 3; i < argc; i++){
                compiler_flags += argv[i];
                compiler_flags += + " ";
            }

            compiler_flags.pop_back();

            std::ofstream filehandler;
            filehandler.open(current_dir / "build" / "CMakeLists.txt");
            filehandler << createCMake(current_dir, project_name, cpp_version, compiler_flags);
            filehandler.close();
        }
        else if(sub_command == "project-name"){
            if (argc < 4){
                printhelp("\nNo project name entered");
                return 0;
            }

            get_cmake_file_info(current_dir, project_name, cpp_version, compiler_flags);
            project_name = argv[3];

            std::ofstream filehandler;
            filehandler.open(current_dir / "build" / "CMakeLists.txt");
            filehandler << createCMake(current_dir, project_name, cpp_version, compiler_flags);
            filehandler.close();
        }
        else if(sub_command == "cpp-version"){
            if (argc < 4){
                printhelp("\nNo C++ version number entered");
                return 0;
            }

            get_cmake_file_info(current_dir, project_name, cpp_version, compiler_flags);
            cpp_version = argv[3];

            std::ofstream filehandler;
            filehandler.open(current_dir / "build" / "CMakeLists.txt");
            filehandler << createCMake(current_dir, project_name, cpp_version, compiler_flags);
            filehandler.close();
        }
    }

    else if (command == "build"){
        std::string sub_command;
        if(argc >= 3)
            sub_command = argv[2];
        else{
            printhelp("\nNo subcommand entered(pick between [dev] or [release]\n");
            return 0;
        }


        std::string project_name;
        std::string cpp_version;
        std::string compiler_flags;
        updateCMake(current_dir, project_name, cpp_version, compiler_flags);

        if (sub_command == "dev")
            buildProject(current_dir, project_name, "Debug");
        else if (sub_command == "release")
            buildProject(current_dir, project_name, "Release");
    }

    else if (command == "help"){
        printhelp("");
        return 0;
    }


    return 0;
}
