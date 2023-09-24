#pragma once
#include <cstring>
#include "Entity.h"
#include <fstream>

namespace helper {
	bool isUpper(char a);
	bool isLower(char a);
	bool isDigit(char a);
	size_t getFileSize(std::ifstream& file);
	bool isEOF(std::ifstream& file);
	bool isEOF(std::fstream& file);
	bool isEOF(std::ofstream& file);
}