#include "ISystem.h"

namespace BetterPrntScreen
{
	bool ISystem::ShutdownFlag = false;
	bool ISystem::UpdateFlag = false;
	std::string ISystem::ClientVersion = "0.4.1";

	const std::string ISystem::GetClientVersion() {
		return ClientVersion;
	}

	bool ISystem::ShouldShutdown()
	{
		return ShutdownFlag;
	}

	bool ISystem::ShouldUpdate()
	{
		return UpdateFlag;
	}

	void ISystem::SetShutdownFlag(bool Flag)
	{
		ShutdownFlag = Flag;
	}

	void ISystem::SetUpdateFlag(bool Flag) {
		UpdateFlag = Flag;
	}
}