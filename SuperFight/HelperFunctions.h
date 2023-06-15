#pragma once
#include "MyString.h"
#include <fstream>

namespace helper {
	bool isUpper(char a);
	bool isLower(char a);
	bool isDigit(char a);
	size_t getFileSize(std::ifstream& file);
}