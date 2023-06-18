#include "HelperFunctions.h"
#include "Regex_Error.h"
#include "File_Error.h"
#include "Constants.h"
#include "User.h"
#include "Player.h"
#include "SuperHero.h"
#include "Admin.h"
#include <fstream>

static void getStartIndexAndEndIndexOfPlayerInFile(std::ifstream& file, int& indexStart, int& indexEnd, const Player& player)
{
	if (!file.is_open())
	{
		throw File_Error("File couldn't open!");
	}

	bool isFound = false;

	UserFactory* factory = PlayerFactory::getInstance();

	while (!helper::isEOF(file))
	{
		indexStart = file.tellg();
		User* curr = factory->readFromBinary(file);
		if (file.eof())
		{
			delete curr;
			break;
		}
		if (player.getNickname() == curr->getNickname())
		{
			delete curr;
			isFound = true;
			indexEnd = file.tellg();
			break;
		}
		delete curr;
	}

	if (!isFound)
	{
		throw std::invalid_argument("User doesn't exists!");
	}
}

static void getStartIndexAndEndIndexOfAdminInFile(std::ifstream& file, int& indexStart, int& indexEnd, const Admin& admin)
{
	if (!file.is_open())
	{
		throw File_Error("File couldn't open!");
	}

	bool isFound = false;

	UserFactory* factory = AdminFactory::getInstance();

	while (!helper::isEOF(file))
	{
		indexStart = file.tellg();
		User* curr = factory->readFromBinary(file);
		if (file.eof())
		{
			delete curr;
			break;
		}
		if (admin.getNickname() == curr->getNickname())
		{
			delete curr;
			isFound = true;
			indexEnd = file.tellg();
			break;
		}
		delete curr;
	}

	if (!isFound)
	{
		throw std::invalid_argument("User doesn't exists!");
	}
}

static void getStartIndexAndEndIndexOfSuperHeroInFile(std::ifstream& file, int& indexStart, int& indexEnd, const SuperHero& superhero)
{
	if (!file.is_open())
	{
		throw File_Error("File couldn't open!");
	}

	bool isFound = false;

	SuperHeroFactory* factory = SuperHeroFactory::getInstance();

	while (!helper::isEOF(file))
	{
		indexStart = file.tellg();
		SuperHero* curr = factory->readFromBinary(file);
		if (file.eof())
		{
			delete curr;
			break;
		}
		if (superhero.getNickname() == curr->getNickname())
		{
			delete curr;
			isFound = true;
			indexEnd = file.tellg();
			break;
		}
		delete curr;
	}

	if (!isFound)
	{
		throw std::invalid_argument("User doesn't exists!");
	}
}

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

size_t helper::getFileSize(const MyString& fileName)
{
	std::ifstream file(fileName.c_str(), std::ios::binary);
	if (!file.is_open())
	{
		return 0;
	}

	file.seekg(0, std::ios::end);
	size_t size = file.tellg();

	file.close();
	return size;
}

size_t helper::getFileSize(std::ofstream& file)
{
	size_t curr = file.tellp();
	file.seekp(0, std::ios::end);
	size_t size = file.tellp();
	file.seekp(curr);

	return size;
}

size_t helper::getFileSize(std::ifstream& file)
{
	size_t curr = file.tellg();
	file.seekg(0, std::ios::end);
	size_t size = file.tellg();
	file.seekg(curr);

	return size;
}

void helper::deleteDataFromFile(const MyString& fileName, int indexStart, int indexEnd)
{
	std::ifstream file(fileName.c_str(), std::ios::binary);
	std::ofstream newFile("newFile.bin", std::ios::binary);

	if (!file.is_open() || !newFile.is_open())
	{
		throw File_Error("File couldn't open!");
	}

	//We are making a buffer to store the data from 0 index of the file to the start index of the player and then transfering the data to the new file
	char* buffer = new char[indexStart + 1];
	file.read(buffer, indexStart);
	newFile.write((const char*)&buffer, indexStart);
	delete[] buffer;

	//The same we are doing for the rest of the data after the player's data
	size_t fileSize = helper::getFileSize(file);
	file.seekg(indexEnd);
	buffer = new char[fileSize - indexEnd + 1];
	file.read(buffer, fileSize - indexEnd);
	newFile.write((const char*)&buffer, fileSize - indexEnd);
	delete[] buffer;

	file.close();
	newFile.close();

	remove(fileName.c_str());

	if (rename("newFile.bin", fileName.c_str()) != 0)
	{
		throw File_Error("Error with renaming the files when trying to remove an entity!");
	}
}

void helper::getStartIndexAndEndIndexOfEntityInFile(const MyString& fileName, int& indexStart, int& indexEnd, const Entity& entity)
{
	std::ifstream file(fileName.c_str(), std::ios::binary);

	if (!file.is_open())
	{
		throw File_Error("File couldn't open!");
	}

	if (const User* user = dynamic_cast<const User*>(&entity))
	{
		switch (user->getRole())
		{
		case UserRole::Admin:
			getStartIndexAndEndIndexOfAdminInFile(file, indexStart, indexEnd, static_cast<const Admin&>(*user));
			break;
		case UserRole::Player:
			getStartIndexAndEndIndexOfPlayerInFile(file, indexStart, indexEnd, static_cast<const Player&>(*user));
			break;
		default:
			break;
		}
	}
	else
	{
		getStartIndexAndEndIndexOfSuperHeroInFile(file, indexStart, indexEnd, static_cast<const SuperHero&>(entity));
	}

	file.close();
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