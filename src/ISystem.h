#pragma once
#include <string>

class ISystem {
public:
	/*
		Each system type need to include these overrides based on the system specifications, and api's.

		Specifications for each system -
		1. Root folder path.
		2. screenshots path under root folder.
		3. logs path under root folder.
	*/

	//Check for key pressed callback
	virtual bool wasKeyPressed(int keycode) = 0;

	//Needs to return the path to the file just saved.
	virtual std::string captureSnapShot() = 0;

	virtual std::string getAppDataPath() = 0;

	virtual std::string getWorkingDir() = 0;

	virtual bool registerForStartup(const char*) = 0;
	
	//Used to manually force the write of a log file.
	virtual void disposeLogFile() = 0;

public:
	//TODO: Move these to the .cpp file.
	static inline bool ShouldShutdown() {
		return shutdownFlag;
	}

	static inline bool ShouldUpdate() {
		return updateFlag;
	}

	static inline void setShutdownFlag(bool flag) {
		shutdownFlag = flag;
	}

	static inline void setUpdateFlag(bool flag) {
		updateFlag = flag;
	}

	bool keyCodeHandler[101];
	const std::string applicationName = "BetterPrntScreen";
	const std::string applicationNamePathed = "/BetterPrntScreen/";
	static std::string getClientVersion();
private:
	static std::string clientVersion;
	static bool shutdownFlag;
	static bool updateFlag;
};
