#pragma once
#include "ISystem.h"

class Utilities {
public:
	//Retrives the date/time in 'Year/Month/Day/Hour/Sec' format.
	static std::string currentDateTime();

	static void unzipUpdate();

	static bool doesFileExist(std::string path);

	static void updateSequence(ISystem& SystemCore, std::string dir);
};
