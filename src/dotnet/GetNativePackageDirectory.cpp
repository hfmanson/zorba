#include <windows.h>
#include <string>

std::wstring GetNativePackageDirectory()
{
	wchar_t buffer[MAX_PATH];
	DWORD size = GetModuleFileNameW(NULL, buffer, MAX_PATH);

	if (size == 0 || size == MAX_PATH) {
		return L"";
	}

	std::wstring fullPath(buffer);
	size_t lastSlash = fullPath.find_last_of(L"\\/");
	if (lastSlash != std::wstring::npos) {
		return fullPath.substr(0, lastSlash + 1);
	}

	return L"";
}

std::string WStringToString(const std::wstring& wstr)
{
	if (wstr.empty()) return std::string();

	int sizeNeeded = WideCharToMultiByte(CP_UTF8, 0, &wstr[0], (int)wstr.size(), NULL, 0, NULL, NULL);

	std::string strTo(sizeNeeded, 0);

	WideCharToMultiByte(CP_UTF8, 0, &wstr[0], (int)wstr.size(), &strTo[0], sizeNeeded, NULL, NULL);

	return strTo;
}

std::string GetNativePackageDirectoryA()
{
	return WStringToString(GetNativePackageDirectory());
}
