#include "BPSWindows.h"
#include "Logger.h"
#include "Utilities.h"

namespace BetterPrntScreen
{
	namespace Windows
	{
		BPSWindows::BPSWindows()
		{
			CreateDirectory(StringtoWString(std::string(GetAppDataPath() + m_ApplicationNamePathed)).c_str(), NULL) || ERROR_ALREADY_EXISTS == GetLastError();
			CreateDirectory(StringtoWString(std::string(GetAppDataPath() + m_ApplicationNamePathed + "/screenshots/")).c_str(), NULL) || ERROR_ALREADY_EXISTS == GetLastError();
			CreateDirectory(StringtoWString(std::string(GetAppDataPath() + m_ApplicationNamePathed + "/logs/")).c_str(), NULL) || ERROR_ALREADY_EXISTS == GetLastError();
			Gdiplus::GdiplusStartup(&m_GdiPlusToken, &m_GdiPlusStartupInput, NULL);

			for (unsigned int i = 0; i < (int)sizeof(keyCodeHandler); ++i)
			{
				keyCodeHandler[i] = IsKeyPressed(i);
			}
		}

		BPSWindows::~BPSWindows() {
			Gdiplus::GdiplusShutdown(m_GdiPlusToken);
			//DisposeLogFile();
		}

		int GetEncoderClsid(const WCHAR* format, CLSID* pClsid)
		{
			using namespace Gdiplus;
			UINT  num = 0;          // number of image encoders
			UINT  size = 0;         // size of the image encoder array in bytes

			ImageCodecInfo* pImageCodecInfo = NULL;

			GetImageEncodersSize(&num, &size);
			if (size == 0)
				return -1;  // Failure

			pImageCodecInfo = (ImageCodecInfo*)(malloc(size));
			if (pImageCodecInfo == NULL)
				return -1;  // Failure

			GetImageEncoders(num, size, pImageCodecInfo);

			for (UINT j = 0; j < num; ++j)
			{
				if (wcscmp(pImageCodecInfo[j].MimeType, format) == 0)
				{
					*pClsid = pImageCodecInfo[j].Clsid;
					free(pImageCodecInfo);
					return j;  // Success
				}
			}

			free(pImageCodecInfo);
			return -1;  // Failure
		}

		bool BPSWindows::IsKeyDown(const int keyCode)
		{
			return (GetAsyncKeyState(keyCode) & (1 << 16));
		}

		bool BPSWindows::IsKeyPressed(int keyCode)
		{
			bool previousState = keyCodeHandler[keyCode];

			keyCodeHandler[keyCode] = IsKeyDown(keyCode);

			return (keyCodeHandler[keyCode] && !previousState);
		}

		bool BPSWindows::IsKeyReleased(const int keyCode)
		{
			bool previousState = keyCodeHandler[keyCode];

			keyCodeHandler[keyCode] = IsKeyPressed(keyCode);

			return (!keyCodeHandler[keyCode] && previousState);
		}

		void BPSWindows::AwaitMousePress()
		{
			while (!IsKeyPressed(VK_LBUTTON)) {
				Sleep(1);
			}
		}

		bool BPSWindows::AwaitKeyRelease(int keyCode)
		{
			while (IsKeyPressed(keyCode)) {
				Sleep(1);
			}
			return true;
		}

