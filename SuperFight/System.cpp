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
	std::ifstream file("players.bin", std::ios::binary);

	if (!file.is_open())
	{
		//TODO: file error
		return;
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

void PlayerSystem::run()
{
	bool end = false;
	int n = -1;
	MyString buff = "";

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
				//show players
				break;
			case 2:
				//show market
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
				//register
			}
			else
			{
				//error
			}

		}


	}
}