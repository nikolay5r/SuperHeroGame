#include "HelperFunctions.h"
#include "Regex_Error.h"
#include "File_Error.h"
#include "Constants.h"
#include "User.h"
#include "Player.h"
#include "SuperHero.h"
#include "Admin.h"
#include <fstream>
#include <iostream>

bool helper::isUpper(char a)
{
	return a >= 'A' && a <= 'Z';
}

bool helper::isLower(char a)
{
	return a >= 'a' && a <= 'z';
}

bool helper::isDigit(char a)
{
	return a >= '0' && a <= '9';
}

size_t helper::getFileSize(std::ifstream& file)
{
	size_t curr = file.tellg();
	file.seekg(0, std::ios::end);
	size_t size = file.tellg();
	file.seekg(curr);

	return size;
}

bool helper::isEOF(std::ofstream& file)
{
	size_t index = file.tellp();
	file.seekp(0, std::ios::end);
	bool eof = (index >= file.tellp());
	file.seekp(index);
	return eof;
}

bool helper::isEOF(std::ifstream& file)
{
	size_t index = file.tellg();
	file.seekg(0, std::ios::end);
	bool eof = (index >= file.tellg());
	file.seekg(index);
	return eof;
}

bool helper::isEOF(std::fstream& file)
{
	size_t index = file.tellg();
	file.seekg(0, std::ios::end);
	bool eof = (index >= file.tellg());
	file.seekg(index);
	return eof;
}