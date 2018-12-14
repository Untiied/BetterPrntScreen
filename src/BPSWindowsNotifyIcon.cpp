#include "BPSWindowsNotifyIcon.h"
#include "Utilities.h"
#include "ISystem.h"
#include "Logger.h"
#include "Updater.h"
#include "Network.h"

#define EXIT_ID 1000
#define UPDATE_ID 1001

//Dirty ownership & initilization inorder for static members to be able to access it..
HWND hWnd;

//Windows api functions
LRESULT CALLBACK WndProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	{
		switch (uMsg)
		{
		case APPWM_ICONNOTIFY:
		{
			switch (lParam)
			{
			case WM_LBUTTONUP:
				//...
				break;
			case WM_RBUTTONUP:
				BPSWindowsNotifyIcon::deployPopupMenu();
				break;
			}

			return 0;
		}
		case WM_COMMAND:
		{

			switch (wParam)
			{
			case EXIT_ID:
			{
				ISystem::setShutdownFlag(true);
				break;
			}

			case UPDATE_ID:
			{
				if (Updater::isUpdateAvaliable()) {
					std::string newestUpdate = Network::getServerClientVersion() + ".zip";
					if (Updater::attemptUpdateDownload(std::string(newestUpdate))) {
						if (Updater::unpackUpdate("BPSUpdate.zip")) {
							ISystem::setShutdownFlag(true);
							ISystem::setUpdateFlag(true);
						}
					}
				}
				break;
			}
			}

			return 0;
		}
		int i = 0;
		}

	}
	return DefWindowProc(hwnd, uMsg, wParam, lParam);
}

ATOM MyRegisterClass(HINSTANCE hInstance)
{
	WNDCLASSEX wcex = {};
	wcex.cbSize = sizeof(WNDCLASSEX);
	wcex.style = CS_HREDRAW | CS_VREDRAW;
	wcex.lpfnWndProc = WndProc;
	wcex.cbClsExtra = 0;
	wcex.cbWndExtra = 0;
	wcex.hInstance = GetModuleHandle(NULL);
	wcex.hIcon = (HICON)LoadImageW(NULL, L"favicon/favicon.ico", IMAGE_ICON, 0, 0, LR_LOADFROMFILE | LR_SHARED);
	wcex.hCursor = LoadCursor(NULL, IDC_ARROW);
	wcex.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
	wcex.lpszMenuName = NULL;
	wcex.lpszClassName = _T("BetterPrntScreen");
	wcex.hIconSm = (HICON)LoadImageW(NULL, L"favicon/favicon.ico", IMAGE_ICON, 0, 0, LR_LOADFROMFILE | LR_SHARED);
	return RegisterClassEx(&wcex);
}

void APIENTRY BPSWindowsNotifyIcon::deployPopupMenu()
{
	HMENU exitPopupMenu;
	POINT pt;
	GetCursorPos(&pt);

	exitPopupMenu = CreatePopupMenu();

	AppendMenu(exitPopupMenu, MF_STRING, UPDATE_ID, _T("&Check for updates"));
	AppendMenu(exitPopupMenu, MF_STRING, EXIT_ID, _T("&Exit"));

	SetForegroundWindow(hWnd);

	TrackPopupMenu(exitPopupMenu, TPM_LEFTALIGN | TPM_LEFTBUTTON | TPM_BOTTOMALIGN, pt.x, pt.y, 0, hWnd, NULL);
}

//Class functions
BPSWindowsNotifyIcon::BPSWindowsNotifyIcon()
{
}

BPSWindowsNotifyIcon::~BPSWindowsNotifyIcon()
{
	removeFromNotificationArea();
}

void BPSWindowsNotifyIcon::init() {
	SystemLog("Registered class: %s", MyRegisterClass(GetModuleHandle(NULL)) ? "Successful" : "Error")
		SystemLog("Sending to status area: %s", sendToNotificationArea() ? "Successful" : "Error")
		cycle();
}

void BPSWindowsNotifyIcon::cycle()
{
	MSG msg;
	do {
		if (GetMessage(&msg, hWnd, 0, 0)) {
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
	} while (!ISystem::ShouldShutdown());
}

bool BPSWindowsNotifyIcon::sendToNotificationArea()
{
	hWnd = CreateWindow(_T("BetterPrntScreen"), NULL, WS_OVERLAPPEDWINDOW,
		CW_USEDEFAULT, 0, CW_USEDEFAULT, 0, NULL, NULL, GetModuleHandle(NULL), NULL);

	if (hWnd) {
		NOTIFYICONDATAW notifyData = {};
		notifyData.cbSize = sizeof(NOTIFYICONDATAW);
		notifyData.uID = 1001;
		notifyData.hWnd = hWnd;
		notifyData.uVersion = NOTIFYICON_VERSION_4;
		notifyData.dwState = NIS_SHAREDICON;
		notifyData.uFlags = NIF_ICON | NIF_MESSAGE | NIF_GUID | NIF_TIP;
		lstrcpyW(notifyData.szTip, _T("BetterPrntScreen"));
		notifyData.uCallbackMessage = APPWM_ICONNOTIFY;
		notifyData.hIcon = (HICON)LoadImageW(NULL, L"favicon/favicon.ico", IMAGE_ICON, 0, 0, LR_LOADFROMFILE | LR_SHARED);
		bool result = Shell_NotifyIcon(NIM_ADD, &notifyData);

		return result;
	}

	return false;
}

bool BPSWindowsNotifyIcon::removeFromNotificationArea()
{
	if (hWnd) {
		NOTIFYICONDATAW notifyData = {};
		notifyData.cbSize = sizeof(NOTIFYICONDATAW);
		notifyData.uID = 1001;
		notifyData.hWnd = hWnd;
		notifyData.uVersion = NOTIFYICON_VERSION_4;
		notifyData.dwState = NIS_SHAREDICON;
		notifyData.uFlags = NIF_ICON | NIF_MESSAGE | NIF_GUID | NIF_TIP;
		lstrcpyW(notifyData.szTip, _T("BetterPrntScreen"));
		notifyData.uCallbackMessage = APPWM_ICONNOTIFY;
		notifyData.hIcon = (HICON)LoadImageW(NULL, L"favicon/favicon.ico", IMAGE_ICON, 0, 0, LR_LOADFROMFILE | LR_SHARED);
		bool result = Shell_NotifyIcon(NIM_DELETE, &notifyData);
		return result;
	}
}
