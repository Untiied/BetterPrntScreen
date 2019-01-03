#include "Network.h"
#include "Logger.h"
#include <curl/curl.h>

const char* Network::outerUpload = "http://betterprntscreen.com/upload";
std::string Network::outerHostname = "http://betterprntscreen.com/";

static size_t WriteCallback(char *contents, size_t size, size_t nmemb, void *userp)
{
	((std::string*)userp)->append((char*)contents, size * nmemb);
	return size * nmemb;
}

static size_t WriteFileCallback(void *buffer, size_t size, size_t nmemb, void *stream)
{
	struct FtpFile *out = (struct FtpFile *)stream;
	if (out && !out->stream) {
		/* open file for writing */
		out->stream = fopen(out->filename, "wb");
		if (!out->stream)
			return -1; /* failure, can't open file to write */
	}
	return fwrite(buffer, size, nmemb, out->stream);
}

std::string Network::GetServerClientVersion() {
	CURL *curlhash = curl_easy_init();
	CURLcode response;
	std::string responseFromServer;

	curl_easy_setopt(curlhash, CURLOPT_URL, std::string(outerHostname + "api/clientVersion").c_str());
	curl_easy_setopt(curlhash, CURLOPT_WRITEFUNCTION, WriteCallback);
	curl_easy_setopt(curlhash, CURLOPT_WRITEDATA, &responseFromServer);
	response = curl_easy_perform(curlhash);

	if (response == CURLE_OK) {
		curl_easy_cleanup(curlhash);
		return responseFromServer.substr(0, responseFromServer.rfind('\n'));
	}
	else {
		NetworkLog("Couldn't receive the clientVersion from server...");
		curl_easy_cleanup(curlhash);
		return nullptr;
	}
}

bool Network::DownloadNewestUpdate(std::string fileName) {
	CURL *curlhash = curl_easy_init();
	CURLcode response;
	struct FtpFile ftpfile = {
	  "BPSUpdate.zip", /* name to store the file as if successful */
	  NULL
	};

	curl_easy_setopt(curlhash, CURLOPT_URL, std::string(outerHostname + "api/download/" + fileName).c_str());
	curl_easy_setopt(curlhash, CURLOPT_WRITEFUNCTION, WriteFileCallback);
	curl_easy_setopt(curlhash, CURLOPT_WRITEDATA, &ftpfile);
	response = curl_easy_perform(curlhash);

	if (response == CURLE_OK && ftpfile.stream != nullptr) {
		NetworkLog("Successfully downloaded newest update!")
			curl_easy_cleanup(curlhash);
		if (ftpfile.stream)
			fclose(ftpfile.stream); /* close the local file */
		return true;
	}
	else {
		NetworkLog("Couldn't receive newest client download from server...")
			curl_easy_cleanup(curlhash);
		if (ftpfile.stream)
			fclose(ftpfile.stream); /* close the local file */
		return false;
	}
}

//During this function it upload the screenshot, and will also save the link to the clipboard.
void Network::UploadFileToServer(const char* path) {
	struct curl_httppost *post = NULL;
	struct curl_httppost *last = NULL;
	CURL *curlhash = curl_easy_init();
	CURLcode response;
	std::string link;

	curl_easy_setopt(curlhash, CURLOPT_URL, outerUpload);
	curl_easy_setopt(curlhash, CURLOPT_WRITEFUNCTION, WriteCallback);
	curl_easy_setopt(curlhash, CURLOPT_WRITEDATA, &link);
	curl_formadd(&post, &last,
		CURLFORM_COPYNAME, "imgupload",
		CURLFORM_FILE, path,
		CURLFORM_END);

	curl_easy_setopt(curlhash, CURLOPT_HTTPPOST, post);

	response = curl_easy_perform(curlhash);

	if (response == CURLE_OK) {
		NetworkLog("Successfully sent image: %s to server!", path)
			link = link.substr(link.find(':') + 1);
		link = outerHostname + link;
		link = link.substr(0, link.rfind("."));
#ifdef _WIN32
		Sleep(500);
		const size_t len = strlen(link.c_str()) + 1;
		HGLOBAL hMem = GlobalAlloc(GMEM_MOVEABLE, len);
		memcpy(GlobalLock(hMem), link.c_str(), len);
		OpenClipboard(0);
		EmptyClipboard();
		SetClipboardData(CF_TEXT, hMem);
		CloseClipboard();
		GeneralLog("Path coppied to clipboard")
#endif
	}
	else {
		NetworkLog("Failed to upload image: %s to server...", path)
	}

	curl_formfree(post);
	curl_easy_cleanup(curlhash);
}

