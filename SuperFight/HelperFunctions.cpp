#include "HelperFunctions.h"
#include "Regex_Error.h"
#include "User.h"
#include <fstream>

bool helper::isUpper(char a)
{
	return a >= 'A' && a <= 'Z';
}

bool helper::isLower(char a)
{
	return !isUpper(a);
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
