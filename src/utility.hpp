#ifndef UTILITY_H
#define UTILITY_H

#include "Logger.hpp"
#include <SWI-cpp.h>
#include <limits>
#include <physfs.h>
#include <stdexcept>
#include <string>
#include <vector>

std::string readFile(const std::string& filename);

#endif
