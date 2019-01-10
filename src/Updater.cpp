#include "Utilities.h"
#include "Updater.h"
#include "Network.h"
#include "ISystem.h"
#include "Logger.h"

namespace BPS = ::BetterPrntScreen;

bool Updater::IsUpdateAvaliable()
{
#ifdef NDEBUG
	std::string ServerVersion = Network::GetServerClientVersion();
	std::string ClientVersion = BPS::ISystem::GetClientVersion();

	bool Result = AreVersionsSimilar(ClientVersion, ServerVersion);

	if (Result)
	{
		UpdateLog("Current version: %s is up to date!", ClientVersion.c_str())
		return false;
	}

	UpdateLog("Current version: %s isn't up to date!\n	  Newest version is: %s", ClientVersion.c_str(), ServerVersion.c_str())
	return true;
	#else if DEBUG
		UpdateLog("No version checking was done... Must be a development build.")
		return false;
	#endif
}

bool Updater::AttemptUpdateSequence()
{
	if (Updater::AttemptUpdateDownload(Network::GetServerClientVersion() + ".zip")) {
		if (Updater::UnpackUpdate("BPSUpdate.zip")) {
			BetterPrntScreen::ISystem::SetShutdownFlag(true);
			BetterPrntScreen::ISystem::SetUpdateFlag(true);
			return true;
		}
	}

	return false;
}

bool Updater::AttemptUpdateDownload(std::string updateName)
{
	return Network::DownloadNewestUpdate(updateName);
}

bool Updater::UnpackUpdate(std::string updateName)
{
	return Utilities::UnpackFile(updateName);
}

bool Updater::AreVersionsSimilar(std::string OurClientValue, std::string ServerClientValue)
{
	int ClientMajor = std::stoi(OurClientValue.substr(0, OurClientValue.find('.')));
	int ClientMinor = std::stoi(OurClientValue.substr(OurClientValue.find('.') + 1, OurClientValue.find('.', OurClientValue.find('.') + 1) - 1));
	int ClientPatch = std::stoi(OurClientValue.substr(OurClientValue.find('.', OurClientValue.find('.') + 1) + 1, OurClientValue.length()));

	int ServerClientMajor = std::stoi(ServerClientValue.substr(0, ServerClientValue.find('.')));
	int ServerClientMinor = std::stoi(ServerClientValue.substr(ServerClientValue.find('.') + 1, ServerClientValue.find('.', ServerClientValue.find('.') + 1) - 1));
	int ServerClientPatch = std::stoi(ServerClientValue.substr(ServerClientValue.find('.', ServerClientValue.find('.') + 1) + 1, ServerClientValue.length()));

	if (ClientMajor < ServerClientMajor)
	{
		return false;
	}
	else if (ClientMinor < ServerClientMinor)
	{
		return false;
	}
	else if (ClientPatch < ServerClientPatch && ClientMinor <= ServerClientMinor)
	{
		return false;
	}

	return true;
}
