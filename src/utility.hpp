#ifndef UTILITY_H
#define UTILITY_H

#include "Logger.hpp"
#include <limits>
#include <physfs.h>
#include <stdexcept>
#include <string>
#include <vector>

std::vector<char> readFile(const std::string& filename);

#endif
