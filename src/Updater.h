#pragma once

class Updater {
public:
	
	//Function will grab the VERSION of the client from the server.
	//Then compares it to the current client version.
	//If the clients version is < the servers then it will return true.
	static bool IsUpdateAvaliable();

	//Function will send a request to the server.
	//Will attempt to download the newest update.
	static bool AttemptUpdateDownload(std::string updateName);

	//Function will upack the zip file the update is contained in.
	//Leaves all new data inside a folder called BPSUpdate.
	static bool UnpackUpdate(std::string updateName);

private:
	//Breaks down each string to major, minor, and patch based on the Semantic Versioniong protocal. 
	//Compares each of the values. If even one of them don't match the server value, an update is forced.
	static bool AreVersionsSimilar(std::string OurClientValue, std::string ServerClientValue);
};