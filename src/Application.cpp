#include "BetterPrntScreen.h"
#include <filesystem>

namespace BPS = ::BetterPrntScreen;

//Defined on the stack, per system based.
std::shared_ptr<BPS::ISystem> SystemCore;
std::shared_ptr<BPS::INotifyIcon> NotificationIcon;
bool normalSS = false;

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
		if (SystemCore->IsKeyPressed(VK_SNAPSHOT)) {
			normalSS = true;
			while (SystemCore->IsKeyDown(VK_SNAPSHOT)) {
				if (SystemCore->IsKeyPressed(VK_LBUTTON)) {
					GeneralLog("Click SS cycle started!")
					BPS::Point p1 = SystemCore->GetCursorPosition();
					GeneralLog("Clicked once")
					Sleep(150);
					SystemCore->AwaitMousePress();
					BPS::Point p2 = SystemCore->GetCursorPosition();
					GeneralLog("Clicked twice")
					std::string path = SystemCore->CaptureSnapShotBetween(p1, p2);
					Network::uploadFileToServer(path.c_str());
					normalSS = false;
					break;
				}
			}
		}
		if (normalSS) {
			normalSS = false;
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
