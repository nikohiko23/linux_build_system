#ifndef LOGGER_HPP
#define LOGGER_HPP

#include <string>
#include <fstream>
#include <mutex>

class Logger {
public:
    explicit Logger(const std::string& filename = "logs/build.log");
    void log(const std::string& message);

private:
    std::string logfile;
    std::ofstream out;
    std::mutex logMutex;
};

#endif // LOGGER_HPP
