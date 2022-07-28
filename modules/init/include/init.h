#pragma once
#include <filesystem>
#include <fstream>
#include "parser.h"
#include "projectmanager.h"

void createBaseDirectories(const std::filesystem::path& current_dir);
void createMainFile(const std::filesystem::path& current_dir);

void init(const std::filesystem::path& current_dir);