# stitch
A minimal, opinionated project manager designed around removing the frustrations that come with C++ build ecosystem.
So far, only Linux is guaranteed to be supported.
## Requirements
- CMake
- Ninja
## Installation
<ins>**You can find binaries in the release section**</ins>

**Building from Scratch:** The executable can be found in the `output` directory
```bash
git clone https://github.com/sujit-saravanan/build-system.git
cd build-system/build
cmake -DCMAKE_BUILD_TYPE=Release -DCMAKE_MAKE_PROGRAM=ninja -G Ninja .
ninja -j 20
```
## Usage
     [init] - initializes a stitch project
     [run] - builds and runs the project
          [debug] - builds and runs the project in debug mode
          [release] - builds and runs the project in release mode
     [build] - builds the project but does not run it
          [debug] - builds the project in debug mode
          [release] - builds the project in release mode
     [set] - sets a CMake/compiler variable
          [compiler-flags] - sets the compiler flags to passed arguments
          [cpp-version] - sets the C++ version to passed argument
          [project-name] - sets the project name to passed argument
          [export-compiler-commands] - sets compiler_commands.json generation
               [ON] - enables compiler_commands.json generation
               [OFF] - enables compiler_commands.json generation
     [create] - creates .cpp + .h files for all passed arguments
          [internal] - creates the module in the 'internal' directory
          [vendor] - creates the module in the 'vendor'
     [delete] - deletes .cpp + .h files for all passed arguments
          [internal] - deletes the module in the 'internal' directory
          [vendor] - deletes the module in the 'vendor'
     [help] - prints out this help string
