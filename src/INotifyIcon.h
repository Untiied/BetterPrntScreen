#pragma once

namespace BetterPrntScreen
{

	class INotifyIcon {
	public:
		virtual void Init() = 0;
		virtual void Cycle() = 0;
		virtual bool SendToNotificationArea() = 0;
		virtual bool RemoveFromNotificationArea() = 0;
	};

}