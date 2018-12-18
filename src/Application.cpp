#include "BetterPrntScreen.h"
#include <filesystem>

namespace BPS = ::BetterPrntScreen;

//Defined on the stack, per system based.
std::shared_ptr<BPS::ISystem> SystemCore;
std::shared_ptr<BPS::INotifyIcon> NotificationIcon;

int main(int argc, char *argv[]) {
	if (argv[1]) {
		std::filesystem::remove(std::filesystem::current_path().string() + "/_BPSUpdater.exe");
	}

	//Starts up the core of the system.
#ifdef _WIN32
	SystemCore = std::make_shared<BPS::Windows::BPSWindows>();
	NotificationIcon = std::make_shared<BPS::Windows::BPSWindowsNotifyIcon>();
#ifdef NDEBUG
	ShowWindow(GetConsoleWindow(), SW_HIDE);
#else
	ShowWindow(GetConsoleWindow(), SW_SHOW);
#endif // !NDBUG
	if (SystemCore->RegisterForStartup(argv[0])) {
		SystemLog("Succesfully added to startup!")
	}
	else {
		SystemLog("failed to add to startup!")
	}
#endif

#ifdef NDEBUG
	if (Updater::isUpdateAvaliable()) {
		std::string newestUpdate = Network::getServerClientVersion() + ".zip";
		if (Updater::attemptUpdateDownload(std::string(newestUpdate))) {
			if (Updater::unpackUpdate("BPSUpdate.zip")) {
				SystemCore->SetShutdownFlag(true);
				SystemCore->SetUpdateFlag(true);
			}
		}
	}
#endif

	std::thread notiThread(&BPS::INotifyIcon::Init, NotificationIcon);

	while (!SystemCore->ShouldShutdown()) {
		if (SystemCore->WasKeyPressed(VK_SNAPSHOT)) {
			std::string path = SystemCore->CaptureSnapShot();
			Network::uploadFileToServer(path.c_str());
		}
		Sleep(1);
	}

	NotificationIcon->RemoveFromNotificationArea();
	notiThread.join();

	SystemCore->DisposeLogFile();

	if (BPS::ISystem::ShouldUpdate()) {
		system("start BPSUpdater.exe");
	}
}