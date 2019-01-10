#pragma once
#include "ISystem.h"

class Utilities {
public:
	//Retrives the date/time in 'Year/Month/Day/Hour/Sec' format.
	static std::string CurrentDateTime();

	static std::string GetExecutablePath();

	//Uses zipper to unpack a zip file into a folder.
	static bool UnpackFile(std::string fileName);
};
