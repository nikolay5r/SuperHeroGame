#include "AdminSystem.h"
#include "System.h"
#include "File_Error.h"
#include "Input_Error.h"
#include "SystemConfigurations.h"
#include "Regex_Error.h"
#include "Player.h"
#include "Admin.h"
#include "SuperHero.h"
#include <iostream>

void AdminSystem::handleEmptyMarket()
{
	if (marketSuperheroes.empty())
	{
		std::cout << "The market is empty and because of that you have to add 3 superheroes!" << std::endl;

		for (size_t i = 0; i < 3; i++)
		{
			addSuperheroToMarket();
		}
	}
}

System* AdminSystem::getInstance()
{
	if (!System::instance)
	{
		System::instance = new AdminSystem();
	}

	return System::instance;
}

void AdminSystem::logout()
{
	currentUser = nullptr;
	std::cout << "You logged out." << std::endl;
}

void AdminSystem::deleteOwnProfile()
{
	try
	{
		admins.erase(admins.begin() + findIndexOfEntity(admins, currentUser->getFirstName()));
		currentUser = nullptr;
		std::cout << "You deleted your account successfully!" << std::endl;
		std::cout << std::endl;
	}
	catch (const std::exception& error)
	{
		std::cerr << "An exception was thrown when trying to delete a profile! " << error.what() << std::endl;
		exit(EXIT_FAILURE);
	}
	catch (...)
	{
		std::cerr << "Something went wrong when trying to delete a profile!" << std::endl;
		exit(EXIT_FAILURE);
	}
}

void AdminSystem::showGraveyard()
{
	try
	{
		size_t size = deadSuperheroes.size();
		std::string buff;

		if (size == 0)
		{
			std::cout << "No superheroes to show right now..." << std::endl
				<< std::endl
				<< "If you want to go back enter 'back';" << std::endl
				<< "If you want to add a superhero to the market enter 'add':" << std::endl;
		}
		else
		{
			for (size_t i = 0; i < size; i++)
			{
				deadSuperheroes[i].printFullInfo();
			}
			std::cout << std::endl
				<< "If you want to go back enter 'back';" << std::endl
				<< "If you want to add a superhero to the market enter 'add';" << std::endl
				<< "If you want to delete enter 'delete':" << std::endl;
		}

		std::cout << "Command: ";
		std::cin >> buff;

		if (buff == "back")
		{
			return;
		}
		else if (buff == "add")
		{
			std::cout << std::endl;
			addSuperheroFromGraveyard();
		}
		else if (buff == "delete")
		{
			std::cout << std::endl;
			deleteSuperhero(deadSuperheroes);
		}
		else
		{
			throw Input_Error("Invalid keyword was entered!");
		}
	}
	catch (const Input_Error& error)
	{
		std::cerr << error.what() << std::endl;
		showGraveyard();
	}
	catch (const std::invalid_argument& error)
	{
		std::cerr << error.what() << std::endl;
		showGraveyard();
	}
	catch (const std::exception& error)
	{
		std::cerr << "An exception was thrown when trying to show market! " << error.what() << std::endl;
		exit(EXIT_FAILURE);
	}
	catch (...)
	{
		std::cerr << "Something went wrong when trying to show market!" << std::endl;
		exit(EXIT_FAILURE);
	}
}

void AdminSystem::showMarket()
{
	try
	{
		size_t size = marketSuperheroes.size();
		std::string buff;

		if (size == 0)
		{
			std::cout << "No superheroes to show right now..." << std::endl
				<< std::endl
				<< "If you want to go back enter 'back';" << std::endl
				<< "If you want to add a superhero enter 'add':" << std::endl;
		}
		else
		{
			for (size_t i = 0; i < size; i++)
			{
				marketSuperheroes[i].printFullInfo();
			}
			std::cout << std::endl
				<< "If you want to go back enter 'back';" << std::endl
				<< "If you want to add a superhero enter 'add';" << std::endl
				<< "If you want to delete enter 'delete':" << std::endl;
		}

		std::cout << "Command: ";
		std::cin >> buff;

		if (buff == "back")
		{
			return;
		}
		else if (buff == "add")
		{
			std::cout << std::endl;
			addSuperheroToMarket();
		}
		else if (buff == "delete")
		{
			std::cout << std::endl;
			deleteSuperhero(marketSuperheroes);
		}
		else
		{
			throw Input_Error("Invalid keyword was entered!");
		}
	}
	catch (const Input_Error& error)
	{
		std::cerr << error.what() << std::endl;
		showMarket();
	}
	catch (const std::invalid_argument& error)
	{
		std::cerr << error.what() << std::endl;
		showMarket();
	}
	catch (const std::exception& error)
	{
		std::cerr << "An exception was thrown when trying to show market! " << error.what() << std::endl;
		exit(EXIT_FAILURE);
	}
	catch (...)
	{
		std::cerr << "Something went wrong when trying to show market!" << std::endl;
		exit(EXIT_FAILURE);
	}
}

