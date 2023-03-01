cmake_minimum_required(VERSION 3.16)
project(stitch)
set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_FLAGS "-lfmt -O3 -flto -march=native")
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ../output)

set(HEADER_FILES )

set(SOURCE_FILES 
		../src/main.cpp
		${HEADER_FILES})


include_directories(src)

add_executable(stitch ${SOURCE_FILES})

target_precompile_headers(stitch PRIVATE ${HEADER_FILES})
