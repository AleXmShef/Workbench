#include "wbpch.h"
#include "WindowsUtility.h"

std::wstring s2ws(const std::string& s)
{
	int len;
	int slength = (int)s.length() + 1;
	len = MultiByteToWideChar(CP_ACP, 0, s.c_str(), slength, 0, 0);
	wchar_t* buf = new wchar_t[len];
	MultiByteToWideChar(CP_ACP, 0, s.c_str(), slength, buf, len);
	std::wstring r(buf);
	delete[] buf;
	return r;
}

std::string ws2s(const std::wstring& s) {
	int size = (int)WideCharToMultiByte(CP_UTF8, WC_ERR_INVALID_CHARS, &s[0], s.size(), NULL, 0, NULL, NULL);
	std::string str = std::string(size, 0);
	WideCharToMultiByte(CP_UTF8, WC_ERR_INVALID_CHARS, &s[0], s.size(), &str[0], str.size(), NULL, NULL);
	return str;
}