void AdminSystem::showPlayers()
{
	try
	{
		size_t size = players.size();
		std::string buff;

		if (size == 0)
		{
			std::cout << "No players to show right now..." << std::endl
				<< std::endl
				<< "If you want to go back enter 'back';" << std::endl
				<< "If you want to add a player enter 'add':" << std::endl;
		}
		else
		{
			for (size_t i = 0; i < size; i++)
			{
				players[i].printFullInfo();
			}
			std::cout << std::endl
				<< "If you want to go back enter 'back';" << std::endl
				<< "If you want to add a player enter 'add';" << std::endl
				<< "If you want to delete enter 'delete':" << std::endl;
		}

		std::cout << "Command: ";
		std::cin >> buff;

		if (buff == "back")
		{
			return;
		}
		else if (buff == "delete")
		{
			std::cout << std::endl;
			deletePlayer();
		}
		else if (buff == "add")
		{
			std::cout << std::endl;
			addPlayer();
		}
		else
		{
			throw Input_Error("Invalid keyword was entered!");
		}
	}
	catch (const File_Error&)
	{
		std::cerr << "File error occured when trying to show players!" << std::endl;
		throw;
	}
	catch (const Input_Error& error)
	{
		std::cerr << error.what() << std::endl;
		showPlayers();
	}
	catch (const std::invalid_argument& error)
	{
		std::cerr << error.what() << std::endl;
		showPlayers();
	}
	catch (const std::exception& error)
	{
		std::cerr << "An exception was thrown when trying to show players! " << error.what() << std::endl;
		exit(EXIT_FAILURE);
	}
	catch (...)
	{
		std::cerr << "Something went wrong when trying to show players!" << std::endl;
		exit(EXIT_FAILURE);
	}
}

void AdminSystem::showProfile()
{
	try
	{
		currentUser->printFullInfo();

		std::cout << std::endl << "If you want to go back enter 'back';" << std::endl
			<< "If you want to delete your profile enter 'delete': " << std::endl
			<< "Command: ";
		std::string buff;
		std::cin >> buff;

		if (buff == "back")
		{
			return;
		}
		else if (buff == "delete")
		{
			std::cout << std::endl;
			deleteOwnProfile();
		}
		else
		{
			throw Input_Error("Keyword is not valid!");
		}

		std::cout << std::endl;
		showProfile();
	}
	catch (const Input_Error& error)
	{
		std::cerr << error.what() << std::endl;
		std::cout << std::endl;
		showProfile();
	}
	catch (const std::logic_error& error)
	{
		std::cerr << error.what() << std::endl;
		std::cout << std::endl;
		showProfile();
	}
	catch (const std::exception& error)
	{
		std::cerr << "An exception was thrown when trying to show profile!" << std::endl;
		throw;
	}
	catch (...)
	{
		std::cerr << "Something happened when trying to show profile!" << std::endl;
		throw;
	}
}

void AdminSystem::login()
{
	std::string buff;
	std::cout << "If you want to go back enter 'back';" << std::endl
		<< "Enter nickname: ";
	std::cin >> buff;

	if (buff == "back")
	{
		return;
	}

	int index = findIndexOfEntity(admins, buff);

	if (index == -1)
	{
		std::cerr << "There is no admin with that nickname!" << std::endl;
		login();
	}

	std::cout << "Enter password: ";
	std::cin >> buff;

	if (admins[index].getPassword() == buff)
	{
		currentUser = &admins[index];
		std::cout << "Log in was successful!" << std::endl;
		std::cout << "Welcome back, " << currentUser->getNickname() << "!" << std::endl;
		handleEmptyMarket();
	}
	else
	{
		std::cerr << "Invalid password! Please try again!" << std::endl;
		login();
	}
}

