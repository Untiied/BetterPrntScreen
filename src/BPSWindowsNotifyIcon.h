#pragma once
#include "INotifyIcon.h"
#include <string>
#include <tchar.h>
#include <Windows.h>
#include <ShlObj.h>
#include <shellapi.h>

namespace BetterPrntScreen
{
	namespace Windows
	{
		class BPSWindowsNotifyIcon : public INotifyIcon {
		public:
			BPSWindowsNotifyIcon();
			~BPSWindowsNotifyIcon();
		public:
			bool SendToNotificationArea();
			bool RemoveFromNotificationArea();

			static std::wstring StringtoWString(std::string str);

			static void APIENTRY DeployPopupMenu();

			static BPSWindowsNotifyIcon* Get();

			static HWND NotifyIconWindow;
		private:
			static BPSWindowsNotifyIcon* BPSWindowsNotifyIconInstance;
		};
	}
}