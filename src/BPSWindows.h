#pragma once
#include "ISystem.h"
#include <iostream>
#include <string>
#ifdef _WIN32
#include <Windows.h>
#include <gdiplus.h>
#include <gdiplusinit.h>
#include <ShlObj.h>
#pragma comment (lib, "GdiPlus.lib")
#endif

class BPSWindows : public ISystem {
public:
	BPSWindows::BPSWindows();
	BPSWindows::~BPSWindows();

	bool wasKeyPressed(int keyCode);

	std::string captureSnapShot();

	std::string getAppDataPath();

	void disposeLogFile();

	bool registerForStartup(const char* str);

	inline std::string getWorkingDir()
	{
		const unsigned long maxDir = 260;
		char currentDir[maxDir];
		::GetCurrentDirectoryA(maxDir, currentDir);
		return std::string(currentDir);
	}

	inline std::wstring stringtoWString(const std::string& str) {
		int len;
		int slength = (int)str.length() + 1;
		len = MultiByteToWideChar(CP_ACP, 0, str.c_str(), slength, 0, 0);
		wchar_t* buf = new wchar_t[len];
		MultiByteToWideChar(CP_ACP, 0, str.c_str(), slength, buf, len);
		std::wstring r(buf);
		delete[] buf;
		return r;
	};
private:
	ULONG_PTR gdiplusToken;
	HWND windowsWindow = GetDesktopWindow();
	Gdiplus::GdiplusStartupInput gdiplusStartupInput;
	std::string screenshotsAppdataFolder = getAppDataPath() + applicationNamePathed + "screenshots/";
};