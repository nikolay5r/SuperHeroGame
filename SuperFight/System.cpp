#include "System.h"
#include "MyString.h"
#include "Player.h"
#include "Admin.h"
#include "User.h"
#include "MyVector.hpp"
#include <fstream>
#include <iostream>
#include "Validation.h"
#include "Regex_Error.h"
#include "File_Error.h"
#include "SuperHero.h"
#include "Input_Error.h"
#include "SystemConfigurations.h"

System* System::instance = nullptr;

System::System()
	: admins(std::move(readAdminsFromFile(constants::ADMINS_FILE_PATH))),
	players(std::move(readPlayersFromFile(constants::PLAYERS_FILE_PATH))),
	deadSuperheroes(std::move(readSuperheroesFromFile(constants::DEAD_SUPERHEROES_FILE_PATH))),
	superheroes(std::move(readSuperheroesFromFile(constants::DEAD_SUPERHEROES_FILE_PATH))) {}


void System::freeSystem()
{
	saveAdminsToFile(constants::ADMINS_FILE_PATH, admins);
	savePlayersToFile(constants::PLAYERS_FILE_PATH, players);
	saveSuperheroesToFile(constants::SUPERHEROES_FILE_PATH, superheroes);
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
