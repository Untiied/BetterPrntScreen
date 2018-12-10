#pragma once
#include <string>

class INotifyIcon {
public:
	virtual void init() = 0;
	virtual void cycle() = 0;
	virtual bool sendToNotificationArea() = 0;
	virtual bool removeFromNotificationArea() = 0;
};