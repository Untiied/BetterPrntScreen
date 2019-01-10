#include "ISystem.h"
#include "Logger.h"

namespace BetterPrntScreen
{
	bool ISystem::bShutdownFlag = false;
	bool ISystem::bUpdateFlag = false;
	std::string ISystem::ClientVersion = "0.4.5";

	ISystem::ISystem()
	{
	}

	const std::string ISystem::GetClientVersion() {
		return ClientVersion;
	}

	bool ISystem::ShouldShutdown()
	{
		return bShutdownFlag;
	}

	bool ISystem::ShouldUpdate()
	{
		return bUpdateFlag;
	}

	void ISystem::SetShutdownFlag(bool Flag)
	{
		bShutdownFlag = Flag;
	}

	void ISystem::SetUpdateFlag(bool Flag) {
		bUpdateFlag = Flag;
	}
}