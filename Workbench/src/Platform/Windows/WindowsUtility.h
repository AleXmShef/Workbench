#pragma once
#include "Core.h"
#include "Input/Keycodes.h"

namespace Workbench {
	std::wstring s2ws(const std::string& s);

	std::string ws2s(const std::wstring& s);

	WB_KEYCODES WinToWbKeycode(WPARAM code);
	uint32_t WbToWinKeycode(WB_KEYCODES code);
}

