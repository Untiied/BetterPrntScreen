#include "Utilities.h"
#include "Network.h"
#include "Logger.h"
#include <time.h>


std::string Utilities::currentDateTime() {
	time_t     now = time(0);
	struct tm  tstruct;
	char       buf[80];
	tstruct = *localtime(&now);
	//http://en.cppreference.com/w/cpp/chrono/c/strftime
	strftime(buf, sizeof(buf), "%Y-%m-%d-%I-%S", &tstruct);

	return buf;
}
