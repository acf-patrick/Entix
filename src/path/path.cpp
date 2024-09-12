#include "path.h"

#include "../application/application.h"

Path::Path(const char* path) {
    if (path) {
        _path = Application::Get().getConfigPath() / std::string(path);
    }
}

Path::Path(const std::string& path) {
    if (path.size() > 0) _path = Application::Get().getConfigPath() / path;
}

Path::Path(const fs::path& path) {
    if (path.string().size() > 0)
        _path = Application::Get().getConfigPath() / path;
}

Path::operator std::string() const { return _path.string(); }

Path::operator fs::path() const { return _path; }

Path Path::operator/(const Path& path) const { return _path / path; }

bool Path::exists() const { return fs::exists(_path); }

std::ostream& operator<<(std::ostream& out, const Path& path) {
    out << (std::string)path;
    return out;
}