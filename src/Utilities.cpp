#include "Utilities.h"
#include "Network.h"
#include "Logger.h"
#include <time.h>
#define ZLIB_WINAPI
#include <zipper/unzipper.h>

#ifdef _WIN32
#include <Windows.h>
#endif

std::string Utilities::CurrentDateTime() {
	time_t     now = time(0);
	struct tm  tstruct;
	char       buf[80];
	tstruct = *localtime(&now);
	//http://en.cppreference.com/w/cpp/chrono/c/strftime
	strftime(buf, sizeof(buf), "%Y-%m-%d-%I-%S", &tstruct);

	return buf;
}

std::string Utilities::GetExecutablePath()
{

#ifdef _WIN32
	wchar_t ExeName[MAX_PATH];
	GetModuleFileName(NULL, ExeName, MAX_PATH);
	std::wstring WideStandardString(ExeName);
	return std::string(WideStandardString.begin(), WideStandardString.end());
#endif

	return std::string();
}

bool Utilities::UnpackFile(std::string fileName)
{
	using namespace zipper;

	Unzipper Unzipper(fileName);
	bool Success = Unzipper.extract("NEW");
	Unzipper.close();

	return Success;
}
