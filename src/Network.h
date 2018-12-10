#pragma once
#include <stdio.h>
#include <string>

struct FtpFile {
	const char *filename;
	FILE *stream;
};

class Network {
public:
	static const char* localUpload;
	static const char* outerUpload;	

	static std::string localHostname;
	static std::string outerHostname;

	static std::string getServerClientVersion();

	static void downloadUpdate();

	//During this function it upload the screenshot, and will also save the link to the clipboard.
	static void uploadFileToServer(const char* path);
};