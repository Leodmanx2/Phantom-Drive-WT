#include "PSCN.hpp"

#include <cereal/archives/portable_binary.hpp>
#include <cereal/types/string.hpp>
#include <cereal/types/vector.hpp>

void PSCN::File::write(const std::string& filename) {
	std::ofstream                       of(filename, std::ofstream::binary);
	cereal::PortableBinaryOutputArchive oarchive(of);
	oarchive(*this);
}

PSCN::File PSCN::File::parse(std::istream& fileContents) {
	cereal::PortableBinaryInputArchive iarchive(fileContents);
	File                               file;
	iarchive(file);
	return file;
}
