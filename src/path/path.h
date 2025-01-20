/**
 * @author acf-patrick (miharisoap@gmail.com)
 *
 * Handles paths from config file path
 */

#pragma once

#include <filesystem>
#include <ostream>

namespace fs = std::filesystem;

namespace entix {

class Path {
    fs::path _path;

   public:
    Path();
    Path(const char*);
    Path(const std::string&);
    Path(const fs::path&);

    bool exists() const;

    operator std::string() const;

    operator fs::path() const;

    Path operator/(const Path& path) const;
};

}  // namespace entix

std::ostream& operator<<(std::ostream& out, const entix::Path& path);