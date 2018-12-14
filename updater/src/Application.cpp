#include <iostream>
#include <filesystem>
#include <exception>

void print(const char* toPrint) {
	std::cout << toPrint << std::endl;
}

int main(){
	print("Updating BetterPrntScreen! \n");
	//Deletes all the old files in the path.
	for (auto& entry : std::filesystem::directory_iterator(std::filesystem::current_path())) {
		if (entry.path() != std::filesystem::path(std::filesystem::current_path().string() + "/BPSUpdater.exe") && 
			entry.path() != std::filesystem::path(std::filesystem::current_path().string() + "/NEW"))
		{
			if (entry.is_directory())
			{
				for (auto& folderEntry : std::filesystem::directory_iterator(entry.path()))
				{
					std::cout << folderEntry.path() << std::endl;
					std::filesystem::remove(folderEntry.path());
				}
			}

			std::cout << entry.path() << std::endl;
			std::filesystem::remove(entry.path());
		}
	}

	//Copys all the files from the new path to the old path.
	for (auto& entry : std::filesystem::directory_iterator(std::filesystem::path(std::filesystem::current_path().string() + "/NEW/")))
	{
		if (entry.path().filename() != "BPSUpdater.exe")
		{
			if (entry.is_directory())
			{
				print(std::string("Creating: " + std::filesystem::path(std::filesystem::current_path().string() + "/" + entry.path().filename().string()).string()).c_str());
				std::filesystem::create_directory(std::filesystem::path(std::filesystem::current_path().string() + "/" + entry.path().filename().string()));
				for (auto& folderEntry : std::filesystem::directory_iterator(entry.path()))
				{
					std::cout << "Copying:" << folderEntry.path() << std::endl;
					std::filesystem::copy(folderEntry.path(), std::filesystem::path(std::filesystem::current_path().string() + "/" + entry.path().filename().string()));
				}
			}

			std::cout << "Copying:"  << entry.path() << std::endl;
			std::filesystem::copy(entry.path(), std::filesystem::current_path());
		}
	}

	//Renames this file to _BPSUpdater - and takes /NEW/BPSUpdater, copys it to the working dir.
	std::filesystem::rename(std::filesystem::path(std::filesystem::current_path().string() + "/BPSUpdater.exe"), std::filesystem::path(std::filesystem::current_path().string() + "/_BPSUpdater.exe"));
	std::filesystem::copy(std::filesystem::path(std::filesystem::current_path().string() + "/NEW/BPSUpdater.exe"), std::filesystem::current_path());

	//Deletes all files that need to be removed.
	for (auto& entry : std::filesystem::directory_iterator(std::filesystem::path(std::filesystem::current_path().string() + "/NEW"))) 
	{
		if (entry.is_directory())
		{
			for (auto& folderEntry : std::filesystem::directory_iterator(entry.path()))
			{
				std::cout << "Removing: " << folderEntry.path() << std::endl;
				std::filesystem::remove(folderEntry.path());
			}
		}

		std::cout << "Removing: " << entry.path() << std::endl;
		std::filesystem::remove(entry.path());
	}

	//Removes the temp folder
	std::filesystem::remove(std::filesystem::path(std::filesystem::current_path().string() + "/NEW"));

	//Makes sure there wasn't a fault with the installation.
	if (std::filesystem::exists(std::filesystem::path(std::filesystem::current_path().string() + "/BPSUpdater.exe"))) {
		system("start BetterPrntScreen.exe rm_UP");
	}
	
	return -1;
}