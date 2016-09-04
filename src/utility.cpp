#include "utility.hpp"

std::string readFile(const std::string& filename) {
	if(!PHYSFS_exists(filename.c_str())) {
		throw std::runtime_error(std::string("Could not find file: ") + filename);
	}

	PHYSFS_File* shaderFile = PHYSFS_openRead(filename.c_str());
	if(!shaderFile) {
		throw std::runtime_error(std::string("Could not open file: ") + filename);
	}

	PHYSFS_sint64 fileSizeLong = PHYSFS_fileLength(shaderFile);
	if(fileSizeLong == -1)
		throw std::runtime_error(
		  std::string("Could not determine size of shader: ") + filename);
	if(fileSizeLong > std::numeric_limits<int>::max())
		throw std::runtime_error(std::string("File too large: ") + filename);

	int fileSize = static_cast<int>(fileSizeLong);

	std::string buffer;
	buffer.resize(fileSize);
	int bytesRead =
	  PHYSFS_read(shaderFile, const_cast<char*>(buffer.data()), 1, fileSize);
	PHYSFS_close(shaderFile);
	if(bytesRead < fileSize || bytesRead == -1) {
		g_logger->write(Logger::LOG_ERROR, PHYSFS_getLastError());
		throw std::runtime_error(std::string("Could not read all of file: ") +
		                         filename);
	}

	return buffer;
}
