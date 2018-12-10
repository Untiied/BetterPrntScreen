#include "BPSWindows.h"
#include "Logger.h"
#include "Utilities.h"
#include <vector>

BPSWindows::BPSWindows()
{
	CreateDirectory(stringtoWString(std::string(getAppDataPath() + applicationNamePathed)).c_str(), NULL) || ERROR_ALREADY_EXISTS == GetLastError();
	CreateDirectory(stringtoWString(std::string(getAppDataPath() + applicationNamePathed + "/screenshots/")).c_str(), NULL) || ERROR_ALREADY_EXISTS == GetLastError();
	CreateDirectory(stringtoWString(std::string(getAppDataPath() + applicationNamePathed + "/logs/")).c_str(), NULL) || ERROR_ALREADY_EXISTS == GetLastError();
	Gdiplus::GdiplusStartup(&gdiplusToken, &gdiplusStartupInput, NULL);
}

BPSWindows::~BPSWindows() {
	Gdiplus::GdiplusShutdown(gdiplusToken);
	disposeLogFile();
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

bool BPSWindows::wasKeyPressed(int keyCode)
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

std::string BPSWindows::captureSnapShot()
{
	bool				isSuccessful = false;
	HDC					hwindowDC, hwindowCompatibleDC;
	int					height, width;
	HBITMAP				hbwindow;
	BITMAPINFOHEADER	bi;
	BITMAPFILEHEADER	bmfHeader;
	BITMAP				bmpScreen;
	RECT				windowsize;

	hwindowDC = GetDC(windowsWindow);
	hwindowCompatibleDC = CreateCompatibleDC(hwindowDC);
	SetStretchBltMode(hwindowCompatibleDC, COLORONCOLOR);

	GetClientRect(windowsWindow, &windowsize);

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
	ReleaseDC(windowsWindow, hwindowDC);

	// Anything from here on is strictly using the GDI api.
	Gdiplus::Bitmap* screenBitmap = Gdiplus::Bitmap::FromBITMAPINFO((BITMAPINFO*)&bi, &bits[0]);

	CLSID encoderClsid;
	GetEncoderClsid(L"image/png", &encoderClsid);
	std::string pngfilepath = screenshotsAppdataFolder + "screenshot" + "-" + Utilities::currentDateTime() + ".png";
	screenBitmap->Save(stringtoWString(pngfilepath).c_str(), &encoderClsid);

	return pngfilepath;
}

std::string BPSWindows::getAppDataPath()
{
	TCHAR buffer[MAX_PATH];
	BOOL result = SHGetSpecialFolderPath(windowsWindow, buffer, CSIDL_LOCAL_APPDATA, false);
	if (!result)
		return std::string();
	std::wstring wide(buffer);
	return std::string(wide.begin(), wide.end());
}

void BPSWindows::disposeLogFile()
{
	Logger::ReleaseLog(std::string(getAppDataPath() + applicationNamePathed + "/logs/").c_str(), Utilities::currentDateTime());
}

bool BPSWindows::registerForStartup(const char* str)
{
	HKEY hKey = NULL;
	LONG lResult = 0;
	BOOL fSuccess = TRUE;
	DWORD dwSize;
	PCWSTR args = L"-foobar";

	const size_t count = MAX_PATH * 2;
	wchar_t szValue[count] = {};
	std::wstring pathToExe = stringtoWString(std::string(str));
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
