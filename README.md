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
git clone https://github.com/sujit-saravanan/stitch.git
cd stitch/build
cmake -DCMAKE_BUILD_TYPE=Release -DCMAKE_MAKE_PROGRAM=ninja -G Ninja .
ninja -j 20
```
## Usage
     [init] - initializes a stitch project
     
     [run] - builds and runs the project
          [dev] - builds and runs the project in debug mode
          [release] - builds and runs the project in release mode
          
     [modules] - creates or deletes modules
          [create] - creates all modules passed as arguments
          [delete] - deletes all modules passed as arguments
          
     [set] - sets a CMake/compiler variable
          [compiler-flags] - sets the compiler flags to passed arguments
          [cpp-version] - sets the C++ version to passed argument
          [project-name] - sets the project name to passed argument
          
     [build] - builds the project but does not run it
          [dev] - builds the project in debug mode
          [release] - builds the project in release mode
          
     [help] - prints out this help string
