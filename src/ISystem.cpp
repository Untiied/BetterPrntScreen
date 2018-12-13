#include "ISystem.h"

bool ISystem::shutdownFlag = false;
std::string ISystem::clientVersion = "0.1.0";

std::string ISystem::getClientVersion() {
	return clientVersion;
}