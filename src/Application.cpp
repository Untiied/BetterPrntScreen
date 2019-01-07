#include "BetterPrntScreen.h"
#include <filesystem>

namespace BPS = ::BetterPrntScreen;

//Defined on the stack, per system based.
std::shared_ptr<BPS::ISystem> SystemCore;
std::shared_ptr<BPS::INotifyIcon> NotificationIcon;
bool bNormalScreenShot = false;

int main(int argc, char *argv[])
{
	// This will only be called an update. It passes the argument 'rm_UP' to the client - calling for the updater exe to be deleted.
	if (argv[1]) {
		std::filesystem::remove(std::filesystem::current_path().string() + "/_BPSUpdater.exe");
	}

	#ifdef _WIN32
		//Starts up the core of the system. For Windows.
		SystemCore = std::make_shared<BPS::Windows::BPSWindows>();
		NotificationIcon = std::make_shared<BPS::Windows::BPSWindowsNotifyIcon>();
	#ifdef NDEBUG
		ShowWindow(GetConsoleWindow(), SW_HIDE);
	#else
		ShowWindow(GetConsoleWindow(), SW_SHOW);
	#endif 
	#endif

	// Asks the SystemCore to register the application for a runtime startup. 
	if (SystemCore->RegisterForStartup(argv[0])) {
		SystemLog("Succesfully added to startup!")
	}
	else {
		SystemLog("failed to add to startup!")
	}

	if (Updater::IsUpdateAvaliable()) {
		if (Updater::AttemptUpdateDownload(Network::GetServerClientVersion() + ".zip")) {
			if (Updater::UnpackUpdate("BPSUpdate.zip")) {
				SystemCore->SetShutdownFlag(true);
				SystemCore->SetUpdateFlag(true);
			}
		}
	}

	//Creates a seperate thread for the Notification Icon to exist. This is because OS polling runs events need to be looped. Taking over the main thread.
	std::thread NotiThread(&BPS::INotifyIcon::Init, NotificationIcon);

	while (!SystemCore->ShouldShutdown())
	{
		if (SystemCore->IsKeyPressed(VK_SNAPSHOT))
		{
			bNormalScreenShot = true;
			while (SystemCore->IsKeyDown(VK_SNAPSHOT))
			{
				if (SystemCore->IsKeyPressed(VK_LBUTTON))
				{
					GeneralLog("Click SS cycle started!")
					BPS::Point p1 = SystemCore->GetCursorPosition();
					GeneralLog("Clicked Point 1 @ Point X: %d & Point Y: %d", p1.x, p1.y)

					// Stalls the thread inorder to have the system catch up intime for the 2nd click... Maybe switch to a polling method?
					std::this_thread::sleep_for(std::chrono::milliseconds(150));

					SystemCore->AwaitMousePress();
					BPS::Point p2 = SystemCore->GetCursorPosition();
					GeneralLog("Clicked Point 2 @ Point X: %d & Point Y: %d", p2.x, p2.y)
					std::string Path = SystemCore->CaptureSnapShotBetween(p1, p2);
					Network::UploadFileToServer(Path.c_str());
					bNormalScreenShot = false;
					break;
				}

				std::this_thread::sleep_for(std::chrono::milliseconds(1));
			}
		}

		if (bNormalScreenShot)
		{
			bNormalScreenShot = false;
			std::string Path = SystemCore->CaptureSnapShot();
			Network::UploadFileToServer(Path.c_str());
		}

		// Sleep is only here to tell the system to relax and not use CPU cycles on our program until we get a key press.
		std::this_thread::sleep_for(std::chrono::milliseconds(1));
	}

	NotiThread.join();
	SystemCore.reset();

	if (BPS::ISystem::ShouldUpdate()) {
		system("start BPSUpdater.exe");
	}
}
