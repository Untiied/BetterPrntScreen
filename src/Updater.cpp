#include "Utilities.h"
#include "Updater.h"
#include "Network.h"
#include "ISystem.h"
#include "Logger.h"

bool Updater::isUpdateAvaliable()
{
	//Used to reduce bandwith.
	std::string serverVersion = Network::getServerClientVersion();
	std::string clientVersion = ISystem::getClientVersion();

	//Converts the strings recived into the float for comparison.
	float serverClientVersion = Utilities::versionToFloat(serverVersion);
	float currentClientVersion = Utilities::versionToFloat(clientVersion);

	if(currentClientVersion < serverClientVersion)
	{
		UpdateLog("New version: %s is available.", serverVersion.c_str())
		return true;
	}

	UpdateLog("Current version: %s is up to-date.", clientVersion.c_str())
	return false;
}

bool Updater::attemptUpdateDownload(std::string updateName)
{
	return Network::downloadNewestUpdate(updateName);
}

bool Updater::unpackUpdate(std::string updateName)
{
	return Utilities::unpackFile(updateName);
}
