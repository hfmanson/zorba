#include <filesystem>
#include <string>

std::string getCurrentDirectoryFileURL()
{
    namespace fs = std::filesystem;

    fs::path cwd = fs::current_path();

    // Maak een absolute path → file:// URL
    std::string url = "file:///" + cwd.generic_string();

    // Windows: drive letter moet URL‑escaped worden
    // C: → C%3A
#ifdef _WIN32
    if (url.size() > 8 && url[8] == ':') {
        url.replace(8, 1, "%3A");
    }
#endif

    return url + "/";
}
