#pragma once

class Updater {
public:
	
	//Function will grab the VERSION of the client from the server.
	//Then compares it to the current client version.
	//If the clients version is < the servers then it will return true.
	static bool isUpdateAvaliable();

	//Function will send a request to the server.
	//Will attempt to download the newest update.
	static bool attemptUpdateDownload(std::string updateName);

	static bool unpackUpdate(std::string updateName);
};