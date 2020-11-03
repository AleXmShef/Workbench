#pragma once

namespace Workbench {
	enum class WB_KEYCODES {
		None = 0,

		//Mouse buttons
		LMB,
		MMB,
		RMB,

		//Keyboard buttons
		//Numbers
		WB_VK_0 = 48,
		WB_VK_1,
		WB_VK_2,
		WB_VK_3,
		WB_VK_4,
		WB_VK_5,
		WB_VK_6,
		WB_VK_7,
		WB_VK_8,
		WB_VK_9,

		//Alphabet
		WB_VK_A = 65,
		WB_VK_B,
		WB_VK_C,
		WB_VK_D,
		WB_VK_E,
		WB_VK_F,
		WB_VK_G,
		WB_VK_H,
		WB_VK_I,
		WB_VK_J,
		WB_VK_K,
		WB_VK_L,
		WB_VK_M,
		WB_VK_N,
		WB_VK_O,
		WB_VK_P,
		WB_VK_Q,
		WB_VK_R,
		WB_VK_S,
		WB_VK_T,
		WB_VK_U, 
		WB_VK_V,
		WB_VK_W,
		WB_VK_X,
		WB_VK_Y,
		WB_VK_Z,

		WB_VK_RETURN,
		WB_VK_BACKSPACE,
		WB_VK_TAB,
		WB_VK_CAPS,
		WB_VK_ESCAPE,
		WB_VK_SPACE,

		WB_VK_UP,
		WB_VK_DOWN,
		WB_VK_LEFT,
		WB_VK_RIGHT,

		WB_VK_LSHIFT,
		WB_VK_RSHIFT,
		WB_VK_LCTRL,
		WB_VK_RCTRL,
		WB_VK_LALT,
		WB_VK_RALT,

		WB_VK_SUPER,

		WB_VK_F1 = 112,
		WB_VK_F2,
		WB_VK_F3,
		WB_VK_F4,
		WB_VK_F5,
		WB_VK_F6,
		WB_VK_F7,
		WB_VK_F8,
		WB_VK_F9,
		WB_VK_F10,
		WB_VK_F11,
		WB_VK_F12

	};
}