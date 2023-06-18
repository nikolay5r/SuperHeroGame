#pragma once
#include "MyString.h"
#include "Entity.h"
#include <fstream>

namespace helper {
	bool isUpper(char a);
	bool isLower(char a);
	bool isDigit(char a);
	size_t getFileSize(const MyString& fileName);
	size_t getFileSize(std::ofstream& file);
	size_t getFileSize(std::ifstream& file);
	void deleteDataFromFile(const MyString& fileName, int indexStart, int indexEnd);
	void getStartIndexAndEndIndexOfEntityInFile(const MyString& fileName, int& indexStart, int& indexEnd, const Entity& entity);
	bool isEOF(std::ifstream& file);
	bool isEOF(std::fstream& file);
	bool isEOF(std::ofstream& file);
}