// The Storage class is used to find and load assets into memory, whereupon 
// other parts of the program can make use of them. 

// For the time being, assets are stored as individual files in 
// a generic asset directory. IDs are just the file names.
// One step to take would be to split assets into separate
// directories based on their types. Eventually, it may be worthwhile
// to package everything in a single file.

#ifndef STORAGE_H
#define STORAGE_H

#include <map>
#include <utility>
#include <fstream>
#include <string>

namespace storage {
	enum AssetType {TEXTURE, VERTEX_SHADER, GEOMETRY_SHADER, PIXEL_SHADER, MODEL};
	typedef std::map<std::pair<std::string, AssetType>, const char*> AssetMap;
	
	namespace {
		// This section is for internal use only			
		AssetMap activeAssets;
	}

	const char* load(AssetType type, const char* id);
	void unload(AssetType type, const char* id);
}

#endif
