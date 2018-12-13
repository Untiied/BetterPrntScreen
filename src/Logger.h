#pragma once
#include <string>
#include <fstream>
#include <iostream>

#define GeneralLog(...) Logger::LogMaster("General", __VA_ARGS__);
#define UpdateLog(...) Logger::LogMaster("Updater", __VA_ARGS__);
#define WindowLog(...) Logger::LogMaster("Window", __VA_ARGS__);
#define NetworkLog(...) Logger::LogMaster("Networking", __VA_ARGS__);
#define SystemLog(...) Logger::LogMaster("System", __VA_ARGS__);

class Logger {
public:
	//Masterstring for the whole log;
	static std::string m_LogString;

	template<typename...Args>
	static void LogMaster(const std::string& logType, const std::string& format, Args&&... args)
	{
		std::string CstrnLogType = "[" + logType + "] ";
		printf("[%s] ", logType.c_str());
		printf(format.c_str(), std::forward<Args>(args)...);
		printf("\n");

		char buff[256];

		snprintf(buff, sizeof(buff), CstrnLogType.c_str());
		m_LogString.append(buff);
		snprintf(buff, sizeof(buff), format.c_str(), std::forward<Args>(args)...);
		m_LogString.append(buff);

		m_LogString.append("\n");
	}

	template<typename...Args>
	static inline void LogMasterList(const std::string& format, Args&&... args)
	{
		printf("\t");
		printf(format.c_str(), std::forward<Args>(args)...);
		printf("\n");

		char buff[256];

		snprintf(buff, sizeof(buff), format.c_str(), std::forward<Args>(args)...);
		m_LogString.append(buff);

		m_LogString.append("\n");
	}

	//Used to log the file into a text file.
	static void ReleaseLog(std::string path, std::string time);
};
