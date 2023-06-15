#pragma once
#include "MyString.h"
#include "Entity.h"
#include <fstream>

namespace helper {
	bool isUpper(char a);
	bool isLower(char a);
	bool isDigit(char a);
	size_t getFileSize(std::ifstream& file);
	void deleteDataFromFile(std::ifstream& file, int indexStart, int indexEnd);
	void getStartIndexAndEndIndexOfEntityInFile(std::ifstream& file, int& indexStart, int& indexEnd, const Entity& entity);
}