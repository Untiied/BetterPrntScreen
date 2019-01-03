#include "Utilities.h"
#include "Network.h"
#include "Logger.h"
#include <time.h>
#define ZLIB_WINAPI
#include <zipper/unzipper.h>

std::string Utilities::CurrentDateTime() {
	time_t     now = time(0);
	struct tm  tstruct;
	char       buf[80];
	tstruct = *localtime(&now);
	//http://en.cppreference.com/w/cpp/chrono/c/strftime
	strftime(buf, sizeof(buf), "%Y-%m-%d-%I-%S", &tstruct);

	return buf;
}

bool Utilities::unpackFile(std::string fileName)
{
	using namespace zipper;

	Unzipper Unzipper(fileName);
	bool Success = Unzipper.extract("NEW");
	Unzipper.close();

	return Success;
}