		std::string BPSWindows::CaptureSnapShot()
		{
			bool				isSuccessful = false;
			HDC					hwindowDC, hwindowCompatibleDC;
			int					height, width;
			HBITMAP				hbwindow;
			BITMAPINFOHEADER	bi;
			BITMAPFILEHEADER	bmfHeader;
			BITMAP				bmpScreen;
			RECT				windowsize;

			hwindowDC = GetDC(GetActiveDesktopWindow());
			hwindowCompatibleDC = CreateCompatibleDC(hwindowDC);
			SetStretchBltMode(hwindowCompatibleDC, COLORONCOLOR);

			GetClientRect(GetActiveDesktopWindow(), &windowsize);

			height = windowsize.bottom;
			width = windowsize.right;

			// create a bitmap
			hbwindow = CreateCompatibleBitmap(hwindowDC, width, height);
			GetObject(hbwindow, sizeof(BITMAP), &bmpScreen);

			bi.biSize = sizeof(BITMAPINFOHEADER);    //http://msdn.microsoft.com/en-us/library/windows/window/dd183402%28v=vs.85%29.aspx
			bi.biWidth = bmpScreen.bmWidth;//width;
			bi.biHeight = bmpScreen.bmHeight;//-height;  //this is the line that makes it draw upside down or not
			bi.biPlanes = 1;
			bi.biBitCount = 32;
			bi.biCompression = BI_RGB;
			bi.biSizeImage = 0;
			bi.biXPelsPerMeter = 0;
			bi.biYPelsPerMeter = 0;
			bi.biClrUsed = 0;
			bi.biClrImportant = 0;

			// use the previously created device context with the bitmap
			SelectObject(hwindowCompatibleDC, hbwindow);
			// copy from the window device context to the bitmap device context
			StretchBlt(hwindowCompatibleDC, 0, 0, width, height, hwindowDC, 0, 0, width, height, SRCCOPY); //change SRCCOPY to NOTSRCCOPY for wacky colors !

			DWORD dwBmpSize = ((bmpScreen.bmWidth * bi.biBitCount + 31) / 32) * 4 * bmpScreen.bmHeight;
			std::vector<char> bits(dwBmpSize);
			GetDIBits(hwindowCompatibleDC, hbwindow, 0, bmpScreen.bmHeight, &bits[0], (BITMAPINFO*)&bi, DIB_RGB_COLORS);  //copy from hwindowCompatibleDC to hbwindow

			DeleteObject(hbwindow);
			DeleteDC(hwindowCompatibleDC);
			ReleaseDC(GetActiveDesktopWindow(), hwindowDC);

			// Anything from here on is strictly using the GDI api.
			Gdiplus::Bitmap* screenBitmap = Gdiplus::Bitmap::FromBITMAPINFO((BITMAPINFO*)&bi, &bits[0]);

			CLSID encoderClsid;
			GetEncoderClsid(L"image/png", &encoderClsid);
			std::string pngfilepath = m_ScreenshotsAppdataFolder + "screenshot" + "-" + Utilities::currentDateTime() + ".png";
			screenBitmap->Save(StringtoWString(pngfilepath).c_str(), &encoderClsid);

			return pngfilepath;
		}

		std::string BPSWindows::CaptureSnapShotBetween(Point bound1, Point bound2)
		{
			Point upperBound;
			Point lowerBound;
			if (bound1.y <= bound2.y) {
				upperBound = bound1;
				lowerBound = bound2;
			}
			else {
				upperBound = bound2;
				lowerBound = bound1;
			}


			bool				isSuccessful = false;
			HDC					hwindowDC, hwindowCompatibleDC;
			int					height, width;
			HBITMAP				hbwindow;
			BITMAPINFOHEADER	bi;
			BITMAPFILEHEADER	bmfHeader;
			BITMAP				bmpScreen;
			RECT				windowsize;

			hwindowDC = GetDC(GetActiveDesktopWindow());
			hwindowCompatibleDC = CreateCompatibleDC(hwindowDC);
			SetStretchBltMode(hwindowCompatibleDC, COLORONCOLOR);

			GetClientRect(GetActiveDesktopWindow(), &windowsize);

			height = lowerBound.y - upperBound.y;
			width = lowerBound.x - upperBound.x;

			// create a bitmap
			hbwindow = CreateCompatibleBitmap(hwindowDC, width, height);
			GetObject(hbwindow, sizeof(BITMAP), &bmpScreen);

			bi.biSize = sizeof(BITMAPINFOHEADER);    //http://msdn.microsoft.com/en-us/library/windows/window/dd183402%28v=vs.85%29.aspx
			bi.biWidth = width;
			bi.biHeight = height;  //this is the line that makes it draw upside down or not
			bi.biPlanes = 1;
			bi.biBitCount = 32;
			bi.biCompression = BI_RGB;
			bi.biSizeImage = 0;
			bi.biXPelsPerMeter = 0;
			bi.biYPelsPerMeter = 0;
			bi.biClrUsed = 0;
			bi.biClrImportant = 0;

			// use the previously created device context with the bitmap
			SelectObject(hwindowCompatibleDC, hbwindow);
			// copy from the window device context to the bitmap device context
			StretchBlt(hwindowCompatibleDC, 0, 0, width, height, hwindowDC, upperBound.x, upperBound.y, width, height, SRCCOPY); //change SRCCOPY to NOTSRCCOPY for wacky colors !

			DWORD dwBmpSize = ((width * bi.biBitCount + 31) / 32) * 4 * height;
			std::vector<char> bits(dwBmpSize);
			GetDIBits(hwindowCompatibleDC, hbwindow, 0, height, &bits[0], (BITMAPINFO*)&bi, DIB_RGB_COLORS);  //copy from hwindowCompatibleDC to hbwindow

			DeleteObject(hbwindow);
			DeleteDC(hwindowCompatibleDC);
			ReleaseDC(GetActiveDesktopWindow(), hwindowDC);

			// Anything from here on is strictly using the GDI api.
			Gdiplus::Bitmap* screenBitmap = Gdiplus::Bitmap::FromBITMAPINFO((BITMAPINFO*)&bi, &bits[0]);

			CLSID encoderClsid;
			GetEncoderClsid(L"image/png", &encoderClsid);
			std::string pngfilepath = m_ScreenshotsAppdataFolder + "screenshot" + "-" + Utilities::currentDateTime() + ".png";
			screenBitmap->Save(StringtoWString(pngfilepath).c_str(), &encoderClsid);

			return pngfilepath;
		}

