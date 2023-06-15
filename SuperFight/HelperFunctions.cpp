#include "HelperFunctions.h"
#include "Regex_Error.h"
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

int helper::findEntityIndexInFile(const MyString& filePath, const MyString& usernameToFind)
{
	std::ifstream file(filePath.c_str(), std::ios::binary);

	int indexInPlayersFile = 0;
	while (!file.eof())
	{
		size_t length = 0;
		file.read((char*)&length, sizeof(length));
		char* username = new char[length + 1];
		file.read(username, length + 1);
		file.read((char*)&indexInPlayersFile, sizeof(indexInPlayersFile));

		if (usernameToFind == username)
		{
			file.close();
			delete[] username;
			return indexInPlayersFile;
		}

		delete[] username;

		if (file.eof())
		{
			break;
		}
	}

	file.close();
	return -1;
}
