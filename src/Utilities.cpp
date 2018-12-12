#include "Utilities.h"
#include <ZipLib/ZipFile.h>
#include "Network.h"
#include "Logger.h"
#include <time.h>


std::string Utilities::currentDateTime() {
	time_t     now = time(0);
	struct tm  tstruct;
	char       buf[80];
	tstruct = *localtime(&now);
	//http://en.cppreference.com/w/cpp/chrono/c/strftime
	strftime(buf, sizeof(buf), "%Y-%m-%d-%I-%S", &tstruct);

	return buf;
}

void Utilities::unzipUpdate()
{
	try {
		ZipFile::ExtractFile("BetterPrntScreenUpdate.zip", "BetterPrntScreenPortable/BetterPrntScreen.exe", "BetterPrntScreenUpdated.exe");
	}
	catch (std::exception& e) {
		GeneralLog("An exception occured: '%s'\n", e.what())
	}
}

bool Utilities::doesFileExist(std::string path) {
	if (FILE *file = fopen(path.c_str(), "r")) {
		fclose(file);
		return true;
	}
	else {
		return false;
	}
}

void Utilities::updateSequence(std::string dir)
{
#ifdef LOCALBUILD
	if (Network::getServerClientVersion().find(ISystem::getClientVersion()) != std::string::npos) {
	std::string workingDir;
	SystemLog("Client up-to date!")
	workingDir = "DELETEME";

	if (Utilities::doesFileExist(workingDir)) {
		SystemLog("Trying to delete: %s", workingDir.c_str())
		if (remove(workingDir.c_str()) != 0) {
			SystemLog("Failed to remove the old installation...")
		}
		else {
			SystemLog("Successfully removed the old installation!")
		}
	}
}
else {
	SystemLog("Update ready -- installing!")
	Network::downloadUpdate();
	Utilities::unzipUpdate();
	rename("BetterPrntScreen.exe", "DELETEME");
	rename("BetterPrntScreenUpdated.exe", "BetterPrntScreen.exe");
	system("BetterPrntScreen.exe");
	ISystem::setShutdownFlag(true);
}
#endif


	//if (Network::getServerClientVersion().find(ISystem::getClientVersion()) != std::string::npos) {
	//	std::string workingDir;
	//	SystemLog("Client up-to date!")
	//	#ifndef NDEBUG
	//				workingDir = dir;
	//				SystemLog(workingDir)
	//				workingDir = workingDir.substr(0, workingDir.rfind('\\')) + "\\DELETEME";
	//	#else
	//				workingDir = "DELETEME";
	//	#endif // !NDEBUG

	//	if (Utilities::doesFileExist(workingDir)) {
	//		if (remove(workingDir.c_str()) != 0) {
	//			SystemLog("Failed to remove the old installation...")
	//		}
	//		else {
	//			SystemLog("Successfully removed the old installation!")
	//		};
	//	}
	//}
	//else {
	//	SystemLog("Update ready!")
	//	Network::downloadUpdate();
	//	Utilities::unzipUpdate();
	//	rename("BetterPrntScreen.exe", "DELETEME");
	//	rename("BetterPrntScreenUpdated.exe", "BetterPrntScreen.exe");
	//	system("BetterPrntScreen.exe");
	//	quick_exit(0);
	//}
}
