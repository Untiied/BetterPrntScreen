#pragma once

namespace BetterPrntScreen
{

	class INotifyIcon {
	public:
		virtual ~INotifyIcon() {};
		virtual void Init() {};
		virtual void Cycle() {};
		virtual bool SendToNotificationArea() = 0;
		virtual bool RemoveFromNotificationArea() = 0;
	};

}