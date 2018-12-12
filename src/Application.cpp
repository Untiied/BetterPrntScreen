#include "BetterPrntScreen.h"
//Defined on the stack, per system based.
ISystem* SystemCore;
INotifyIcon* NotificationIcon;
Window* SystemWindow;

int main(int argc, char *argv[]) {
	//Starts up the core of the system.
#ifdef _WIN32
	SystemCore = new BPSWindows();
	NotificationIcon = new BPSWindowsNotifyIcon();
#ifdef NDEBUG
	ShowWindow(GetConsoleWindow(), SW_SHOW);
#else
	ShowWindow(GetConsoleWindow(), SW_SHOW);
#endif // !NDBUG
	if (SystemCore->registerForStartup(argv[0])) {
		SystemLog("Succesfully added to startup!")
	}
	else {
		SystemLog("failed to add to startup!")
	}
#endif
	SystemLog("Current client version: %s", ISystem::getClientVersion().c_str())
	//Checks for updates deppending on client/server version.
#ifdef NDEBUG
	Utilities::updateSequence(argv[0]);
#endif

	std::thread notiThread(&INotifyIcon::init, NotificationIcon);

	while (!SystemCore->ShouldShutdown()) {
		if (SystemCore->wasKeyPressed(VK_SNAPSHOT)) {
			std::string path = SystemCore->captureSnapShot();
			Network::uploadFileToServer(path.c_str());
		}
		Sleep(1);
	}

	NotificationIcon->removeFromNotificationArea();
	notiThread.join();

	delete(NotificationIcon);
	delete(SystemCore);
}