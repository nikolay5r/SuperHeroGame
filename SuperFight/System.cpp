#include "System.h"
#include "MyString.h"
#include "Player.h"
#include "Admin.h"
#include "User.h"
#include "MyVector.hpp"
#include <fstream>
#include <iostream>

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
	currentUser = nullptr;
}

void PlayerSystem::login()
{

}

void PlayerSystem::run()
{
	bool end = false;

	std::fstream file("players.bin", std::ios::in | std::ios::binary | std::ios::out);
	std::fstream s("a.bin", std::ios::in | std::ios::binary | std::ios::out);

	while (end)
	{
		if (currentUser)
		{
			std::cout << "Enter a command:" << std::endl
				<< "  0 - logout" << std::endl
				<< "  1 - show all players" << std::endl
				<< "  2 - show market" << std::endl
				<< "  3 - exit" << std::endl;

			int n;
			std::cin >> n;

			switch (n)
			{
			case 0:
				logout();
				break;
			case 1:

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
			MyString buff;
			std::cout << "Enter 'login' if you already have an account and 'register' if you don't: ";
			std::cin >> buff;

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