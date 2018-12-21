#include "BetterPrntScreen.h"
#include <filesystem>

#include <nana/gui.hpp>
#include <nana/gui/widgets/button.hpp>
#include <nana/gui/widgets/picture.hpp>

namespace BPS = ::BetterPrntScreen;

//Defined on the stack, per system based.
std::shared_ptr<BPS::ISystem> SystemCore;
std::shared_ptr<BPS::INotifyIcon> NotificationIcon;


void deployMenu() {
	using namespace nana;

	BPS::Point mousePoint = SystemCore->GetCursorPosition();
	int windowWidth = 220;
	int windowHeight = 120;

	form fm(rectangle(mousePoint.x - (windowWidth / 2), mousePoint.y - (windowHeight / 2), windowWidth, windowHeight), appear::bald<>());

	button Cord(fm, nana::rectangle(0, 0, 100, 100));
	element::bground cord;
	cord = cord.image(paint::image("A:/cross.bmp"), false, {});
	cord.stretch_parts(4, 4, 4, 4);
	Cord.set_bground(cord);
	Cord.events().click([](const nana::arg_click& ei)
	{
		API::exit();
		SystemCore->AwaitMousePress();
		BPS::Point p1 = SystemCore->GetCursorPosition();
		GeneralLog("Clicked once")
		Sleep(150);
		SystemCore->AwaitMousePress();
		BPS::Point p2 = SystemCore->GetCursorPosition();
		GeneralLog("Clicked twice")
		std::string path = SystemCore->CaptureSnapShotBetween(p1, p2);
		Network::uploadFileToServer(path.c_str());
	});

	button Screen(fm, nana::rectangle(110, 0, 100, 100));
	element::bground screen;
	screen = screen.image(paint::image("A:/tv.bmp"), true, {});
	screen.stretch_parts(4, 4, 4, 4);
	Screen.set_bground(screen);
	Screen.events().click([](const nana::arg_click& ei)
	{
		API::exit();
		std::string path = SystemCore->CaptureSnapShot();
		Network::uploadFileToServer(path.c_str());
	});
	fm.show();
	exec();
}

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
			deployMenu();
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
