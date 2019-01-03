#include "BPSWindows.h"
#include "Logger.h"
#include "Utilities.h"

namespace BetterPrntScreen
{
	namespace Windows
	{
		static struct MONITORDATA
		{
			std::vector<RECT> Monitors;
			std::vector<HDC> HDCMonitors;
		};

		//From Windows api documentation.
		static int GetEncoderClsid(const WCHAR* format, CLSID* pClsid)
		{
			using namespace Gdiplus;
			UINT  num = 0;
			UINT  size = 0;

			ImageCodecInfo* pImageCodecInfo = NULL;

			GetImageEncodersSize(&num, &size);
			if (size == 0)
				return -1;

			pImageCodecInfo = (ImageCodecInfo*)(malloc(size));
			if (pImageCodecInfo == NULL)
				return -1;

			GetImageEncoders(num, size, pImageCodecInfo);

			for (UINT j = 0; j < num; ++j)
			{
				if (wcscmp(pImageCodecInfo[j].MimeType, format) == 0)
				{
					*pClsid = pImageCodecInfo[j].Clsid;
					free(pImageCodecInfo);
					return j;
				}
			}

			free(pImageCodecInfo);
			return -1;
		}
		
		BPSWindows::BPSWindows()
		{
			CreateDirectory(StringtoWString(std::string(GetAppDataPath() + ApplicationNamePathed)).c_str(), NULL) || ERROR_ALREADY_EXISTS == GetLastError();
			CreateDirectory(StringtoWString(std::string(GetAppDataPath() + ApplicationNamePathed + "/screenshots/")).c_str(), NULL) || ERROR_ALREADY_EXISTS == GetLastError();
			CreateDirectory(StringtoWString(std::string(GetAppDataPath() + ApplicationNamePathed + "/logs/")).c_str(), NULL) || ERROR_ALREADY_EXISTS == GetLastError();
			Gdiplus::GdiplusStartup(&GdiPlusToken, &GdiPlusStartupInput, NULL);
			GetEncoderClsid(L"image/png", &EncoderClsid);
		}

		BPSWindows::~BPSWindows() {
			Gdiplus::GdiplusShutdown(GdiPlusToken);
			DisposeLogFile();
		}

		bool BPSWindows::IsKeyDown(const int KeyCode)
		{
			return (GetAsyncKeyState(KeyCode) & (1 << 16));
		}

		bool BPSWindows::IsKeyPressed(const int KeyCode)
		{
			bool PreviousState = KeyCodeHandler[KeyCode];

			KeyCodeHandler[KeyCode] = IsKeyDown(KeyCode);

			return (KeyCodeHandler[KeyCode] && !PreviousState);
		}

		bool BPSWindows::IsKeyReleased(const int KeyCode)
		{
			bool PreviousState = KeyCodeHandler[KeyCode];

			KeyCodeHandler[KeyCode] = IsKeyPressed(KeyCode);

			return (!KeyCodeHandler[KeyCode] && PreviousState);
		}

		void BPSWindows::AwaitMousePress()
		{
			while (!IsKeyPressed(VK_LBUTTON)) {
				Sleep(1);
			}
		}

		bool BPSWindows::AwaitKeyRelease(const int KeyCode)
		{
			while (IsKeyPressed(KeyCode)) {
				Sleep(1);
			}

			return true;
		}