void AdminSystem::run()
{
	bool end = false;
	int n = -1;
	configs::initConfigs();
	try
	{
		while (!end)
		{
			if (currentUser)
			{
				std::cout << "Enter a command:" << std::endl
					<< "  0 - logout" << std::endl
					<< "  1 - show all players" << std::endl
					<< "  2 - show market" << std::endl
					<< "  3 - show sold market" << std::endl
					<< "  4 - show profile" << std::endl
					<< "  5 - show admins" << std::endl
					<< "  6 - exit" << std::endl << std::endl
					<< "Command: ";

				std::cin >> n;

				switch (n)
				{
				case 0:
					std::cout << std::endl;
					logout();
					break;
				case 1:
					std::cout << std::endl;
					showPlayers();
					break;
				case 2:
					std::cout << std::endl;
					showMarket();
					break;
				case 3:
					std::cout << std::endl;
					showGraveyard();
					break;
				case 4:
					std::cout << std::endl;
					showProfile();
					break;
				case 5:
					std::cout << std::endl;
					showAdmins();
					break;
				case 6:
					std::cout << std::endl;
					logout();
					end = true;
					break;
				default:
					throw Input_Error("Keyword is not valid!");
				}
			}
			else
			{
				std::cout << "Enter 'back' if you want to go back;"
					<< std::endl << "Enter 'login' if you already have an account and 'register' if you don't: " << std::endl
					<< "Command: ";
				std::string buff;
				std::cin >> buff;

				if (buff == "login")
				{
					login();
				}
				else if (buff == "back")
				{
					end = true;
					break;
				}
				else
				{
					throw Input_Error("Keyword is not valid!");
				}
			}
		}
	}
	catch (const File_Error& error)
	{
		freeSystem();
		std::cerr << "FATAL ERROR:\n\tFILE_ERROR: " << error.what() << " Logging out!" << std::endl;
		exit(EXIT_FAILURE);
	}
	catch (const Input_Error& error)
	{
		std::cerr << error.what() << std::endl;
		run();
	}
	catch (const std::invalid_argument& error)
	{
		std::cerr << error.what() << std::endl;
		run();
	}
	catch (const std::exception& error)
	{
		freeSystem();
		std::cerr << "FATAL ERROR:\n\tUNKNOWN EXCEPTION: " << error.what() << " Logging out!" << std::endl;
		exit(EXIT_FAILURE);
	}
	catch (...)
	{
		freeSystem();
		std::cerr << "FATAL ERROR:\n\tUNKNOWN EXCEPTION: " << "Something wrong happened! Logging out!" << std::endl;
		exit(EXIT_FAILURE);
	}

}

void AdminSystem::showAdmins()
{
	try
	{
		size_t size = players.size();
		std::string buff;

		if (size == 0)
		{
			std::cout << "No admins to show right now..." << std::endl;
		}
		else
		{
			for (size_t i = 0; i < size; i++)
			{
				admins[i].printShortInfo();
			}
			std::cout << std::endl;
		}
		std::cout << "If you want to go back enter 'back';" << std::endl
			<< "If you want to add an admin enter 'add';" << std::endl;

		std::cout << "Command: ";
		std::cin >> buff;

		if (buff == "back")
		{
			return;
		}
		else if (buff == "add")
		{
			std::cout << std::endl;
			addAdmin();
		}
		else
		{
			throw Input_Error("Invalid keyword was entered!");
		}
	}
	catch (const Input_Error& error)
	{
		std::cerr << error.what() << std::endl;
		showAdmins();
	}
	catch (const std::invalid_argument& error)
	{
		std::cerr << error.what() << std::endl;
		showAdmins();
	}
	catch (const std::exception& error)
	{
		std::cerr << "An exception was thrown when trying to show players! " << error.what() << std::endl;
		exit(EXIT_FAILURE);
	}
	catch (...)
	{
		std::cerr << "Something went wrong when trying to show players!" << std::endl;
		exit(EXIT_FAILURE);
	}
}

