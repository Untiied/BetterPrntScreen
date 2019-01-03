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
		virtual ~ISystem() {};

		/*
			Each system type need to include these overrides based on the system specifications, and api's.

			Specifications for each system -
			1. Root folder path.
			2. screenshots path under root folder.
			3. logs path under root folder.
		*/

		//Check for key pressed callback
		virtual bool IsKeyPressed(const int Keycode) = 0;

		virtual bool IsKeyDown(const int Keycode) = 0;

		virtual bool IsKeyReleased(const int keycode) = 0;

		virtual void AwaitMousePress() = 0;

		virtual bool AwaitKeyRelease(const int keyCode) = 0;
		//Needs to return the path to the file just saved.
		virtual std::string CaptureSnapShot() = 0;

		virtual std::string CaptureSnapShotBetween(Point PointOne, Point PointTwo) = 0;

		virtual Point GetCursorPosition() = 0;

		virtual std::string GetAppDataPath() = 0;

		virtual std::string GetWorkingDir() = 0;

		virtual bool RegisterForStartup(const char*) = 0;

		//Used to manually force the write of a log file.
		virtual void DisposeLogFile() = 0;

	public:
		static bool ShouldShutdown();

		static bool ShouldUpdate();

		static void SetShutdownFlag(bool Flag);

		static void SetUpdateFlag(bool Flag);

		bool KeyCodeHandler[101];
		const std::string ApplicationName = "BetterPrntScreen";
		const std::string ApplicationNamePathed = "/BetterPrntScreen/";
		static const std::string GetClientVersion();
	private:
		static std::string ClientVersion;
		static bool ShutdownFlag;
		static bool UpdateFlag;
	};

}


