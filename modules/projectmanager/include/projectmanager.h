#pragma once
#include <thread>
#include "parser.h"

std::string createCMake(const std::filesystem::path& current_dir, const std::string& project_name, const std::string& cpp_version, const std::string& compiler_flags);
void updateCMake(const std::filesystem::path& current_dir, std::string& project_name, std::string& cpp_version, std::string& compiler_flags);
void updateCMake(const std::filesystem::path& current_dir);
void buildProject(const std::filesystem::path& current_dir, const std::string& project_name, const std::string& build_type);
void runProject(const std::filesystem::path& binary_path);
