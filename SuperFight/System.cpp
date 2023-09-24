#include "System.h"
#include <cstring>
#include "Player.h"
#include "Admin.h"
#include "User.h"
#include <vector>
#include <fstream>
#include <iostream>
#include "Validation.h"
#include "Regex_Error.h"
#include "File_Error.h"
#include "SuperHero.h"
#include "Input_Error.h"
#include "SystemConfigurations.h"

template<typename T>
int System::findIndexOfEntity(const std::vector<T>& collection, const std::string& nickname)
{
	for (size_t i = 0; i < collection.size(); i++)
	{
		if (collection[i].getNickname() == nickname)
		{
			return i;
		}
	}

	return -1;
}

template int System::findIndexOfEntity(const std::vector<SuperHero>& collection, const std::string& nickname);
template int System::findIndexOfEntity(const std::vector<Player>& collection, const std::string& nickname);
template int System::findIndexOfEntity(const std::vector<Admin>& collection, const std::string& nickname);

System* System::instance = nullptr;

System::System()
	: admins(std::move(readAdminsFromFile(constants::ADMINS_FILE_PATH))),
	players(std::move(readPlayersFromFile(constants::PLAYERS_FILE_PATH))),
	deadSuperheroes(std::move(readSuperheroesFromFile(constants::DEAD_SUPERHEROES_FILE_PATH))),
	marketSuperheroes(std::move(readSuperheroesFromFile(constants::DEAD_SUPERHEROES_FILE_PATH))) {}


void System::freeSystem()
{
	saveAdminsToFile(constants::ADMINS_FILE_PATH, admins);
	savePlayersToFile(constants::PLAYERS_FILE_PATH, players);
	saveSuperheroesToFile(constants::SUPERHEROES_FILE_PATH, marketSuperheroes);
	saveSuperheroesToFile(constants::DEAD_SUPERHEROES_FILE_PATH, deadSuperheroes);

	delete System::instance;
	System::instance = nullptr;
}

void System::freeInstance()
{
	delete System::instance;
	System::instance = nullptr;
}

System::~System()
{
	delete currentUser;
	currentUser = nullptr;
}
