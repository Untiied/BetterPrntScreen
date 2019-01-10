#include "WindowsProc.h"
#include "BPSWindowsNotifyIcon.h"
#include "Updater.h"
#include "ISystem.h"
#include "Logger.h"
#include "Network.h"

using namespace BetterPrntScreen::Windows;

ATOM BetterPrntScreen::Windows::WindowsProc::MyRegisterClass(HINSTANCE hInstance)
{
	WNDCLASSEX wcex = {};
	wcex.cbSize = sizeof(WNDCLASSEX);
	wcex.style = CS_HREDRAW | CS_VREDRAW;
	wcex.lpfnWndProc = WndProc;
	wcex.cbClsExtra = 0;
	wcex.cbWndExtra = 0;
	wcex.hInstance = hInstance;
	wcex.hIcon = (HICON)LoadImageW(NULL, L"favicon/favicon.ico", IMAGE_ICON, 0, 0, LR_LOADFROMFILE | LR_SHARED);
	wcex.hCursor = LoadCursor(NULL, IDC_ARROW);
	wcex.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
	wcex.lpszMenuName = NULL;
	wcex.lpszClassName = _T("BetterPrntScreen");
	wcex.hIconSm = (HICON)LoadImageW(NULL, L"favicon/favicon.ico", IMAGE_ICON, 0, 0, LR_LOADFROMFILE | LR_SHARED);
	return RegisterClassEx(&wcex);
}

void BetterPrntScreen::Windows::WindowsProc::Init()
{
	BPSWindowsNotifyIcon::Get()->SendToNotificationArea();

	CycleMessages();
}

//Windows api functions
LRESULT CALLBACK BetterPrntScreen::Windows::WindowsProc::WndProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
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
				BPSWindowsNotifyIcon::DeployPopupMenu();
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
				ISystem::SetShutdownFlag(true);
				break;
			}

			case UPDATE_ID:
			{
				if (Updater::IsUpdateAvaliable()) {
					if (Updater::AttemptUpdateDownload(Network::GetServerClientVersion() + ".zip")) {
						if (Updater::UnpackUpdate("BPSUpdate.zip")) {
							ISystem::SetShutdownFlag(true);
							ISystem::SetUpdateFlag(true);
						}
					}
				}
				break;
			}
			}

			return 0;
		}
	}

	return DefWindowProc(hwnd, uMsg, wParam, lParam);
}

void BetterPrntScreen::Windows::WindowsProc::CycleMessages()
{
	MSG msg;
	do {
		if (GetMessage(&msg, nullptr, 0, 0)) {
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
	} while (!ISystem::ShouldShutdown());
}