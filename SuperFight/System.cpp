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

System* System::instance = nullptr;

void System::freeInstance()
{
	delete System::instance;
	System::instance = nullptr;
}

System::~System()
{
	delete System::instance;
	System::instance = nullptr;
}

System* PlayerSystem::getInstance()
{
	if (!System::instance)
	{
		System::instance = new PlayerSystem();
	}

	return System::instance;
}

System* AdminSystem::getInstance()
{
	if (!System::instance)
	{
		System::instance = new AdminSystem();
	}

	return System::instance;
}

void PlayerSystem::logout()
{
	delete currentUser;
	currentUser = nullptr;
}

void PlayerSystem::login()
{
	std::ifstream file(constants::PLAYERS_FILE_PATH.c_str(), std::ios::binary);

	if (!file.is_open())
	{
		throw File_Error("File couldn't open!");
	}

	MyString temp;
	std::cout << "Enter username: ";
	std::cin >> temp;
	validation::isUsernameValid(temp);

	UserFactory* factory = PlayerFactory::getInstance();
	User* user = factory->readFromBinaryByUsername(file, temp);
	std::cout << "Enter password: ";
	std::cin >> temp;

	if (user->getPassword() != temp)
	{
		delete user;
		file.close();
		//TODO: password error
		return;
	}

	currentUser = user;
	file.close();
}

void PlayerSystem::reg()
{
	UserFactory* factory = PlayerFactory::getInstance();
	User* user = factory->createFromConsole();
	const Player& player = static_cast<const Player&>(*user);
	savePlayerToFile(player);
	currentUser = user;
}

void PlayerSystem::showPlayers() const
{
	std::ifstream file(constants::MARKET_SUPERHEROES_FILE_PATH.c_str(), std::ios::in | std::ios::binary);;

	while (!file.eof())
	{
		for (size_t i = 0; i < constants::ENTITIES_TO_LOAD; i++)
		{
			if (file.eof())
			{
				break;
			}
			SuperHeroFactory* factory = SuperHeroFactory::getInstance();
			SuperHero* superhero = factory->readFromBinary(file);
			superhero->print();
			delete superhero;
		}
		if (file.eof())
		{
			break;
		}
		else
		{
			MyString buff;
			std::cout << "If you want to see more superheroes enter 'forward' if you want to go back enter 'back': ";
			std::cin >> buff;
			if (buff == "forward")
			{
				continue;
			}
			else if (buff == "back")
			{
				break;
			}
			else
			{
				throw std::invalid_argument("Invalid keyword was entered!");
			}
		}
	}

	file.close();
}

void PlayerSystem::showMarket() const
{
	std::ifstream file(constants::PLAYERS_FILE_PATH.c_str(), std::ios::in | std::ios::binary);;

	while (!file.eof())
	{
		for (size_t i = 0; i < constants::ENTITIES_TO_LOAD; i++)
		{
			if (file.eof())
			{
				break;
			}
			UserFactory* factory = PlayerFactory::getInstance();
			User* player = factory->readFromBinary(file);
			player->print();
			delete player;
		}
		if (file.eof())
		{
			break;
		}
		else
		{
			MyString buff;
			std::cout << "If you want to see more players enter 'forward' if you want to go back enter 'back': ";
			std::cin >> buff;
			if (buff == "forward")
			{
				continue;
			}
			else if (buff == "back")
			{
				break;
			}
			else
			{
				throw std::invalid_argument("Invalid keyword was entered!");
			}
		}
	}

	file.close();
}

void PlayerSystem::run()
{
	bool end = false;
	int n = -1;
	MyString buff = "";

	//TODO: try{...} catch(...){}
	while (end)
	{
		if (currentUser)
		{
			if (n == -1)
			{
				std::cout << "Enter a command:" << std::endl
					<< "  0 - logout" << std::endl
					<< "  1 - show all players" << std::endl
					<< "  2 - show market" << std::endl
					<< "  3 - exit" << std::endl;

				std::cin >> n;
			}

			switch (n)
			{
			case 0:
				logout();
				break;
			case 1:
				showPlayers();
				break;
			case 2:
				showMarket();
				break;
			case 3:
				end = true;
				break;
			default:
				//error
				break;
			}
		}
		else
		{
			if (buff == "")
			{
				std::cout << "Enter 'login' if you already have an account and 'register' if you don't: ";
				std::cin >> buff;
			}

			if (buff == "login")
			{
				login();
			}
			else if (buff == "register")
			{
				reg();
			}
			else
			{
				//error
			}

		}


	}
}