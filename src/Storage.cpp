#include "Storage.h"

const char* storage::load(AssetType type, const char* id) {
	std::pair<std::string, AssetType> keyPair(std::string(id), type);
	
	// If asset is already in memory, return a pointer to it
	if(activeAssets.find(keyPair) != activeAssets.end())
		return activeAssets.at(keyPair);
	
	// Load file into memory
	char* contents;
	try {
		std::ifstream file(std::string("ass/") + id, std::ios::binary);
		file.seekg(0, file.end);
		std::streampos fileByteSize = file.tellg();
		contents = new char[fileByteSize];
		file.seekg(0, file.beg);
		file.read(&contents[0], fileByteSize);
		file.close();
	}
	catch(int e) {
		delete contents;
		throw e;
	}
	
	// Return a pointer to the asset after adding it to the asset map
	activeAssets.insert({keyPair, contents});
	return contents;
}

void storage::unload(AssetType type, const char* id) {
	std::pair<std::string, AssetType> keyPair(std::string(id), type);
	
	AssetMap::iterator assetIterator = activeAssets.find(keyPair);
	
	delete activeAssets.at(keyPair);
	activeAssets.erase(assetIterator);
}
