#pragma once
#include "parser.h"
#include "modules.h"


void createBaseDirectories(const std::filesystem::path& current_dir);
void createMainFile(const std::filesystem::path& current_dir);

void init(const std::filesystem::path& current_dir);