#include <iostream>
#include <fstream>
#include "MyString.h"
#include "System.h"
#include "PlayerSystem.h"
#include "AdminSystem.h"
#include "Input_Error.h"
#include "SystemConfigurations.h"

void showStartMenu(System* system)
{
	MyString buff;
	std::cout << "Enter 'admin' if you want to sign in as an admin;" << std::endl
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
}

void startSystem(System* system)
{
	std::cout << "Welcome, challenger!" << std::endl << std::endl;
	try
	{
		showStartMenu(system);

		while (!system->run())
		{
			system->freeInstance();
			showStartMenu(system);
		}
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
