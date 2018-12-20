#pragma once
#include "INotifyIcon.h"
#include <Windows.h>
#include <ShlObj.h>
#define APPWM_ICONNOTIFY (WM_APP + 1)
#include <shellapi.h>
#include <tchar.h>

namespace BetterPrntScreen
{
	namespace Windows
	{
		class BPSWindowsNotifyIcon : public INotifyIcon {
		public:
			BPSWindowsNotifyIcon();
			~BPSWindowsNotifyIcon();
		public:
			void Init();
			void Cycle();
			bool SendToNotificationArea();
			bool RemoveFromNotificationArea();

			//Win api needs to call this. Not class specific.
			static void APIENTRY DeployPopupMenu();
		};
	}
}