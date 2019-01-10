#include "BPSWindowsNotifyIcon.h"
#include "Utilities.h"
#include "ISystem.h"
#include "Logger.h"
#include "Updater.h"
#include "Network.h"
#include "WindowsProc.h"

namespace BetterPrntScreen
{
	namespace Windows {

		BPSWindowsNotifyIcon* BPSWindowsNotifyIcon::BPSWindowsNotifyIconInstance = 0;
		HWND BPSWindowsNotifyIcon::NotifyIconWindow;

		std::wstring BPSWindowsNotifyIcon::StringtoWString(std::string str) {
			int len;
			int slength = (int)str.length() + 1;
			len = MultiByteToWideChar(CP_ACP, 0, str.c_str(), slength, 0, 0);
			wchar_t* buf = new wchar_t[len];
			MultiByteToWideChar(CP_ACP, 0, str.c_str(), slength, buf, len);
			std::wstring r(buf);
			delete[] buf;
			return r;
		};

		//Class functions
		BPSWindowsNotifyIcon::BPSWindowsNotifyIcon()
		{
			SystemLog("Attempting to register class with windows:  %s", WindowsProc::MyRegisterClass(GetModuleHandle(NULL)) ? "Successful" : "Error")
			NotifyIconWindow = CreateWindow(_T("BetterPrntScreen"), NULL, WS_OVERLAPPEDWINDOW, CW_USEDEFAULT, 0, CW_USEDEFAULT, 0, NULL, NULL, GetModuleHandle(NULL), NULL);
		}

		BPSWindowsNotifyIcon::~BPSWindowsNotifyIcon()
		{
			RemoveFromNotificationArea();
			delete(BPSWindowsNotifyIconInstance);
		}

		void APIENTRY BPSWindowsNotifyIcon::DeployPopupMenu()
		{
			HMENU exitPopupMenu;
			POINT pt;
			GetCursorPos(&pt);

			exitPopupMenu = CreatePopupMenu();

			AppendMenu(exitPopupMenu, MF_STRING, UPDATE_ID, _T("&Check for updates"));
			AppendMenu(exitPopupMenu, MF_STRING, EXIT_ID, _T("&Exit"));

			AppendMenu(exitPopupMenu, MF_SEPARATOR, NULL, NULL);

			std::wstring ClientVersion = StringtoWString("Version: ") + StringtoWString(ISystem::GetClientVersion());
			AppendMenu(exitPopupMenu, MF_STRING | MF_DISABLED, UPDATE_VERSION_ID, ClientVersion.c_str());
			SetForegroundWindow(NotifyIconWindow);

			TrackPopupMenu(exitPopupMenu, TPM_LEFTALIGN | TPM_LEFTBUTTON | TPM_BOTTOMALIGN, pt.x, pt.y, 0, NotifyIconWindow, NULL);
		}

		BPSWindowsNotifyIcon* BPSWindowsNotifyIcon::Get()
		{
			if (BPSWindowsNotifyIconInstance == nullptr)
			{
				BPSWindowsNotifyIconInstance = new BPSWindowsNotifyIcon();
				return BPSWindowsNotifyIconInstance;
			}

			return BPSWindowsNotifyIconInstance;
		}

		bool BPSWindowsNotifyIcon::SendToNotificationArea()
		{
			if (NotifyIconWindow) {
				NOTIFYICONDATAW notifyData = {};
				notifyData.cbSize = sizeof(NOTIFYICONDATAW);
				notifyData.uID = 1001;
				notifyData.hWnd = NotifyIconWindow;
				notifyData.uVersion = NOTIFYICON_VERSION_4;
				notifyData.dwState = NIS_SHAREDICON;
				notifyData.uFlags = NIF_ICON | NIF_MESSAGE | NIF_GUID | NIF_TIP;
				lstrcpyW(notifyData.szTip, _T("BetterPrntScreen"));
				notifyData.uCallbackMessage = APPWM_ICONNOTIFY;
				notifyData.hIcon = (HICON)LoadImageW(NULL, L"favicon/favicon.ico", IMAGE_ICON, 0, 0, LR_LOADFROMFILE | LR_SHARED);
				bool result = Shell_NotifyIcon(NIM_ADD, &notifyData);

				SystemLog("Sending to status area: %s", result ? "Successful" : "Error")

				return result;
			}

			SystemLog("Sending to status area: %s", false ? "Successful" : "Error")
			return false;
		}

		bool BPSWindowsNotifyIcon::RemoveFromNotificationArea()
		{
			if (NotifyIconWindow) {
				NOTIFYICONDATAW notifyData = {};
				notifyData.cbSize = sizeof(NOTIFYICONDATAW);
				notifyData.uID = 1001;
				notifyData.hWnd = NotifyIconWindow;
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
	}
}