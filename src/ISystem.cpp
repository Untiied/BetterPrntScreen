#include "ISystem.h"

bool ISystem::shutdownFlag = false;
bool ISystem::updateFlag = false;
std::string ISystem::clientVersion = "0.2.1";

std::string ISystem::getClientVersion() {
	return clientVersion;
}