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

void AdminSystem::handleEmptyMarket() const
{
	if (configs::isMarketEmpty())
	{
		std::cout << "Market is empty!" << std::endl
			<< "You have to add 3 superheroes." << std::endl;

		for (size_t i = 0; i < 3; i++)
		{
			std::cout << std::endl;
			addSuperhero();
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

void AdminSystem::reg()
{
	try
	{
		UserFactory* factory = AdminFactory::getInstance();
		User* user = factory->createFromConsole();
		currentUser = user;
		saveAdminToFile(*user);
		configs::incrementCountOfAdmins();
		std::cout << "Registration was successful!" << std::endl;
		handleEmptyMarket();
	}
	catch (const File_Error&)
	{
		std::cerr << "File error occured when trying to register as a player!" << std::endl;
		throw;
	}
	catch (const Input_Error& error)
	{
		std::cerr << error.what() << std::endl;
		std::cout << std::endl;
		reg();
	}
	catch (const std::invalid_argument&)
	{
		std::cerr << "Invalid argument error occured when trying to register as a player!" << std::endl;
		throw;
	}
	catch (const Regex_Error& error)
	{
		std::cerr << error.what() << std::endl;
		std::cout << std::endl;
		reg();
	}
	catch (const std::length_error& error)
	{
		std::cerr << error.what() << std::endl;
		std::cout << std::endl;
		reg();
	}
	catch (const std::exception& error)
	{
		std::cerr << "An exception was thrown when trying to register as a player!" << std::endl;
		throw;
	}
	catch (...)
	{
		std::cerr << "Something went wrong when trying to register as a player!" << std::endl;
		throw;
	}
}

void AdminSystem::logout()
{
	try
	{
		AdminFactory::freeInstance();
		PlayerFactory::freeInstance();
		SuperHeroFactory::freeInstance();

		delete currentUser;
		currentUser = nullptr;
	}
	catch (const std::exception& error)
	{
		std::cerr << "An exception was thrown when trying to logout as a player!" << std::endl;
		throw;
	}
	catch (...)
	{
		std::cerr << "Something went wrong when trying to logout as a player!" << std::endl;
		throw;
	}

	std::cout << "You logged out." << std::endl;
}

void AdminSystem::deleteProfile()
{
	try
	{
		removeAdminFromFile(*currentUser);
		configs::decrementCountOfAdmins();

		AdminFactory::freeInstance();
		PlayerFactory::freeInstance();
		SuperHeroFactory::freeInstance();

		delete currentUser;
		currentUser = nullptr;
		std::cout << "You deleted your account successfully!" << std::endl;
		std::cout << std::endl;
	}
	catch (const File_Error&)
	{
		std::cerr << "File error occured when trying to delete a profile!" << std::endl;
		throw;
	}
	catch (const std::exception& error)
	{
		std::cerr << "An exception was thrown when trying to delete a profile!" << std::endl;
		throw;
	}
	catch (...)
	{
		std::cerr << "Something went wrong when trying to delete a profile!" << std::endl;
		throw;
	}
}

void AdminSystem::showSoldMarket() const
{
	try
	{
		if (!configs::isSoldEmpty())
		{
			printSuperheroes(constants::SOLD_SUPERHEROES_FILE_PATH);
			MyString buff;
			std::cout << "If you want to go back enter 'back';" << std::endl
				<< "If you want to add a superhero enter 'add';" << std::endl
				<< "If you want to delete enter 'delete':" << std::endl
				<< "Command: ";
			std::cin >> buff;

			if (buff == "back")
			{
				return;
			}
			else if (buff == "add")
			{
				std::cout << std::endl;
				addSuperheroFromSold();
			}
			else if (buff == "delete")
			{
				std::cout << std::endl;
				deleteSuperheroFromSold();
			}
			else
			{
				throw Input_Error("Invalid keyword was entered!");
			}

			showMarket();
		}
		else
		{
			std::cout << "No superheroes to show!" << std::endl;
		}
	}
	catch (const File_Error&)
	{
		std::cerr << "File error occured when trying to show market!" << std::endl;
		throw;
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
		std::cerr << "An exception was thrown when trying to show market!" << std::endl;
		throw;
	}
	catch (...)
	{
		std::cerr << "Something went wrong when trying to show market!" << std::endl;
		throw;
	}
}

void AdminSystem::showMarket() const
{
	try
	{
		if (!configs::isMarketEmpty())
		{
			printSuperheroes(constants::MARKET_SUPERHEROES_FILE_PATH);
			MyString buff;
			std::cout << "If you want to go back enter 'back';" << std::endl
				<< "If you want to add a superhero enter 'add';" << std::endl
				<< "If you want to delete enter 'delete':" << std::endl
				<< "Command: ";
			std::cin >> buff;

			if (buff == "back")
			{
				return;
			}
			else if (buff == "add")
			{
				std::cout << std::endl;
				addSuperhero();
			}
			else if (buff == "delete")
			{
				std::cout << std::endl;
				deleteSuperheroFromMarket();
			}
			else
			{
				throw Input_Error("Invalid keyword was entered!");
			}

			showMarket();
		}
		else
		{
			std::cout << "No superheroes to show!" << std::endl;

			MyString buff;
			std::cout << "If you want to go back enter 'back';" << std::endl
				<< "If you want to add a superhero enter 'add';" << std::endl
				<< "Command: ";
			std::cin >> buff;

			if (buff == "back")
			{
				return;
			}
			else if (buff == "add")
			{
				std::cout << std::endl;
				addSuperhero();
			}
			else
			{
				throw Input_Error("Invalid keyword was entered!");
			}
		}
	}
	catch (const File_Error&)
	{
		std::cerr << "File error occured when trying to show market!" << std::endl;
		throw;
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
		std::cerr << "An exception was thrown when trying to show market!" << std::endl;
		throw;
	}
	catch (...)
	{
		std::cerr << "Something went wrong when trying to show market!" << std::endl;
		throw;
	}
}

void AdminSystem::showPlayers() const
{
	try
	{
		if (configs::getCountOfPlayers() != 0)
		{
			printPlayersForAdmins();
			MyString buff;
			std::cout << "If you want to go back enter 'back';" << std::endl
				<< "If you want to add new player enter 'add';" << std::endl
				<< "If you want to delete player's profile enter 'delete';" << std::endl
				<< "Command: ";
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
		else
		{
			MyString buff;
			std::cout << "No players to show!" << std::endl
				<< "If you want to go back enter 'back';" << std::endl
				<< "If you want to add new player enter 'add';" << std::endl
				<< "Command: ";
			std::cin >> buff;

			if (buff == "back")
			{
				return;
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
		showPlayers();
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
		std::cerr << "An exception was thrown when trying to show players!" << std::endl;
		throw;
	}
	catch (...)
	{
		std::cerr << "Something went wrong when trying to show players!" << std::endl;
		throw;
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
		MyString buff;
		std::cin >> buff;

		if (buff == "back")
		{
			return;
		}
		else if (buff == "delete")
		{
			std::cout << std::endl;
			deleteProfile();
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
	try
	{
		UserFactory* factory = AdminFactory::getInstance();
		currentUser = factory->createFromConsoleOnLogin(constants::ADMINS_FILE_PATH);
		std::cout << "Log in was successful!" << std::endl;
		handleEmptyMarket();
	}
	catch (const File_Error&)
	{
		std::cerr << "File error occured when trying to login as a player!" << std::endl;
		throw;
	}
	catch (const Input_Error& error)
	{
		std::cerr << error.what() << std::endl;
		std::cout << std::endl;
		login();
	}
	catch (const std::invalid_argument& error)
	{
		std::cerr << error.what() << std::endl;
		std::cout << std::endl;
		login();
	}
	catch (const Regex_Error& error)
	{
		std::cerr << error.what() << std::endl;
		std::cout << std::endl;
		login();
	}
	catch (const std::length_error& error)
	{
		std::cerr << error.what() << std::endl;
		std::cout << std::endl;
		login();
	}
	catch (const std::exception& error)
	{
		std::cerr << "An exception was thrown when trying to login as a player!" << std::endl;
		throw;
	}
	catch (...)
	{
		std::cerr << "Something went wrong when trying to login as a player!" << std::endl;
		throw;
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
					showSoldMarket();
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
				MyString buff;
				std::cin >> buff;

				if (buff == "login")
				{
					login();
				}
				else if (buff == "register")
				{
					reg();
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
		exit(1);
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
		exit(1);
	}
	catch (...)
	{
		freeSystem();
		std::cerr << "FATAL ERROR:\n\tUNKNOWN EXCEPTION: " << "Something wrong happened! Logging out!" << std::endl;
		exit(1);
	}

}

void AdminSystem::showAdmins() const
{
	try
	{
		if (configs::getCountOfAdmins() != 0)
		{
			printAdmins();
		}
		else
		{
			std::cout << "No admins to show!" << std::endl;
		}
		MyString buff;
		std::cout << "If you want to go back enter 'back';" << std::endl
			<< "If you want to add an admin enter 'add';" << std::endl
			<< "Command: ";
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
		std::cerr << "An exception was thrown when trying to show players!" << std::endl;
		throw;
	}
	catch (...)
	{
		std::cerr << "Something went wrong when trying to show players!" << std::endl;
		throw;
	}
}

void AdminSystem::deleteSuperheroFromMarket() const
{
	try
	{
		std::cout << "Enter the nickname of the superhero you want to delete: ";
		MyString buff;
		std::cin >> buff;

		removeSuperheroFromFile(constants::MARKET_SUPERHEROES_FILE_PATH, buff);
		configs::decrementCountOfMarketSuperheroes();

		std::cout << "You deleted a superhero successfully!" << std::endl;
		std::cout << std::endl;
	}
	catch (const File_Error&)
	{
		std::cerr << "File error occured when trying to delete a profile!" << std::endl;
		throw;
	}
	catch (const std::exception& error)
	{
		std::cerr << "An exception was thrown when trying to delete a profile!" << std::endl;
		throw;
	}
	catch (...)
	{
		std::cerr << "Something went wrong when trying to delete a profile!" << std::endl;
		throw;
	}
}

void AdminSystem::deleteSuperheroFromSold() const
{
	try
	{
		std::cout << "Enter the nickname of the superhero you want to delete: ";
		MyString buff;
		std::cin >> buff;

		removeSuperheroFromFile(constants::SOLD_SUPERHEROES_FILE_PATH, buff);
		configs::decrementCountOfSoldSuperheroes();

		std::cout << "You deleted a superhero successfully!" << std::endl;
		std::cout << std::endl;
	}
	catch (const File_Error&)
	{
		std::cerr << "File error occured when trying to delete a profile!" << std::endl;
		throw;
	}
	catch (const std::exception& error)
	{
		std::cerr << "An exception was thrown when trying to delete a profile!" << std::endl;
		throw;
	}
	catch (...)
	{
		std::cerr << "Something went wrong when trying to delete a profile!" << std::endl;
		throw;
	}
}

void AdminSystem::deletePlayer() const
{
	try
	{
		std::cout << "Enter the nickname of the player you want to delete: ";
		MyString buff;
		std::cin >> buff;

		removePlayerFromFile(buff);
		configs::decrementCountOfPlayers();

		std::cout << "You deleted a player successfully!" << std::endl;
		std::cout << std::endl;
	}
	catch (const File_Error&)
	{
		std::cerr << "File error occured when trying to delete a profile!" << std::endl;
		throw;
	}
	catch (const std::exception& error)
	{
		std::cerr << "An exception was thrown when trying to delete a profile!" << std::endl;
		throw;
	}
	catch (...)
	{
		std::cerr << "Something went wrong when trying to delete a profile!" << std::endl;
		throw;
	}
}

void AdminSystem::addAdmin() const
{
	User* user = nullptr;
	try
	{
		UserFactory* factory = AdminFactory::getInstance();
		user = factory->createFromConsole();
		saveAdminToFile(*user);
		configs::incrementCountOfAdmins();
		delete user;

		std::cout << "Admin added!" << std::endl;
	}
	catch (const File_Error&)
	{
		std::cerr << "File error occured when trying to add an admin!" << std::endl;
		throw;
	}
	catch (const Input_Error& error)
	{
		std::cerr << error.what() << std::endl;
		std::cout << std::endl;
		delete user;
		addAdmin();
	}
	catch (const std::invalid_argument&)
	{
		std::cerr << "Invalid argument error occured when trying to add an admin!" << std::endl;
		delete user;
		throw;
	}
	catch (const Regex_Error& error)
	{
		std::cerr << error.what() << std::endl;
		std::cout << std::endl;
		delete user;
		addAdmin();
	}
	catch (const std::length_error& error)
	{
		std::cerr << error.what() << std::endl;
		std::cout << std::endl;
		addAdmin();
	}
	catch (const std::exception& error)
	{
		std::cerr << "An exception was thrown when trying to add an admin!" << std::endl;
		delete user;
		throw;
	}
	catch (...)
	{
		std::cerr << "Something went wrong when trying to add an admin!" << std::endl;
		delete user;
		throw;
	}
}

void AdminSystem::addPlayer() const
{
	User* user = nullptr;
	try
	{
		UserFactory* factory = PlayerFactory::getInstance();
		user = factory->createFromConsole();
		Player* player = static_cast<Player*>(user);
		srand(time(0));
		player->addSuperHero(user->getFirstName(), user->getLastName(), user->getNickname(), rand() % 30 + 5, SuperHeroPowerType::Earth);
		savePlayerToFile(*player);
		saveSuperheroToFile(constants::SOLD_SUPERHEROES_FILE_PATH, player->getSuperHeroes()[0]);
		configs::incrementCountOfPlayers();

		delete user;

		std::cout << "Player added!" << std::endl;
	}
	catch (const File_Error&)
	{
		std::cerr << "File error occured when trying to add a player!" << std::endl;
		delete user;
		throw;
	}
	catch (const Input_Error& error)
	{
		std::cerr << error.what() << std::endl;
		std::cout << std::endl;
		delete user;
		addPlayer();
	}
	catch (const std::invalid_argument&)
	{
		std::cerr << "Invalid argument error occured when trying to add a player!" << std::endl;
		throw;
	}
	catch (const Regex_Error& error)
	{
		std::cerr << error.what() << std::endl;
		std::cout << std::endl;
		delete user;
		addPlayer();
	}
	catch (const std::length_error& error)
	{
		std::cerr << error.what() << std::endl;
		std::cout << std::endl;
		delete user;
		addPlayer();
	}
	catch (const std::exception& error)
	{
		std::cerr << "An exception was thrown when trying to add a player!" << std::endl;
		delete user;
		throw;
	}
	catch (...)
	{
		std::cerr << "Something went wrong when trying to add a player!" << std::endl;
		delete user;
		throw;
	}
}

void AdminSystem::addSuperhero() const
{
	SuperHero* superhero = nullptr;
	try
	{
		SuperHeroFactory* factory = SuperHeroFactory::getInstance();
		superhero = factory->createFromConsole();
		saveSuperheroToFile(constants::MARKET_SUPERHEROES_FILE_PATH, *superhero);
		configs::incrementCountOfMarketSuperheroes();
		delete superhero;
	}
	catch (const File_Error&)
	{
		std::cerr << "File error occured when trying to add a superhero!" << std::endl;
		throw;
	}
	catch (const Input_Error& error)
	{
		std::cerr << error.what() << std::endl;
		std::cout << std::endl;
		delete superhero;
		addSuperhero();
	}
	catch (const std::invalid_argument&)
	{
		std::cerr << "Invalid argument error occured when trying to add a superhero!" << std::endl;
		delete superhero;
		throw;
	}
	catch (const Regex_Error& error)
	{
		std::cerr << error.what() << std::endl;
		std::cout << std::endl;
		delete superhero;
		addSuperhero();
	}
	catch (const std::length_error& error)
	{
		std::cerr << error.what() << std::endl;
		std::cout << std::endl;
		delete superhero;
		addSuperhero();
	}
	catch (const std::exception& error)
	{
		std::cerr << "An exception was thrown when trying to add a superhero!" << std::endl;
		delete superhero;
		throw;
	}
	catch (...)
	{
		std::cerr << "Something went wrong when trying to add a superhero!" << std::endl;
		delete superhero;
		throw;
	}
}

void AdminSystem::addSuperheroFromSold() const
{
	SuperHero* superhero = nullptr;
	try
	{
		std::cout << "Enter the nickname of the superhero you want to add: ";
		MyString buff;
		std::cin >> buff;
		SuperHeroFactory* factory = SuperHeroFactory::getInstance();
		superhero = factory->readFromBinary(constants::SOLD_SUPERHEROES_FILE_PATH, buff);
		saveSuperheroToFile(constants::MARKET_SUPERHEROES_FILE_PATH, *superhero);
		removeSuperheroFromFile(constants::SOLD_SUPERHEROES_FILE_PATH, *superhero);
		configs::decrementCountOfSoldSuperheroes();
		configs::incrementCountOfMarketSuperheroes();
		delete superhero;
	}
	catch (const File_Error&)
	{
		std::cerr << "File error occured when trying to add a superhero!" << std::endl;
		delete superhero;
		throw;
	}
	catch (const Input_Error& error)
	{
		std::cerr << error.what() << std::endl;
		std::cout << std::endl;
		delete superhero;
		addSuperheroFromSold();
	}
	catch (const std::invalid_argument&)
	{
		std::cerr << "Invalid argument error occured when trying to add a superhero!" << std::endl;
		delete superhero;
		throw;
	}
	catch (const Regex_Error& error)
	{
		std::cerr << error.what() << std::endl;
		std::cout << std::endl;
		delete superhero;
		addSuperheroFromSold();
	}
	catch (const std::length_error& error)
	{
		std::cerr << error.what() << std::endl;
		std::cout << std::endl;
		delete superhero;
		addSuperheroFromSold();
	}
	catch (const std::exception& error)
	{
		std::cerr << "An exception was thrown when trying to add a superhero!" << std::endl;
		delete superhero;
		throw;
	}
	catch (...)
	{
		std::cerr << "Something went wrong when trying to add a superhero!" << std::endl;
		delete superhero;
		throw;
	}
}