#pragma once
#include <string>
#include <map>

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

	/*
		A system interface that every system will write a class upon.
	*/
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

		//Check for key pressed callback.
		virtual bool IsKeyPressed(const int Keycode) = 0;

		//Check to see if the key is pressed down.
		virtual bool IsKeyDown(const int Keycode) = 0;

		//Check to see if the key is released.
		virtual bool IsKeyReleased(const int keycode) = 0;

		//Stalls the thread until the left mouse button is pressed down.
		virtual void AwaitMousePress() = 0;

		//Stalls the thread until the key is released. 
		virtual bool AwaitKeyRelease(const int keyCode) = 0;

		//Takes a screenshot of the entire screen the mouse cursor is on.
		//Needs to return the path to the file just saved.
		virtual std::string CaptureSnapShot() = 0;

		//Takes a screenshot of the bounding box on the screen that the mouse cursor is on.
		//Needs to return the path to the file just saved.
		virtual std::string CaptureSnapShotBetween(Point PointOne, Point PointTwo) = 0;

		//Returns the current Mouses position.
		virtual Point GetCursorPosition() = 0;

		//Return the ApplicationData path for the specified OS.
		virtual std::string GetAppDataPath() = 0;

		//Returns the Executables file path.
		virtual std::string GetWorkingDir() = 0;

		//Registers the Application to be ran upon operation system startup.
		virtual bool RegisterForStartup(const char* Str) = 0;

		//Used to manually force the write of a log file.
		virtual void DisposeLogFile() = 0;

	public:
		//Returns whether or not the Application should shutdown.
		static bool ShouldShutdown();

		//Returns whether or not the Application should update.
		static bool ShouldUpdate();

		//Sets the ShutdownFlag.
		static void SetShutdownFlag(bool Flag);

		//Sets the UpdateFlag.
		static void SetUpdateFlag(bool Flag);

		std::map<int, bool> KeyCodeHandler;
		const std::string ApplicationName = "BetterPrntScreen";
		const std::string ApplicationNamePathed = "/BetterPrntScreen/";
		static const std::string GetClientVersion();
	private:
		static std::string ClientVersion;
		static bool bShutdownFlag;
		static bool bUpdateFlag;
	};

}


