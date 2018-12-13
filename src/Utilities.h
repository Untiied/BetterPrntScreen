#pragma once
#include "ISystem.h"

class Utilities {
public:
	//Retrives the date/time in 'Year/Month/Day/Hour/Sec' format.
	static std::string currentDateTime();

	//Converts the recieved version strings to a float for comparison.
	static float versionToFloat(std::string version);
};
