#pragma once
#include <tuple>
#include <vector>
#include <string>
#include <unordered_map>
#include <windows.h>
#include <Psapi.h>
namespace ClickMapper {
	namespace Proccess {
		BOOL CALLBACK EnumWindowsProc(HWND hWnd, LPARAM lParam)
		{
			std::vector<std::pair<std::string, HWND>>* WindowTitles = (std::vector<std::pair<std::string, HWND>>*) lParam;
			int length = GetWindowTextLengthA(hWnd);
			std::string WindowTitle(length + 1, '\0');
			length = GetWindowTextA(hWnd, &WindowTitle[0], length);
			WindowTitle.resize(length);
			WindowTitles->push_back(std::make_pair(WindowTitle, hWnd));
			return TRUE;
		}

		std::vector<std::pair<std::string, HWND>> GetProcesses() {
			std::vector<std::pair<std::string, HWND>> Windows;
			if (EnumWindows(EnumWindowsProc, (LPARAM)&Windows)) {
				return Windows;
			}
			return Windows;
		}


		/*std::vector<std::pair<std::string, int>> GetProcesses() {
			DWORD Processes[1024], cbNeeded, cProcesses;
			std::vector<std::pair<std::string, int>> _map;
			if (!EnumProcesses(Processes, sizeof(Processes), &cbNeeded)) {
				return _map;
			}


			cProcesses = cbNeeded / sizeof(DWORD);
			
			for (int i = 0; i < cProcesses; i++) {
				if (Processes[i] != 0) {
					DWORD processID = Processes[i];
					TCHAR szProcessName[MAX_PATH] = TEXT("<unknown>");
					HANDLE hProcess = OpenProcess(PROCESS_QUERY_INFORMATION |
						PROCESS_VM_READ,
						FALSE, processID);
					if (NULL != hProcess) {
						HMODULE hMod;
						DWORD isCbNeeded;
						if (EnumProcessModules(hProcess, &hMod, sizeof(hMod), &isCbNeeded)) {
							GetModuleBaseName(hProcess, hMod, szProcessName, sizeof(szProcessName) / sizeof(TCHAR));
						}
#ifndef UNICODE
						std::string _name;
						_name = szProcessName;
						_map.push_back(std::make_pair(_name, processID));
#else
						std::wstring wStr = szProcessName;
						_map.push_back(std::make_pair(std::string(wStr.begin(), wStr.end()), processID));
#endif
						
					}
				}
			}
			return _map;
		}*/
	}
}
