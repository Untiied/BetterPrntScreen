#pragma once
#include <Windows.h>
#include <ShlObj.h>
#include <shellapi.h>

#define EXIT_ID 1000
#define UPDATE_ID 1001
#define UPDATE_VERSION_ID 1002
#define APPWM_ICONNOTIFY (WM_APP + 1)

namespace BetterPrntScreen
{
	namespace Windows 
	{
		class WindowsProc
		{
		public:

			static LRESULT CALLBACK WndProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

			static ATOM MyRegisterClass(HINSTANCE hInstance);

			static void Init();

			static void CycleMessages();
		};

	}
}
