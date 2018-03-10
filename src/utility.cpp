#include "utility.hpp"

#include <plog/Log.h>

using namespace plog;

// pd_consult takes a filename as argument, opens that file, and consults it.
PREDICATE(pd_consult, 1) {
	const std::string fileContents = readFile(static_cast<char*>(A1));
	PlTerm            Stream;

	PlCall("open_string", PlTermv(fileContents.c_str(), Stream));

	PlCompound streamOption("stream", Stream);
	PlTerm     options;
	PlTail     list(options);
	list.append(streamOption);
	list.close();
	PlCall("load_files", PlTermv(A1, options));

	PlCall("close", Stream);
	return true;
}

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

	const int fileSize = static_cast<int>(fileSizeLong);

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
