#pragma once
#include <tuple>
#include <vector>
#include <string>
#include <unordered_map>
#include <windows.h>
#include <iostream>
#include <Psapi.h>
namespace ClickMapper {
	bool IsProcessVisible(HWND hWnd)
	{
		LONG lStyle = GetWindowLongPtrW(hWnd, GWL_STYLE);

		if ((lStyle & WS_VISIBLE) && (lStyle & WS_SYSMENU) && IsWindowVisible(hWnd))
		{
			return true;
		}

		return false;
	}

	BOOL CALLBACK EnumWindowsProc(HWND hWnd, LPARAM lParam)
	{
		std::vector<std::pair<std::string, HWND>>* WindowTitles = (std::vector<std::pair<std::string, HWND>>*) lParam;
		if (!IsProcessVisible(hWnd))
		{
			return TRUE;
		}

		int length = GetWindowTextLengthA(hWnd);
		if (length != 0)
		{
			//GetWindowTextLengthA seems to return one less then the actual length of the text.
			length += 1;
			std::string WindowTitle(length, '\0');
			length = GetWindowTextA(hWnd, &WindowTitle[0], length);
			WindowTitle.resize(length);
			WindowTitles->push_back(std::make_pair(WindowTitle, hWnd));
		}
		return TRUE;
	}

	std::vector<std::pair<std::string, HWND>> GetProcesses() {
		std::vector<std::pair<std::string, HWND>> Windows;
		if (EnumWindows(EnumWindowsProc, (LPARAM)&Windows)) {
			return Windows;
		}
		return Windows;
	}
}
