#pragma once
#include <string>
//Have no choice but to include the entirty of windows.h due to winnt.h not being self-contained.
#include <windows.h>
namespace ClickMapper {
	
	enum KeyType {
		KEYBOARD = 0,
		MOUSE = 1
	};

	KeyType GetKeyType(int VK) {
		if ((VK >= (int)'A' && VK <= 'Z') || (VK >= 0x30 && VK <= 0x39)) {
			return KeyType::KEYBOARD;
		}
		if (VK == 0x01 || VK == 0x02 || VK == 0x04) {
			return KeyType::MOUSE;
		}
	}


	std::string VKToKeyName(int VK) {
		if ((VK >= (int)'A' && VK <= 'Z') || (VK >= 0x30 && VK <= 0x39)) {
			return (char*)&VK;
		}

		std::string returnChar;
		switch (VK) {
		case 0x01:
			returnChar = "Little Mouse Button";
			break;
		case 0x02:
			returnChar = "Right Mouse Button";
			break;
		case 0x04:
			returnChar = "Middle Mouse Button";
			break;
		}
		return returnChar;
	}

	void HandleKey(HWND hwnd, int VK) {
		INPUT input = { 0 };
		switch (VK) {
		case 0x01:
		case 0x02:
		case 0x04:
			int dwFlag = 0;
			int dwReleaseFlag = 0;
			input.type = INPUT_MOUSE;
			if (VK == 0x01) {
				dwFlag = MOUSEEVENTF_LEFTDOWN;
				dwReleaseFlag = MOUSEEVENTF_LEFTUP;
			}

			if (VK == 0x02) {
				dwFlag == MOUSEEVENTF_RIGHTDOWN;
				dwReleaseFlag = MOUSEEVENTF_RIGHTUP;
			}

			if (VK == 0x04) {
				// Wrong event? MOUSEEVENTF_HWHEEL seems more reasonable, but lacks a release flag? Maybe Windows doesn' check for a release?.
				dwFlag = MOUSEEVENTF_MIDDLEUP;
				dwReleaseFlag = MOUSEEVENTF_MIDDLEDOWN;
			}

			input.mi.dwFlags = dwFlag;
			SendInput(1, &input, sizeof(INPUT));
			// Zero out existing data to prepare for release Flag.
			::ZeroMemory(&input, sizeof(INPUT));
			input.type = INPUT_MOUSE;
			input.mi.dwFlags = dwReleaseFlag;
			SendInput(1, &input, sizeof(INPUT));
			return;
		}
		input.type = INPUT_KEYBOARD;
		KEYBDINPUT kb = { 0 };
		kb.wVk = VK;
		input.ki = kb;
		SendInput(1, &input, sizeof(INPUT));
		// Zero out to for Key Up.
		::ZeroMemory(&input, sizeof(INPUT));
		input.type = INPUT_KEYBOARD;
		kb.dwFlags = KEYEVENTF_KEYUP;
		kb.wVk = VK;
		input.ki = kb;
		SendInput(1, &input, sizeof(INPUT));
	}
}