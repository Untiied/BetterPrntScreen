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

			bool RegisterForStartup();

			std::string CaptureSnapShot();

			std::string CaptureSnapShotBetween(Point BoundOne, Point BoundTwo);

			Point GetCursorPosition();

			bool IsKeyPressed(const int keyCode);

			bool IsKeyDown(const int keyCode);

			bool IsKeyReleased(const int keyCode);

			void AwaitMousePress();

			bool AwaitKeyRelease(const int keyCode);

		private:
			CLSID EncoderClsid;
			ULONG_PTR GdiPlusToken;
			RECT GetActiveDesktopWindow();
			Gdiplus::GdiplusStartupInput GdiPlusStartupInput;
			const std::string ScreenshotsAppdataFolder = GetAppDataPath() + ApplicationNamePathed + "screenshots/";
		};
	}
}