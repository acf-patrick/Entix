#include "path.h"

#include "../application/application.h"

namespace entix {

Path::Path() { _path = core::Application::Get().getConfigPath(); }

Path::Path(const char* path) {
    if (path) {
        _path = core::Application::Get().getConfigPath() / std::string(path);
    }
}

Path::Path(const std::string& path) {
    if (path.size() > 0)
        _path = core::Application::Get().getConfigPath() / path;
}

Path::Path(const fs::path& path) {
    if (path.string().size() > 0)
        _path = core::Application::Get().getConfigPath() / path;
}

Path::operator std::string() const { return _path.string(); }

Path::operator fs::path() const { return _path; }

Path Path::operator/(const Path& path) const { return _path / path; }

bool Path::exists() const { return fs::exists(_path); }

}  // namespace entix

std::ostream& operator<<(std::ostream& out, const entix::Path& path) {
    out << (std::string)path;
    return out;
}