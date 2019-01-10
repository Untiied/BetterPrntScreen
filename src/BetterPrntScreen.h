//Curl
#include <curl/curl.h>

//Standard
#include <iostream>
#include <string>
#include <thread>

//BPS
#include "ISystem.h"
#include "INotifyIcon.h"
#include "Logger.h"
#include "Utilities.h"
#include "Network.h"
#include "Updater.h"

#ifdef _WIN32
#include "BPSWindows.h"
#include "BPSWindowsNotifyIcon.h"
#include "WindowsProc.h"
#endif