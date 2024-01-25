#pragma once
#include <string>
#include <unordered_map>

#ifdef _WIN32
#include <windows.h>
#endif

namespace ClickMapper {

	static HHOOK m_hook;

	class KeyManager {
	public:
		enum KeyType 
		{
			KEYBOARD,
			MOUSE,
			UNKNOWN
		};
	private:
		KeyType GetKeyType(int VK);


		std::unordered_map<int, int> m_registeredKeys;
	public:

		static KeyManager& Get();

		static LRESULT CALLBACK HandleKeyPress(int nCode, WPARAM wParam, LPARAM lParam);
		void HandleKey(HWND hwnd, int VK);

		__fastcall KeyManager(HWND* currentProcess);
		__fastcall ~KeyManager();

		HWND* m_currentProcess;

		void RegisterKey(int keyCode, int replacement);
		void RemoveKey(int keyCode, int replacement);
		const char* GetNameFromVK(int VK);
		std::unordered_map<int, int>& GetKeys()
		{
			return m_registeredKeys;
		}
	};
}