#include <iostream>
#include <fstream>
#include "MyString.h"
#include "System.h"
#include "Input_Error.h"

void startSystem(System* system)
{
	try
	{
		MyString buff;
		std::cout << "Welcome, challenger!" << std::endl
			<< "Enter 'admin' if you want to sign in as an admin;" << std::endl
			<< "Enter 'player' if you want to sign in as a player: " << std::endl
			<< "Command: ";
		std::cin >> buff;

		if (buff == "player")
		{
			system = PlayerSystem::getInstance();
		}
		else if (buff == "admin")
		{
			system = AdminSystem::getInstance();
		}
		else
		{
			throw Input_Error("Keyword is invalid!");
		}

		system->run();
	}
	catch (const Input_Error& error)
	{
		std::cerr << error.what() << std::endl;
		startSystem(system);
	}
	catch (...)
	{
		std::cerr << "Something went wrong in the start function!" << std::endl;
	}

}

int main()
{
	System* system = nullptr;
	startSystem(system);
	system->freeInstance();
}
