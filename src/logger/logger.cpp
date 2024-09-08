#include "logger.h"

#include <fstream>

std::shared_ptr<Logger> Logger::instance;

Logger& Logger::Get() {
    if (!instance) instance = std::make_shared<Logger>();
    return *instance;
}

// static
bool Logger::dump(const Path& path) {
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

// static
bool Logger::dumpStatus(Status status, const Path& path) {
    auto& self = Get();

    std::ofstream file(path);
    if (!file) return false;

    std::stringstream ss;
    ss << self.track.str() << self.stream.str();

    std::string line;
    while (std::getline(ss, line)) {
        if (line.empty()) continue;

        auto statusTag = line.substr(1, 4);
        auto lineWithoutStatus = line.substr(7);

        switch (status) {
            case Status::INFO:
                if (statusTag == "INFO") file << lineWithoutStatus << std::endl;
                break;
            case Status::WARN:
                if (statusTag == "WARN") file << lineWithoutStatus << std::endl;
                break;
            case Status::ERROR:
                if (statusTag == "ERRO") file << lineWithoutStatus << std::endl;
                break;
            default:;
        }
    }

    return true;
}