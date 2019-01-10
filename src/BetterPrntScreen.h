//Curl
#include <curl/curl.h>

//Standard
#include <filesystem>
#include <iostream>
#include <string>
#include <thread>

//BPS
#include "INotifyIcon.h"
#include "Utilities.h"
#include "ISystem.h"
#include "Logger.h"
#include "Network.h"
#include "Updater.h"

//BPS Windows
#ifdef _WIN32
#include "BPSWindowsNotifyIcon.h"
#include "WindowsProc.h"
#include "BPSWindows.h"
#endif