#include <windows.h>
#include <algorithm>
#include <string>

std::string PathToURL(std::string path)
{
	std::string url = "file:///" + path;
	std::replace(url.begin(), url.end(), '\\', '/');
	return url;
}
