#pragma once
#include <filesystem>
#include <vector>

namespace fs = std::filesystem;

namespace Stitch {
        class ProjectManager {
        public:
                ProjectManager();
        
        public:
                void initializeProject() noexcept;
                
                void obtainProjectInfo() noexcept;
                void updateProjectInfo() noexcept;
                
                void createCMakeLists() noexcept;
                void createMain() noexcept;
                
                static void printHelp(const char *error) noexcept;
                
                void build(std::string_view build_mode) const noexcept;
                void run() const noexcept;
                
                void createModules(const std::vector<std::string_view> &modules) noexcept;
                void deleteModules(const std::vector<std::string_view> &modules) noexcept;
        private:
                void obtainVendorInfo() noexcept;
                void obtainInternalInfo() noexcept;
        
        public:
                fs::path m_project_directory;
                std::string m_cmake_version;
                std::string m_cxx_version;
                std::string m_project_name;
                std::string m_cxx_flags;
                std::string m_export_compile_commands;
                
                FILE* m_file_handler;
                std::string m_vendor_headers;
                std::string m_vendor_directories;
                std::string m_vendor_sources;
                
                std::string m_internal_headers;
                std::string m_internal_directories;
                std::string m_internal_sources;
        };
}

