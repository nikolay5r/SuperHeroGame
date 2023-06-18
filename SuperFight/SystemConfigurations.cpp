#include "SystemConfigurations.h"
#include <fstream>
#include "File_Error.h"
#include "Constants.h"
#include "HelperFunctions.h"
#include "MyString.h"

// File: [(unsigned)->countOfAdmins][(unsigned)->countOfPlayers][(unsigned)->countOfMarketSuperheroes][(unsigned)->countOfSold][(unsigned)->countOfLogged][strings->loggedUsersNicknames]

void configs::initConfigs()
{
	if (helper::getFileSize(constants::CONFIGS_FILE_PATH) == 0)
	{
		std::ofstream file(constants::CONFIGS_FILE_PATH.c_str(), std::ios::binary);
		if (!file.is_open())
		{
			throw File_Error("File couldn't open!");
		}

		//1 is representing 0 because when trying to enter 0 as counts on init file is full of terminating zeroes and nothing could be read
		unsigned count = 1;

		file.write((char*)&count, sizeof(count));
		file.write((char*)&count, sizeof(count));
		file.write((char*)&count, sizeof(count));
		file.write((char*)&count, sizeof(count));
		file.write((char*)&count, sizeof(count));

		file.close();
	}
}

static unsigned getCount(size_t position)
{
	std::ifstream file(constants::CONFIGS_FILE_PATH.c_str(), std::ios::binary);
	if (!file.is_open())
	{
		throw File_Error("File couldn't open!");
	}
	unsigned count = 0;

	file.seekg(position * sizeof(unsigned));
	file.read((char*)&count, sizeof(count));

	file.close();

	return count;
}

static void changeCount(size_t position, int numberToChange)
{
	std::fstream file(constants::CONFIGS_FILE_PATH.c_str(), std::ios::in | std::ios::out | std::ios::binary);

	if (!file.is_open())
	{
		throw File_Error("File couldn't open!");
	}

	unsigned count = 0;
	file.seekg(position * sizeof(unsigned));
	file.read((char*)&count, sizeof(unsigned));
	file.seekp(position * sizeof(unsigned));
	count += numberToChange;
	file.write((const char*)&count, sizeof(unsigned));

	file.close();
}

void configs::incrementCountOfAdmins()
{
	changeCount(0, 1);
}

void configs::incrementCountOfPlayers()
{
	changeCount(1, 1);
}

void configs::incrementCountOfMarketSuperheroes()
{
	changeCount(2, 1);
}

void configs::incrementCountOfSoldSuperheroes()
{
	changeCount(3, 1);
}

void configs::decrementCountOfAdmins()
{
	changeCount(0, -1);
}

void configs::decrementCountOfPlayers()
{
	changeCount(1, -1);
}

void configs::decrementCountOfMarketSuperheroes()
{
	changeCount(2, -1);
}

void configs::decrementCountOfSoldSuperheroes()
{
	changeCount(3 * sizeof(unsigned), -1);

}

bool configs::saveLoggedPlayerInThePeriod(const MyString& nickname)
{
	std::fstream file(constants::CONFIGS_FILE_PATH.c_str(), std::ios::in | std::ios::out | std::ios::binary);

	if (!file.is_open())
	{
		throw File_Error("File couldn't open!");
	}

	unsigned count = getCount(4);
	if (count == 0)
	{
		size_t len = strlen(nickname.c_str());
		file.write((const char*)&len, sizeof(len));
		file.write(nickname.c_str(), len + 1);
		file.close();
		return true;
	}
	else
	{
		for (size_t i = 0; i < count; i++)
		{
			size_t len = strlen(nickname.c_str());
			file.read((char*)&len, sizeof(len));
			char* buff = new char[len + 1];
			file.read(buff, len + 1);

			if (buff == nickname)
			{
				file.close();
				return false;
			}
		}

		size_t len = strlen(nickname.c_str());
		file.write((const char*)&len, sizeof(len));
		file.write(nickname.c_str(), len + 1);
	}

	file.close();
	return true;
}

void configs::handlePeriod()
{
	unsigned countOfPlayers = getCountOfPlayers();
	unsigned countOfPlayersInThePeriod = getCountOfLogged();

	if (countOfPlayers == countOfPlayersInThePeriod)
	{
		changeCount(4, (-1) * countOfPlayersInThePeriod);
		helper::deleteDataFromFile(constants::CONFIGS_FILE_PATH, 5 * sizeof(unsigned), helper::getFileSize(constants::CONFIGS_FILE_PATH));
	}
}

bool configs::isMarketEmpty()
{
	unsigned count = getCountOfMarket();
	return count == 0;
}

bool configs::isSoldEmpty()
{
	unsigned count = getCountOfSold();
	return count == 0;
}

unsigned configs::getCountOfAdmins()
{
	return getCount(0) - 1;
}

unsigned configs::getCountOfPlayers()
{
	return getCount(1) - 1;
}

unsigned configs::getCountOfMarket()
{
	return getCount(2) - 1;
}

unsigned configs::getCountOfSold()
{
	return getCount(3) - 1;
}

unsigned configs::getCountOfLogged()
{
	return getCount(4) - 1;
}