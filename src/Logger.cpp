#include "Logger.h"
std::string Logger::m_LogString = "";

//Used to log the file into a text file.
void Logger::ReleaseLog(std::string path, std::string time) {
	std::string OutputFileName = path + "BetterPrntScreenLog-" + time + ".txt";
	std::ofstream OutputFile;
	OutputFile.open(OutputFileName);
	OutputFile << m_LogString;
	OutputFile.close();
	m_LogString = "";
}