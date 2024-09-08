#include "logger.h"

#include <fstream>

std::shared_ptr<Logger> Logger::instance;

Logger& Logger::Get() {
    if (!instance) instance = std::make_shared<Logger>();
    return *instance;
}

// static
bool Logger::dump(const std::string& path) {
    auto& self = Get();

    std::ofstream file(path);
    if (!file) return false;

    file << self.track.str() << self.stream.str();

    return true;
}

// static
void Logger::endline() {
    auto& self = Get();
    self.stream << std::endl;

    switch (self.curr_line_status) {
        case Status::INFO:
        case Status::WARN:
            std::cout << self.stream.str();
            break;
        case Status::ERROR:
            std::cerr << self.stream.str();
            break;
        default:;
    }

    self.track << self.stream.str();

    // clear stream
    self.stream.str("");
    self.stream.clear();
}