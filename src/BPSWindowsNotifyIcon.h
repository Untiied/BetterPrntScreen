#pragma once
#include "INotifyIcon.h"
#include <Windows.h>
#include <ShlObj.h>
#define APPWM_ICONNOTIFY (WM_APP + 1)
#include <shellapi.h>
#include <tchar.h>

class BPSWindowsNotifyIcon : public INotifyIcon {
public:
	BPSWindowsNotifyIcon();
	~BPSWindowsNotifyIcon();
public:
	void init();
	void cycle();
	bool sendToNotificationArea();
	bool removeFromNotificationArea();

	//Win api needs to call this. Not class specific.
	static void APIENTRY deployPopupMenu();
};