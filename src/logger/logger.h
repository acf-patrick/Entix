/**
 * @author acf-patrick (miharisoap@gmail.com)
 *
 * Logger handles logs in the application
 */

#pragma once

#include <termcolor.h>

#include <memory>
#include <sstream>

#include "../path/path.h"

namespace entix {

// Interfaces are designed through static methods
class Logger {
   public:
    enum class Status { INFO, WARN, ERROR };

   private:
    using Stream = std::ostringstream;

    static std::shared_ptr<Logger> instance;
    static Logger& Get();

    Stream track;
    Stream stream;
    Status curr_line_status;

   public:
    ~Logger() = default;

    // print messages and end current line
    static void endline();

    // dump log with given status to file
    static bool dumpStatus(Status, const Path&);

    // dump log content to a file
    static bool dump(const Path&);

    template <typename... TArgs>
    static Stream& log(Logger::Status status, TArgs... contexts) {
        auto& self = Get();
        self.curr_line_status = status;

        switch (status) {
            case Status::INFO:
                self.stream << termcolor::green << "[INFO] ";
                break;
            case Status::WARN:
                self.stream << termcolor::yellow << "[WARN] ";
                break;
            case Status::ERROR:
                self.stream << termcolor::red << "[ERROR]";
                break;
            default:;
        }

        ((self.stream << '[' << contexts << ']'), ...);
        self.stream << ' ' << termcolor::reset;

        return self.stream;
    }

    // helper for info message
    template <typename... TArgs>
    static Stream& info(TArgs... contexts) {
        return log(Status::INFO, std::forward<TArgs>(contexts)...);
    }

    // helper for warning message
    template <typename... TArgs>
    static Stream& warn(TArgs... contexts) {
        return log(Status::WARN, std::forward<TArgs>(contexts)...);
    }

    // helper for error message
    template <typename... TArgs>
    static Stream& error(TArgs... contexts) {
        return log(Status::ERROR, std::forward<TArgs>(contexts)...);
    }
};

}