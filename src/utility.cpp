#include "utility.hpp"
#include <limits>
#include <physfs.h>
#include <plog/Log.h>
#include <stdexcept>
#include <vector>

using namespace plog;

std::string readFile(const std::string& filename) {
	LOG(plog::debug) << "readFile -- 1";
	if(!PHYSFS_exists(
	     filename.c_str())) { // FIXME: Crash here; PHYSFD wasn't initialized
		LOG(plog::debug) << "readFile -- 1.1";
		throw std::runtime_error(std::string("Could not find file: ") + filename);
		LOG(plog::debug) << "readFile -- 1.2";
	}

	LOG(plog::debug) << "readFile -- 2";
	PHYSFS_File* shaderFile = PHYSFS_openRead(filename.c_str());
	if(!shaderFile) {
		throw std::runtime_error(std::string("Could not open file: ") + filename);
	}

	LOG(plog::debug) << "readFile -- 3";
	PHYSFS_sint64 fileSizeLong = PHYSFS_fileLength(shaderFile);
	if(fileSizeLong == -1)
		throw std::runtime_error(
		  std::string("Could not determine size of shader: ") + filename);
	if(fileSizeLong > std::numeric_limits<int>::max())
		throw std::runtime_error(std::string("File too large: ") + filename);

	const int fileSize = static_cast<int>(fileSizeLong);

	LOG(plog::debug) << "readFile -- 4";
	std::string buffer;
	buffer.resize(fileSize);
	const int bytesRead =
	  PHYSFS_readBytes(shaderFile, const_cast<char*>(buffer.data()), fileSize);
	PHYSFS_close(shaderFile);
	if(bytesRead < fileSize || bytesRead == -1) {
		LOG(error) << PHYSFS_getErrorByCode(PHYSFS_getLastErrorCode());
		throw std::runtime_error(std::string("Could not read all of file: ") +
		                         filename);
	}

	return buffer;
}
