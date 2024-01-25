#include "KeyManager.hpp"
#include <iostream>
#include <thread>

using namespace ClickMapper;

void CreateInput(INPUT& input, DWORD type, DWORD dwFlag)
{
	input.type = type;
	input.mi.dwFlags = dwFlag;
	SendInput(1, &input, sizeof(INPUT));
	::ZeroMemory(&input, sizeof(INPUT));
}

static KeyManager* g_instance = nullptr;

KeyManager::KeyType ClickMapper::KeyManager::GetKeyType(int VK)
{
	if ((VK >= (int)'A' && VK <= (int)'Z') || (VK >= 0x30 && VK <= 0x39)) {
		return KeyType::KEYBOARD;
	}

	if (VK == 0x01 || VK == 0x02 || VK == 0x04) {
		return KeyType::MOUSE;
	}

	return KeyType::UNKNOWN;
}

const char* ClickMapper::KeyManager::GetNameFromVK(int VK)
{
	if ((VK >= (int)'A' && VK <= 'Z') || (VK >= 0x30 && VK <= 0x39)) {
		return (char*)&VK;
	}

	const char* returnChar;
	switch (VK) {
	case 0x01:
		returnChar = "Left Mouse Button";
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

KeyManager& ClickMapper::KeyManager::Get()
{
	return *g_instance;
}

LRESULT CALLBACK ClickMapper::KeyManager::HandleKeyPress(int nCode, WPARAM wParam, LPARAM lParam)
{
	if (nCode >= 0 && (wParam == WM_KEYDOWN || wParam == WM_SYSKEYDOWN))
	{
		KBDLLHOOKSTRUCT* pKeyStruct = (KBDLLHOOKSTRUCT*)lParam;
		KeyManager* keyManager = g_instance;
		if (keyManager && keyManager->m_currentProcess)
		{
			if (*keyManager->m_currentProcess == GetForegroundWindow() && !keyManager->GetKeys().empty())
			{
				if (auto keyEntry = keyManager->GetKeys().find(pKeyStruct->vkCode); keyEntry != keyManager->GetKeys().end())
				{
					INPUT input = { 0 };
					switch (keyEntry->second) {
					case 0x01:
					case 0x02:
					case 0x04:
						int dwFlag = 0;
						int dwReleaseFlag = 0;

						switch (keyEntry->second)
						{
						case 0x01:
							dwFlag = MOUSEEVENTF_LEFTDOWN;
							dwReleaseFlag = MOUSEEVENTF_LEFTUP;
							break;
						case 0x02:
							dwFlag == MOUSEEVENTF_RIGHTDOWN;
							dwReleaseFlag = MOUSEEVENTF_RIGHTUP;
							break;
						case 0x04:
							dwFlag = MOUSEEVENTF_WHEEL;
							dwReleaseFlag = MOUSEEVENTF_WHEEL;
							break;
						}

						CreateInput(input, INPUT_MOUSE, dwFlag);
						CreateInput(input, INPUT_MOUSE, dwReleaseFlag);
						return 1;
					}

					KEYBDINPUT kb = { 0 };
					kb.wVk = keyEntry->second;
					input.ki = kb;
					CreateInput(input, INPUT_KEYBOARD, NULL);
					kb.dwFlags = KEYEVENTF_KEYUP;
					kb.wVk = keyEntry->second;
					input.ki = kb;
					CreateInput(input, INPUT_KEYBOARD, NULL);
					return 1;
				}

			}
		}
	}

	return CallNextHookEx(m_hook, nCode, wParam, lParam);
}

void ClickMapper::KeyManager::HandleKey(HWND hwnd, int VK)
{
}

__fastcall ClickMapper::KeyManager::KeyManager(HWND* currentProcess) : m_currentProcess(currentProcess)
{
	if (g_instance) {
		return;
	}
	g_instance = this;
	m_hook = SetWindowsHookEx(WH_KEYBOARD_LL, &HandleKeyPress, NULL, 0);

	std::thread([] {
		MSG msg;
		while (GetMessage(&msg, NULL, 0, 0)) {
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
	}).detach();
}

__fastcall::KeyManager::~KeyManager()
{
	if (m_hook)
	{
		UnhookWindowsHookEx(m_hook);
	}
}

void ClickMapper::KeyManager::RegisterKey(int keyCode, int replacement)
{
	m_registeredKeys.insert(std::make_pair(keyCode, replacement));
}

void ClickMapper::KeyManager::RemoveKey(int keyCode, int replacement)
{
	if (auto keyEntry = m_registeredKeys.find(keyCode); keyEntry != m_registeredKeys.end())
	{
		m_registeredKeys.erase(keyEntry);
	}
}
