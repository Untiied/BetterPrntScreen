#pragma once
#include <string>

namespace BetterPrntScreen
{

	struct Point
	{
		int x, y;

		Point(){};

		Point(int x, int y)
			: x(x), y(y)
		{}
	};

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
		virtual bool IsKeyPressed(int keycode) = 0;

		virtual bool IsKeyReleased(int keycode) = 0;
		//Needs to return the path to the file just saved.
		virtual std::string CaptureSnapShot() = 0;

		virtual Point GetCursorPosition() = 0;

		virtual std::string GetAppDataPath() = 0;

		virtual std::string GetWorkingDir() = 0;

		virtual bool RegisterForStartup(const char*) = 0;

		//Used to manually force the write of a log file.
		virtual void DisposeLogFile() = 0;

	public:
		//TODO: Move these to the .cpp file.
		static inline bool ShouldShutdown() {
			return m_ShutdownFlag;
		}

		static inline bool ShouldUpdate() {
			return m_UpdateFlag;
		}

		static inline void SetShutdownFlag(bool flag) {
			m_ShutdownFlag = flag;
		}

		static inline void SetUpdateFlag(bool flag) {
			m_UpdateFlag = flag;
		}

		bool keyCodeHandler[101];
		const std::string m_ApplicationName = "BetterPrntScreen";
		const std::string m_ApplicationNamePathed = "/BetterPrntScreen/";
		static const std::string GetClientVersion();
	private:
		static std::string m_ClientVersion;
		static bool m_ShutdownFlag;
		static bool m_UpdateFlag;
	};

}