		std::string BPSWindows::CaptureSnapShot()
		{
			HDC					HWindowCompatibleDC;
			int					Height, Width;
			HBITMAP				HbWindow;
			BITMAPINFOHEADER	BiInfo;
			BITMAPFILEHEADER	BmfHeader;
			BITMAP				BmpScreen;
			RECT				WindowSize;

			HWindowCompatibleDC = CreateCompatibleDC(GetDC(0));
			SetStretchBltMode(HWindowCompatibleDC, COLORONCOLOR);

			WindowSize = GetActiveDesktopWindow();

			Height = WindowSize.bottom;
			Width = abs(WindowSize.right - WindowSize.left);

			HbWindow = CreateCompatibleBitmap(GetDC(0), Width, Height);
			GetObject(HbWindow, sizeof(BITMAP), &BmpScreen);

			BiInfo.biSize = sizeof(BITMAPINFOHEADER);
			BiInfo.biWidth = BmpScreen.bmWidth;
			BiInfo.biHeight = BmpScreen.bmHeight;
			BiInfo.biPlanes = 1;
			BiInfo.biBitCount = 32;
			BiInfo.biCompression = BI_RGB;
			BiInfo.biSizeImage = 0;
			BiInfo.biXPelsPerMeter = 0;
			BiInfo.biYPelsPerMeter = 0;
			BiInfo.biClrUsed = 0;
			BiInfo.biClrImportant = 0;

			SelectObject(HWindowCompatibleDC, HbWindow);
			StretchBlt(HWindowCompatibleDC, 0, 0, Width, Height, GetDC(0), WindowSize.left, 0, Width, Height, SRCCOPY);

			DWORD DwBmpSize = ((BmpScreen.bmWidth * BiInfo.biBitCount + 31) / 32) * 4 * BmpScreen.bmHeight;
			std::vector<char> Bits(DwBmpSize);
			GetDIBits(HWindowCompatibleDC, HbWindow, 0, BmpScreen.bmHeight, &Bits[0], (BITMAPINFO*)&BiInfo, DIB_RGB_COLORS);

			/* Clean up the window's objects. */
			DeleteObject(HbWindow);
			DeleteDC(HWindowCompatibleDC);

			Gdiplus::Bitmap* ScreenBitmap = Gdiplus::Bitmap::FromBITMAPINFO((BITMAPINFO*)&BiInfo, &Bits[0]);
			std::string ScreenshotFilePath = ScreenshotsAppdataFolder + "screenshot" + "-" + Utilities::CurrentDateTime() + ".png";
			ScreenBitmap->Save(StringtoWString(ScreenshotFilePath).c_str(), &EncoderClsid);
			return ScreenshotFilePath;
		}

		std::string BPSWindows::CaptureSnapShotBetween(Point BoundOne, Point BoundTwo)
		{
			HDC					HWindowCompatibleDC;
			int					Height, Width;
			HBITMAP				HbWindow;
			BITMAPINFOHEADER	BiInfo;
			BITMAPFILEHEADER	BmfHeader;
			BITMAP				BmpScreen;
			RECT				WindowSize;

			Point UpperBound;
			Point LowerBound;

			if (BoundTwo.y < BoundOne.y && BoundTwo.x < BoundOne.x)
			{
				UpperBound = BoundTwo;
				LowerBound = BoundOne;
			}
			else if(BoundTwo.y < BoundOne.y && BoundTwo.x > BoundOne.x) 
			{
				UpperBound.y = BoundTwo.y;
				UpperBound.x = BoundOne.x;

				LowerBound.y = BoundOne.y;
				LowerBound.x = BoundTwo.x;
			}
			else if (BoundTwo.y > BoundOne.y && BoundTwo.x < BoundOne.x)
			{
				UpperBound.y = BoundOne.y;
				UpperBound.x = BoundTwo.x;

				LowerBound.y = BoundTwo.y;
				LowerBound.x = BoundOne.x;
			} 
			else if (BoundTwo.y > BoundOne.y && BoundTwo.x > BoundOne.x)
			{
				UpperBound = BoundOne;
				LowerBound = BoundTwo;
			}

			HWindowCompatibleDC = CreateCompatibleDC(GetDC(0));
			SetStretchBltMode(HWindowCompatibleDC, COLORONCOLOR);

			WindowSize = GetActiveDesktopWindow();
			Height = LowerBound.y - UpperBound.y;
			Width = abs(UpperBound.x - LowerBound.x);

			HbWindow = CreateCompatibleBitmap(GetDC(0), Width, Height);
			GetObject(HbWindow, sizeof(BITMAP), &BmpScreen);

			BiInfo.biSize = sizeof(BITMAPINFOHEADER);
			BiInfo.biWidth = Width;
			BiInfo.biHeight = Height;
			BiInfo.biPlanes = 1;
			BiInfo.biBitCount = 32;
			BiInfo.biCompression = BI_RGB;
			BiInfo.biSizeImage = 0;
			BiInfo.biXPelsPerMeter = 0;
			BiInfo.biYPelsPerMeter = 0;
			BiInfo.biClrUsed = 0;
			BiInfo.biClrImportant = 0;

			SelectObject(HWindowCompatibleDC, HbWindow);

			StretchBlt(HWindowCompatibleDC, 0, 0, Width, Height, GetDC(0), UpperBound.x, UpperBound.y, Width, Height, SRCCOPY);

			DWORD DwBmpSize = ((Width * BiInfo.biBitCount + 31) / 32) * 4 * Height;
			std::vector<char> Bits(DwBmpSize);
			GetDIBits(HWindowCompatibleDC, HbWindow, 0, Height, &Bits[0], (BITMAPINFO*)&BiInfo, DIB_RGB_COLORS);

			/* Clean up the window's objects. */
			DeleteObject(HbWindow);
			DeleteDC(HWindowCompatibleDC);

			Gdiplus::Bitmap* ScreenBitmap = Gdiplus::Bitmap::FromBITMAPINFO((BITMAPINFO*)&BiInfo, &Bits[0]);
			std::string ScreenshotFilePath = ScreenshotsAppdataFolder + "screenshot" + "-" + Utilities::CurrentDateTime() + ".png";
			ScreenBitmap->Save(StringtoWString(ScreenshotFilePath).c_str(), &EncoderClsid);
			return ScreenshotFilePath;
		}

