#include "ISystem.h"

namespace BetterPrntScreen
{

	bool ISystem::m_ShutdownFlag = false;
	bool ISystem::m_UpdateFlag = false;
	std::string ISystem::m_ClientVersion = "0.2.3";

	std::string const ISystem::GetClientVersion() {
		return m_ClientVersion;
	}

}