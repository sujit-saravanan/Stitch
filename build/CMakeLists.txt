cmake_minimum_required(VERSION 3.16)
project(stitch)
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_FLAGS "-O3 -flto -march=native")
set(CMAKE_EXPORT_COMPILE_COMMANDS )
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ../output)

set(VENDOR_HEADER_FILES
                ../vendor/fnv_hash.h
                )

set(INTERNAL_HEADER_FILES
                ../internal/project_manager/project_manager.h
                )

set(VENDOR_SOURCE_FILES
                )

set(INTERNAL_SOURCE_FILES
                ../internal/project_manager/project_manager.cpp
                )

set(SOURCE_FILES ../src/main.cpp
                ${INTERNAL_SOURCE_FILES}
                ${VENDOR_SOURCE_FILES}
                ${INTERNAL_HEADER_FILES}
                ${VENDOR_HEADER_FILES}
                )

include_directories(../vendor
                ../internal/project_manager
                )

add_executable(stitch ${SOURCE_FILES})
target_precompile_headers(stitch PRIVATE ${VENDOR_HEADER_FILES})
