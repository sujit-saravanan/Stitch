#include "project_manager.h"
#include "fnv_hash.h"

int main(int argc, char* argv[]) {
        std::vector<std::string_view> args(argv, std::next(argv, static_cast<std::ptrdiff_t>(argc)));
        
        Stitch::ProjectManager manager;
        
        if (argc < 2)
                Stitch::ProjectManager::printHelp("");
        
        switch (FNVHash(args[1])) {
                case "init"_:
                        manager.initializeProject();
                        break;
                
                
                case "run"_: {
                        if (argc < 3)
                                Stitch::ProjectManager::printHelp("CHOOSE 'debug' OR 'release'");
                        switch (FNVHash(args[2])) {
                                case "debug"_:
                                        manager.updateProjectInfo();
                                        manager.createCMakeLists();
                                        manager.build("Debug");
                                        break;
                                case "release"_:
                                        manager.updateProjectInfo();
                                        manager.createCMakeLists();
                                        manager.build("Release");
                                        break;
                                default:
                                        Stitch::ProjectManager::printHelp("CHOOSE 'debug' OR 'release'");
                        }
                        manager.run();
                }
                        break;
                
                
                case "build"_: {
                        if (argc < 3)
                                Stitch::ProjectManager::printHelp("CHOOSE 'debug' OR 'release'");
                        switch (FNVHash(args[2])) {
                                case "debug"_:
                                        manager.updateProjectInfo();
                                        manager.createCMakeLists();
                                        manager.build("Debug");
                                        break;
                                case "release"_:
                                        manager.updateProjectInfo();
                                        manager.createCMakeLists();
                                        manager.build("Release");
                                        break;
                                default:
                                        Stitch::ProjectManager::printHelp("CHOOSE 'debug' OR 'release'");
                        }
                }
                        break;
                
                
                case "create"_: {
                        if (argc < 3 || (args[2] != "vendor" & args[2] != "internal"))
                                Stitch::ProjectManager::printHelp("CHOOSE 'vendor' OR 'internal'");
                        if (argc < 4)
                                Stitch::ProjectManager::printHelp("NO FILES CREATED");
                        
                        manager.createModules(args);
                }
                        break;
                
                
                case "delete"_: {
                        if (argc < 3 || (args[2] != "vendor" & args[2] != "internal"))
                                Stitch::ProjectManager::printHelp("CHOOSE 'vendor' OR 'internal'");
                        if (argc < 4)
                                Stitch::ProjectManager::printHelp("NO FILES CREATED");
                        
                        manager.deleteModules(args);
                }
                        break;
                
                
                case "set"_: {
                        if (argc < 4)
                                Stitch::ProjectManager::printHelp("PASS IN VALUE");
                        switch (FNVHash(args[2])) {
                                case "project-name"_:
                                        manager.updateProjectInfo();
                                        manager.m_project_name = args[3];
                                        manager.createCMakeLists();
                                        break;
                                case "cpp-version"_:
                                        manager.updateProjectInfo();
                                        manager.m_cxx_version = args[3];
                                        manager.createCMakeLists();
                                        break;
                                case "compiler-flags"_:
                                        manager.updateProjectInfo();
                                        manager.m_cxx_flags = args[3];
                                        manager.createCMakeLists();
                                        break;
                                case "export-compiler-commands"_:
                                        manager.updateProjectInfo();
                                        manager.m_export_compile_commands = args[3];
                                        manager.createCMakeLists();
                                        break;
                                default:
                                        Stitch::ProjectManager::printHelp("INVALID VALUE");
                        }
                }
                        break;
                
                
                case "help"_:
                        Stitch::ProjectManager::printHelp("");
                        break;
                
                
                default:
                        Stitch::ProjectManager::printHelp("INVALID COMMAND\n");
                        break;
        }
        return 0;
}