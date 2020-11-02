#include "wbpch.h"
#include "WindowsUtility.h"

namespace Workbench{
	std::wstring s2ws(const std::string & s)
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

	std::string ws2s(const std::wstring & s) {
		size_t size = WideCharToMultiByte(CP_UTF8, WC_ERR_INVALID_CHARS, &s[0], (int)s.size(), NULL, 0, NULL, NULL);
		std::string str = std::string(size, 0);
		WideCharToMultiByte(CP_UTF8, WC_ERR_INVALID_CHARS, &s[0], (int)s.size(), &str[0], (int)str.size(), NULL, NULL);
		return str;
	}

	WB_KEYCODES WinToWbKeycode(WPARAM code) {
		if (code >= (uint32_t)WB_KEYCODES::WB_VK_A && code <= (uint32_t)WB_KEYCODES::WB_VK_Z) {			//charecters
			return (WB_KEYCODES)code;
		}
		else if (code >= (uint32_t)WB_KEYCODES::WB_VK_0 && code <= (uint32_t)WB_KEYCODES::WB_VK_9) {	//numbers
			return (WB_KEYCODES)code;
		}
		else if (code == VK_RETURN)
			return WB_KEYCODES::WB_VK_RETURN;
		else if (code == VK_SPACE)
			return WB_KEYCODES::WB_VK_SPACE;
		else if (code == VK_LBUTTON)
			return WB_KEYCODES::LMB;
		else if (code == VK_MBUTTON)
			return WB_KEYCODES::MMB;
		else if (code == VK_RBUTTON)
			return WB_KEYCODES::RMB;
		else
			return WB_KEYCODES::None;
	};

	uint32_t WbToWinKeycode(WB_KEYCODES code) {
		return 1;
	};
}
