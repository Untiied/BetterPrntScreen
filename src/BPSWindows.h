#pragma once
#include "ISystem.h"
#include <iostream>
#include <string>
#include <vector>
#ifdef _WIN32
#include <Windows.h>
#include <gdiplus.h>
#include <gdiplusinit.h>
#include <ShlObj.h>
#pragma comment (lib, "GdiPlus.lib")
#endif

namespace BetterPrntScreen
{
	namespace Windows
	{
		class BPSWindows : public ISystem {
		public:
			BPSWindows::BPSWindows();
			BPSWindows::~BPSWindows();

			std::string CaptureSnapShot();

			std::string GetAppDataPath();

			std::string GetWorkingDir();

			std::wstring StringtoWString(const std::string& str);

			void DisposeLogFile();

			bool WasKeyPressed(const int keyCode);

			bool RegisterForStartup(const char* str);

		private:
			ULONG_PTR m_GdiPlusToken;
			HWND m_WindowsDesktopWindow = GetDesktopWindow();
			Gdiplus::GdiplusStartupInput m_GdiPlusStartupInput;
			const std::string m_ScreenshotsAppdataFolder = GetAppDataPath() + m_ApplicationNamePathed + "screenshots/";
		};
	}
}