void AdminSystem::deleteSuperhero(std::vector<SuperHero>& superheroes)
{
	std::string buff;
	std::cout << "If you want to go back enter 'back';" << std::endl
		<< "Enter the nickname of the superhero you want to delete: ";
	std::cin >> buff;

	if (buff == "back")
	{
		return;
	}

	int index = findIndexOfEntity(superheroes, buff);
	if (index != -1)
	{
		superheroes.erase(superheroes.begin() + index);

		std::cout << "You deleted a superhero successfully!" << std::endl;
		std::cout << std::endl;
	}
	else
	{
		std::cerr << "Invalid nickname! Try again..." << std::endl << std::endl;
		deleteSuperhero(superheroes);
	}
}

void AdminSystem::deletePlayer()
{
	std::string buff;
	std::cout << "If you want to go back enter 'back';" << std::endl
		<< "Enter the nickname of the player you want to delete: ";
	std::cin >> buff;

	if (buff == "back")
	{
		return;
	}

	int index = findIndexOfEntity(players, buff);
	if (index != -1)
	{
		players.erase(players.begin() + index);

		std::cout << "You deleted " << buff << "'s profile successfully!" << std::endl;
		std::cout << std::endl;
	}
	else
	{
		std::cerr << "Invalid nickname! Try again..." << std::endl << std::endl;
		deletePlayer();
	}

}

void AdminSystem::addAdmin()
{
	try
	{
		std::cout << "Admin to add:" << std::endl;

		admins.push_back(std::move(createAdminFromConsole()));

		std::cout << "Admin added!" << std::endl;
	}
	catch (const Input_Error& error)
	{
		std::cerr << error.what() << std::endl << std::endl;
		addAdmin();
	}
	catch (const std::invalid_argument& error)
	{
		std::cerr << error.what() << std::endl << std::endl;
		addAdmin();
	}
	catch (const Regex_Error& error)
	{
		std::cerr << error.what() << std::endl << std::endl;
		addAdmin();
	}
	catch (const std::length_error& error)
	{
		std::cerr << error.what() << std::endl << std::endl;
		addAdmin();
	}
	catch (const std::exception& error)
	{
		std::cerr << "An exception was thrown when trying to add an admin! " << error.what() << std::endl;
		exit(EXIT_FAILURE);
	}
	catch (...)
	{
		std::cerr << "Something went wrong when trying to add an admin!" << std::endl;
		exit(EXIT_FAILURE);
	}
}

void AdminSystem::addPlayer()
{
	try
	{
		std::cout << "Player to add:" << std::endl;

		players.push_back(std::move(createPlayerFromConsole()));

		std::cout << "Player added!" << std::endl;
	}
	catch (const Input_Error& error)
	{
		std::cerr << error.what() << std::endl << std::endl;
		addPlayer();
	}
	catch (const std::invalid_argument& error)
	{
		std::cerr << error.what() << std::endl << std::endl;
		addPlayer();
	}
	catch (const Regex_Error& error)
	{
		std::cerr << error.what() << std::endl << std::endl;
		addPlayer();
	}
	catch (const std::length_error& error)
	{
		std::cerr << error.what() << std::endl << std::endl;
		addPlayer();
	}
	catch (const std::exception& error)
	{
		std::cerr << "An exception was thrown when trying to add a player! " << error.what() << std::endl;
		exit(EXIT_FAILURE);
	}
	catch (...)
	{
		std::cerr << "Something went wrong when trying to add a player!" << std::endl;
		exit(EXIT_FAILURE);
	}
}

void AdminSystem::addSuperheroToMarket()
{
	try
	{
		std::cout << "Superhero to add:" << std::endl;

		marketSuperheroes.push_back(std::move(createSuperheroFromConsole()));
	}
	catch (const Input_Error& error)
	{
		std::cerr << error.what() << std::endl << std::endl;
		addSuperheroToMarket();
	}
	catch (const std::invalid_argument& error)
	{
		std::cerr << error.what() << std::endl << std::endl;
		addSuperheroToMarket();
	}
	catch (const Regex_Error& error)
	{
		std::cerr << error.what() << std::endl << std::endl;
		addSuperheroToMarket();
	}
	catch (const std::length_error& error)
	{
		std::cerr << error.what() << std::endl << std::endl;
		addSuperheroToMarket();
	}
	catch (const std::exception& error)
	{
		std::cerr << "An exception was thrown when trying to add a superhero to the market!" << std::endl;
		exit(EXIT_FAILURE);
	}
	catch (...)
	{
		std::cerr << "Something went wrong when trying to add a superhero to the market!" << std::endl;
		exit(EXIT_FAILURE);
	}
}