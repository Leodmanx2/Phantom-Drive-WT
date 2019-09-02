#include "pmdl.hpp"

#include <cereal/archives/json.hpp>
#include <cereal/archives/portable_binary.hpp>
#include <cereal/types/vector.hpp>

void PMDL::File::write(const std::string& filename) {
	std::ofstream                       of(filename, std::ofstream::binary);
	cereal::PortableBinaryOutputArchive oarchive(of);
	oarchive(*this);
}

PMDL::File PMDL::File::parse(std::istream& fileContents) {
	cereal::PortableBinaryInputArchive iarchive(fileContents);
	File                               file;
	iarchive(file);
	return file;
}
