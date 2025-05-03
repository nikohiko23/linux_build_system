#include "build_system.hpp"
#include <iostream>
#include <filesystem>
#include <cstdlib>
#include <fstream>

namespace fs = std::filesystem;

BuildSystem::BuildSystem(const std::string& sourceDir)
    : sourceDirectory(sourceDir) {}
    
void BuildSystem::build() {
    loadCache();
    
    std::vector<std::string> cppFiles = getCppFiles();
    std::vector<std::string> objectFiles;
    
    for (const auto& file : cppFiles) {
        if (needsRebuild(file)) {
            compileFile(file);
            cache[file] = std::filesystem::last_write_time(file);
        } else {
            std::cout << "Skipping (cached): " << file << "\n";
        }
    
        std::string obj = file.substr(0, file.find_last_of('.')) + ".o";
        objectFiles.push_back(obj);
    }
    
    linkObjects(objectFiles);
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
    std::string command = "g++ -I" + sourceDirectory + "/../include -c " + filepath + " -o " + objfile;
    std::cout << "Compilation: " << command << std::endl;
    system(command.c_str());
}

void BuildSystem::linkObjects(const std::vector<std::string>& objects) {
    std::string command = "g++";
    for (const auto& obj : objects) {
        command += " " + obj;
    }
    command += " -o " + sourceDirectory + "/my_program"; // Абсолютный путь для исполнимого файла
    std::cout << "Linking: " << command << std::endl;
    system(command.c_str());
}

void BuildSystem::loadCache() {
    std::ifstream infile("build_cache.txt");
    std::string path;
    std::time_t time;

    while (infile >> path >> time) {
        cache[path] = std::filesystem::file_time_type(std::chrono::seconds(time));
    }
}

void BuildSystem::saveCache() {
    std::ofstream outfile("build_cache.txt");
    for (const auto& [path, ftime] : cache) {
        auto sctp = std::chrono::time_point_cast<std::chrono::seconds>(ftime);
        outfile << path << " " << sctp.time_since_epoch().count() << "\n";
    }
}

bool BuildSystem::needsRebuild(const std::string& filepath) {
    auto current_time = std::filesystem::last_write_time(filepath);

    if (!cache.contains(filepath)) return true;

    return current_time != cache[filepath];
}
