#include "logger.hpp"
#include <iostream>
#include <chrono>
#include <ctime>

Logger::Logger(const std::string& filename)
    : logfile(filename), out(filename, std::ios::app) {
    if (!out.is_open()) {
        std::cerr << "Failed to open log file: " << filename << std::endl;
    }
}

void Logger::log(const std::string& message) {
    std::lock_guard<std::mutex> lock(logMutex);
    auto now = std::chrono::system_clock::now();
    std::time_t nowTime = std::chrono::system_clock::to_time_t(now);
    out << std::ctime(&nowTime) << ": " << message << std::endl;
}
