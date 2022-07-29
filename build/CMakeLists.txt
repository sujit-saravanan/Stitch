cmake_minimum_required(VERSION 3.23)
project(stitch)
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_FLAGS )
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ../output)


add_library(modules OBJECT ../modules/modules/modules.cpp)
target_include_directories(modules PUBLIC ../modules/modules/include)

add_library(projectmanager OBJECT ../modules/projectmanager/projectmanager.cpp)
target_include_directories(projectmanager PUBLIC ../modules/projectmanager/include)

add_library(parser OBJECT ../modules/parser/parser.cpp)
target_include_directories(parser PUBLIC ../modules/parser/include)

add_library(assist OBJECT ../modules/assist/assist.cpp)
target_include_directories(assist PUBLIC ../modules/assist/include)

add_library(init OBJECT ../modules/init/init.cpp)
target_include_directories(init PUBLIC ../modules/init/include)



add_executable(${PROJECT_NAME} ../source/main.cpp)

target_link_libraries(modules PUBLIC projectmanager)

target_link_libraries(projectmanager PUBLIC parser)

target_link_libraries(init PUBLIC parser)
target_link_libraries(init PUBLIC projectmanager)

target_link_libraries(${PROJECT_NAME} PUBLIC parser)
target_link_libraries(${PROJECT_NAME} PUBLIC init)
target_link_libraries(${PROJECT_NAME} PUBLIC modules)
target_link_libraries(${PROJECT_NAME} PUBLIC projectmanager)
target_link_libraries(${PROJECT_NAME} PUBLIC assist)

