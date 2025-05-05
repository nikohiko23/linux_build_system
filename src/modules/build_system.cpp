#include "build_system.hpp"
#include <iostream>
#include <filesystem>
#include <cstdlib>
#include <fstream>
#include "../../include/ipc/socket_client.hpp"
#include <thread>
#include <mutex>
#include <vector>

namespace fs = std::filesystem;

BuildSystem::BuildSystem(const std::string& sourceDir)
    : sourceDirectory(sourceDir), logger("logs/build.log") {}
    
void BuildSystem::build() {
    ipc::SocketClient client("/tmp/build_socket");
    client.sendMessage("Build started");
    logger.log("Build started");
    
    std::mutex mtx;
    std::vector<std::string> cppFiles = getCppFiles();
    std::vector<std::string> objectFiles;
    std::vector<std::thread> threads;
    
    loadCache();
    
    for (const auto& file : cppFiles) {
        if (!needsRebuild(file)) continue;

        threads.emplace_back([&, file]() {
            client.sendMessage("Compiling: " + file);
            logger.log("Compiling: " + file); 
            compileFile(file);

            std::string obj = file.substr(0, file.find_last_of('.')) + ".o";

            std::lock_guard<std::mutex> lock(mtx);
            objectFiles.push_back(obj);
        });
    }
    
    for (auto& t : threads) {
        if (t.joinable()) t.join();
    }
    
    client.sendMessage("Linking files");
    logger.log("Linking started");
    
    linkObjects(objectFiles);
    
    client.sendMessage("Build finished");
    logger.log("Build finished");
    
    saveCache();
}
    
    
    
std::vector<std::string> BuildSystem::getCppFiles() {
    std::vector<std::string> files;

    if (!fs::exists(sourceDirectory) || !fs::is_directory(sourceDirectory)) {
        std::cerr << "Error: Directory " << sourceDirectory << " not found!" << std::endl;
        return files;
    }

    std::cout << "Scan the directory: " << sourceDirectory << std::endl;
    for (const auto& entry : fs::recursive_directory_iterator(sourceDirectory)) {
        if (entry.path().extension() == ".cpp") {
            std::cout << "File found: " << entry.path() << std::endl;
            files.push_back(entry.path().string());
        }
    }
    return files;
}

void BuildSystem::compileFile(const std::string& filepath) {
    std::string objfile = filepath.substr(0, filepath.find_last_of('.')) + ".o";
    std::string command = "g++ -Iinclude -c " + filepath + " -o " + objfile;
    std::cout << "Compilation: " << command << std::endl;
    system(command.c_str());

    
    cache[filepath] = std::filesystem::last_write_time(filepath);
}


void BuildSystem::linkObjects(const std::vector<std::string>& objects) {
    if (objects.empty()) {
        std::cerr << "No object files to link!" << std::endl;
        return;
    }

    std::string command = "g++";
    for (const auto& obj : objects) {
        command += " " + obj;
    }

    std::string outputPath = sourceDirectory + "/my_program";
    command += " -o " + outputPath;

    std::cout << "Linking: " << command << std::endl;
    system(command.c_str());
}

void BuildSystem::loadCache() {
    std::ifstream infile("build/build_cache.txt");
    std::string path;
    std::time_t time;

    while (infile >> path >> time) {
        cache[path] = std::filesystem::file_time_type(std::chrono::seconds(time));
    }
}

void BuildSystem::saveCache() {
    std::ofstream outfile("build/build_cache.txt");
    for (const auto& [path, ftime] : cache) {
        auto sctp = std::chrono::time_point_cast<std::chrono::seconds>(ftime);
        outfile << path << " " << sctp.time_since_epoch().count() << "\n";
    }
}

bool BuildSystem::needsRebuild(const std::string& filepath) {
    auto current_time = std::filesystem::last_write_time(filepath);

    if (cache.find(filepath) == cache.end()) return true;

    return current_time != cache[filepath];
}