		Point BPSWindows::GetCursorPosition()
		{
			POINT CursorPoint;
			if (GetCursorPos(&CursorPoint))
			{
				return Point(CursorPoint.x, CursorPoint.y);
			}
		}

		std::string BPSWindows::GetAppDataPath()
		{
			TCHAR buffer[MAX_PATH];
			BOOL result = SHGetSpecialFolderPath(GetDesktopWindow(), buffer, CSIDL_LOCAL_APPDATA, false);
			if (!result)
				return std::string();
			std::wstring wide(buffer);
			return std::string(wide.begin(), wide.end());
		}

		std::string BPSWindows::GetWorkingDir()
		{
			const unsigned long maxDir = 260;
			char currentDir[maxDir];
			GetCurrentDirectoryA(maxDir, currentDir);
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
			Logger::ReleaseLog(std::string(GetAppDataPath() + ApplicationNamePathed + "/logs/").c_str(), Utilities::CurrentDateTime());
		}

		bool BPSWindows::RegisterForStartup(const char* Str)
		{
			HKEY hKey = NULL;
			LONG lResult = 0;
			BOOL fSuccess = TRUE;
			DWORD dwSize;
			PCWSTR args = L"-foobar";

			const size_t count = MAX_PATH * 2;
			wchar_t szValue[count] = {};
			std::wstring pathToExe = StringtoWString(std::string(Str));
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

		BOOL CALLBACK MonitorEnum(HMONITOR hMon, HDC hdc, LPRECT lprcMonitor, LPARAM pData)
		{
			MONITORDATA *_MONITORS = reinterpret_cast<MONITORDATA*>(pData);
			_MONITORS->Monitors.push_back(*lprcMonitor);
			_MONITORS->HDCMonitors.push_back(hdc);
			return TRUE;
		}

		RECT BPSWindows::GetActiveDesktopWindow()
		{
			MONITORDATA MonitorData;

			POINT CursorPoint = {};
			GetCursorPos(&CursorPoint);

			EnumDisplayMonitors(0 ,0, &MonitorEnum, (LPARAM)&MonitorData);

			RECT SelectedMonitor;
			for (auto &Monitor : MonitorData.Monitors) {
				if (CursorPoint.x > Monitor.left && CursorPoint.x < Monitor.right)
				{
					SelectedMonitor = Monitor;
				}
			}

			return SelectedMonitor;
		}
	}
}