		Point BPSWindows::GetCursorPosition()
		{
			POINT cursorPoint;
			if (GetCursorPos(&cursorPoint))
			{
				return Point(cursorPoint.x, cursorPoint.y);
			}
		}

		std::string BPSWindows::GetAppDataPath()
		{
			TCHAR buffer[MAX_PATH];
			BOOL result = SHGetSpecialFolderPath(GetActiveDesktopWindow(), buffer, CSIDL_LOCAL_APPDATA, false);
			if (!result)
				return std::string();
			std::wstring wide(buffer);
			return std::string(wide.begin(), wide.end());
		}

		std::string BPSWindows::GetWorkingDir()
		{
			const unsigned long maxDir = 260;
			char currentDir[maxDir];
			::GetCurrentDirectoryA(maxDir, currentDir);
			return std::string(currentDir);
		}

		std::wstring BPSWindows::StringtoWString(const std::string& str) {
			int len;
			int slength = (int)str.length() + 1;
			len = MultiByteToWideChar(CP_ACP, 0, str.c_str(), slength, 0, 0);
			wchar_t* buf = new wchar_t[len];
			MultiByteToWideChar(CP_ACP, 0, str.c_str(), slength, buf, len);
			std::wstring r(buf);
			delete[] buf;
			return r;
		};

		void BPSWindows::DisposeLogFile()
		{
			Logger::ReleaseLog(std::string(GetAppDataPath() + m_ApplicationNamePathed + "/logs/").c_str(), Utilities::currentDateTime());
		}

		bool BPSWindows::RegisterForStartup(const char* str)
		{
			HKEY hKey = NULL;
			LONG lResult = 0;
			BOOL fSuccess = TRUE;
			DWORD dwSize;
			PCWSTR args = L"-foobar";

			const size_t count = MAX_PATH * 2;
			wchar_t szValue[count] = {};
			std::wstring pathToExe = StringtoWString(std::string(str));
			wcscpy_s(szValue, count, L"\"");
			wcscat_s(szValue, count, pathToExe.c_str());
			wcscat_s(szValue, count, L"\" ");

			if (args != NULL)
			{
				// caller should make sure "args" is quoted if any single argument has a space
				// e.g. (L"-name \"Mark Voidale\"");
				wcscat_s(szValue, count, args);
			}

			lResult = RegCreateKeyExW(HKEY_CURRENT_USER, L"Software\\Microsoft\\Windows\\CurrentVersion\\Run", 0, NULL, 0, (KEY_WRITE | KEY_READ), NULL, &hKey, NULL);

			fSuccess = (lResult == 0);

			if (fSuccess)
			{
				dwSize = (wcslen(szValue) + 1) * 2;
				lResult = RegSetValueExW(hKey, L"BetterPrntScreen", 0, REG_SZ, (BYTE*)szValue, dwSize);
				fSuccess = (lResult == 0);
			}

			if (hKey != NULL)
			{
				RegCloseKey(hKey);
				hKey = NULL;
			}

			return fSuccess;
		}

		static struct MONITORDATA
		{
			std::vector<RECT> Monitors;
		};

		BOOL CALLBACK MonitorEnum(HMONITOR hMon, HDC hdc, LPRECT lprcMonitor, LPARAM pData)
		{
			MONITORDATA *_MONITORS = reinterpret_cast<MONITORDATA*>(pData);
			_MONITORS->Monitors.push_back(*lprcMonitor);
			return TRUE;
		}

		HWND BPSWindows::GetActiveDesktopWindow()
		{
			MONITORDATA monitorData;

			POINT cursorPoint = {};
			GetCursorPos(&cursorPoint);

			EnumDisplayMonitors(0 ,0, &MonitorEnum, (LPARAM)&monitorData);

			RECT selectedMonitor;
			for (auto &monitor : monitorData.Monitors) {
				if (cursorPoint.x > monitor.left && cursorPoint.x < monitor.right)
				{
					selectedMonitor = monitor;
				}
			}

			return GetDesktopWindow();
		}
	}
}