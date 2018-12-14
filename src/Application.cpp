#include "BetterPrntScreen.h"
#include <filesystem>

//Defined on the stack, per system based.
ISystem* SystemCore;
INotifyIcon* NotificationIcon;
Window* SystemWindow;

int main(int argc, char *argv[]) {
	if (argv[1]) {
		std::filesystem::remove(std::filesystem::current_path().string() + "/_BPSUpdater.exe");
	}

	//Starts up the core of the system.
#ifdef _WIN32
	SystemCore = new BPSWindows();
	NotificationIcon = new BPSWindowsNotifyIcon();
#ifdef NDEBUG
	ShowWindow(GetConsoleWindow(), SW_HIDE);
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

	if (Updater::isUpdateAvaliable()) {
		std::string newestUpdate = Network::getServerClientVersion() + ".zip";
		if (Updater::attemptUpdateDownload(std::string(newestUpdate))) {
			if (Updater::unpackUpdate("BPSUpdate.zip")) {
				SystemCore->setShutdownFlag(true);
				SystemCore->setUpdateFlag(true);
			}
		}
	}

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

	SystemCore->disposeLogFile();

	delete(NotificationIcon);
	delete(SystemCore);

	if (ISystem::ShouldUpdate()) {
		system("start BPSUpdater.exe");
	}
}