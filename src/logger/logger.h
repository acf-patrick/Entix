/**
 * @author acf-patrick (miharisoap@gmail.com)
 *
 * Logger handles logs in the application
 */

#pragma once

#include <termcolor.h>

#include <memory>
#include <sstream>

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

    // dump log content to a file
    static bool dump(const std::string& path);

    static Stream& log(Status);

    // helper for info message
    static Stream& info();

    // helper for warning message
    static Stream& warn();

    // helper for error message
    static Stream& error();
};
