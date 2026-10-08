#include <filesystem>
#include <string>

namespace fs = std::filesystem;

std::string _PathToURL(fs::path path)
{
    std::string url = "file:///" + path.generic_string();

    return url;
}

std::string PathToURL(std::string path)
{
    return _PathToURL(path);
}
