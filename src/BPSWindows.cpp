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

		bool BPSWindows::WasKeyPressed(const int keyCode)
		{
			if (GetKeyState(keyCode) & 0x8000 && keyCodeHandler[keyCode] == false) {
				keyCodeHandler[keyCode] = true;
				return true;
			}
			else if (GetAsyncKeyState(keyCode) == 0) {
				keyCodeHandler[keyCode] = false;
			}
			return false;
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

			hwindowDC = GetDC(m_WindowsDesktopWindow);
			hwindowCompatibleDC = CreateCompatibleDC(hwindowDC);
			SetStretchBltMode(hwindowCompatibleDC, COLORONCOLOR);

			GetClientRect(m_WindowsDesktopWindow, &windowsize);

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
			ReleaseDC(m_WindowsDesktopWindow, hwindowDC);

			// Anything from here on is strictly using the GDI api.
			Gdiplus::Bitmap* screenBitmap = Gdiplus::Bitmap::FromBITMAPINFO((BITMAPINFO*)&bi, &bits[0]);

			CLSID encoderClsid;
			GetEncoderClsid(L"image/png", &encoderClsid);
			std::string pngfilepath = m_ScreenshotsAppdataFolder + "screenshot" + "-" + Utilities::currentDateTime() + ".png";
			screenBitmap->Save(StringtoWString(pngfilepath).c_str(), &encoderClsid);

			return pngfilepath;
		}

		std::string BPSWindows::GetAppDataPath()
		{
			TCHAR buffer[MAX_PATH];
			BOOL result = SHGetSpecialFolderPath(m_WindowsDesktopWindow, buffer, CSIDL_LOCAL_APPDATA, false);
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
	}
}