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
			BPSWindows::~BPSWindows() override;

			std::string GetAppDataPath();

			std::string GetWorkingDir();

			std::wstring StringtoWString(const std::string& str);

			void DisposeLogFile();

			std::string CaptureSnapShot();

			std::string CaptureSnapShotBetween(Point bound1, Point bound2);

			Point GetCursorPosition();

			bool IsKeyPressed(const int keyCode);

			bool IsKeyDown(const int keyCode);

			bool IsKeyReleased(const int keyCode);

			void AwaitMousePress();

			bool AwaitKeyRelease(const int keyCode);

			bool RegisterForStartup(const char* str);

		private:
			CLSID EncoderClsid;
			ULONG_PTR GdiPlusToken;
			RECT GetActiveDesktopWindow();
			Gdiplus::GdiplusStartupInput GdiPlusStartupInput;
			const std::string ScreenshotsAppdataFolder = GetAppDataPath() + ApplicationNamePathed + "screenshots/";
		};
	}
}