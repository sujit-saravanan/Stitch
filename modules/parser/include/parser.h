#pragma once
#include "pch.h"


std::string getLibraries(const std::filesystem::path& current_dir);
std::string getIncludes(const std::filesystem::path& filepath);
std::string getMainIncludes(const std::filesystem::path& current_dir);
std::string getTargetLinkLibraries(const std::filesystem::path& current_dir);
void get_cmake_file_info(const std::filesystem::path& current_dir, std::string& project_name, std::string& cpp_version, std::string& compiler_flags);
void get_cmake_file_info(const std::filesystem::path& current_dir, std::string& project_name, std::string& cpp_version);