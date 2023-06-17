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

static MyString buff;

System* System::instance = nullptr;

void System::freeSystem()
{
	delete System::instance;
	System::instance = nullptr;

	UserFactory::freeInstance();
	SuperHeroFactory::freeInstance();

	delete currentUser;
	currentUser = nullptr;
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
	try
	{
		removeFromFile(*currentUser);
		saveToFile(*currentUser);

		UserFactory::freeInstance();
		SuperHeroFactory::freeInstance();

		delete currentUser;
		currentUser = nullptr;
	}
	catch (const File_Error&)
	{
		std::cerr << "File error occured when trying to logout as a player!" << std::endl;
		throw;
	}
	catch (const std::invalid_argument&)
	{
		std::cerr << "Invalid argument error occured when trying to logout as a player!" << std::endl;
		throw;
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
}

void PlayerSystem::login()
{
	try
	{
		UserFactory* factory = PlayerFactory::getInstance();
		currentUser = factory->createFromConsoleOnLogin(constants::PLAYERS_FILE_PATH);
	}
	catch (const File_Error&)
	{
		std::cerr << "File error occured when trying to login as a player!" << std::endl;
		throw;
	}
	catch (const Input_Error& error)
	{
		std::cerr << error.what() << std::endl;
		reg();
	}
	catch (const std::invalid_argument&)
	{
		std::cerr << "Invalid argument error occured when trying to login as a player!" << std::endl;
		throw;
	}
	catch (const Regex_Error& error)
	{
		std::cerr << error.what() << std::endl;
		login();
	}
	catch (const std::length_error& error)
	{
		std::cerr << error.what() << std::endl;
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

void PlayerSystem::reg()
{
	try
	{
		UserFactory* factory = PlayerFactory::getInstance();
		User* user = factory->createFromConsole();
		currentUser = user;
		srand(time(0));
		static_cast<Player*>(user)->addSuperHero(user->getFirstName(), user->getLastName(), user->getNickname(), rand() % 30 + 5, SuperHeroPowerType::Earth);
	}
	catch (const File_Error&)
	{
		std::cerr << "File error occured when trying to register as a player!" << std::endl;
		throw;
	}
	catch (const Input_Error& error)
	{
		std::cerr << error.what() << std::endl;
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
		reg();
	}
	catch (const std::length_error& error)
	{
		std::cerr << error.what() << std::endl;
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

void PlayerSystem::sellSuperHero() const
{
	try
	{
		Player* player = static_cast<Player*>(currentUser);
		if (player->getNumberOfSuperHeroes() == 0)
		{
			throw std::logic_error("You have no superheroes to sell!");
		}
		std::cout << "Enter 'back' if you want to go back;" << std::endl << "Enter nickname of the superhero you want to sell: ";
		std::cin >> buff;
		if (buff == "back")
		{
			return;
		}
		player->sellSuperHero(buff);
		sell(buff);
	}
	catch (const File_Error& error)
	{
		std::cerr << "File error occured when trying to sell player!" << std::endl;
		throw;
	}
	catch (const std::invalid_argument& error)
	{
		std::cerr << error.what() << std::endl;
		sellSuperHero();
	}
	catch (const std::logic_error& error)
	{
		std::cerr << error.what() << std::endl;
		sellSuperHero();
	}
	catch (const std::exception& error)
	{
		std::cerr << "An exception was thrown when trying to sell a superhero!" << std::endl;
		throw;
	}
	catch (...)
	{
		std::cerr << "Something went wrong when trying to sell a superhero!" << std::endl;
		throw;
	}
}

static void printResultOfBattle(int result, int playerCoins, int otherPlayerCoins, const Player& player, const Player& otherPlayer)
{
	switch (result)
	{
	case 1:
		std::cout << "You won!" << std::endl
			<< "You won " << player.getCoins() - playerCoins << " coins." << std::endl
			<< "Enemy lost " << otherPlayerCoins - otherPlayer.getCoins() << " coins." << std::endl;
		break;
	case 0:
		std::cout << "It was a tie!" << std::endl
			<< "You lost " << playerCoins - player.getCoins() << " coins." << std::endl;
		break;
	case -1:
		std::cout << "You lost!" << std::endl
			<< "You lost " << playerCoins - player.getCoins() << " coins." << std::endl
			<< "Enemy won " << otherPlayer.getCoins() - otherPlayerCoins << " coins." << std::endl;
		break;
	default:
		std::cout << "I don't know what happened!" << std::endl;
		break;
	}
}

void PlayerSystem::battle() const
{
	User* user = nullptr;
	try
	{
		Player* player = static_cast<Player*>(currentUser);
		if (player->getNumberOfSuperHeroes() == 0)
		{
			throw std::logic_error("You have no superheroes! You have to buy at least to start fighting!");
		}

		std::cout << "Enter 'back' if you want to go back;" << std::endl << "Enter player you want to attack: ";
		std::cin >> buff;
		if (buff == "back")
		{
			return;
		}
		UserFactory* factory = PlayerFactory::getInstance();
		user = factory->readFromBinary(buff);
		Player* otherPlayer = static_cast<Player*>(user);
		unsigned playerCoinsAtStart = player->getCoins();
		unsigned otherPlayerCoinsAtStart = otherPlayer->getCoins();
		if (otherPlayer->getNumberOfSuperHeroes() == 0)
		{
			player->attack(*otherPlayer, 0);
			std::cout << "Enemy didn't have any superheroes! You won!" << std::endl
				<< "Enemy lost " << otherPlayerCoinsAtStart - otherPlayer->getCoins() << " conins." << std::endl;
		}

		MyString nickname1;
		MyString nickname2;
		std::cout << "Enter nickname of your superhero(if you want to attack with random superhero enter '-'): ";
		std::cin >> nickname1;
		std::cout << "Enter nickname of your superhero(if you want to attack with random superhero enter '-'): ";
		std::cin >> nickname2;

		if (nickname1 == "-" && nickname2 == "-")
		{
			printResultOfBattle(player->attack(*otherPlayer), playerCoinsAtStart, otherPlayerCoinsAtStart, *player, *otherPlayer);
		}
		else if (nickname1 == "-")
		{
			printResultOfBattle(player->attack(*otherPlayer, nickname2), playerCoinsAtStart, otherPlayerCoinsAtStart, *player, *otherPlayer);
		}
		else if (nickname2 == "-")
		{
			printResultOfBattle(player->attack(nickname1, *otherPlayer), playerCoinsAtStart, otherPlayerCoinsAtStart, *player, *otherPlayer);
		}
		else
		{
			printResultOfBattle(player->attack(nickname1, *otherPlayer, nickname2), playerCoinsAtStart, otherPlayerCoinsAtStart, *player, *otherPlayer);
		}
	}
	catch (const File_Error&)
	{
		delete user;
		std::cerr << "File error occured when trying to attack another player!" << std::endl;
		throw;
	}
	catch (const std::invalid_argument& error)
	{
		delete user;
		std::cerr << error.what() << std::endl;
		battle();
	}
	catch (const std::logic_error& error)
	{
		delete user;
		std::cerr << error.what() << std::endl;
		return;
	}
	catch (const std::exception& error)
	{
		delete user;
		std::cerr << "An exception was thrown when trying to attack another player!" << std::endl;
		throw;
	}
	catch (...)
	{
		std::cerr << "Something went wrong when trying to attack another player!" << std::endl;
		delete user;
		throw;
	}
}

void PlayerSystem::deleteProfile()
{
	try
	{
		removeFromFile(*currentUser);

		UserFactory::freeInstance();
		SuperHeroFactory::freeInstance();

		delete currentUser;
		currentUser = nullptr;
		std::cout << "You deleted your account successfully!" << std::endl;
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

void PlayerSystem::buySuperHero() const
{
	SuperHero* superhero = nullptr;
	try
	{
		std::cout << "Enter 'back' if you want to go back;" << std::endl << "Enter the nickname of the superhero: ";
		std::cin >> buff;
		if (buff == "back")
		{
			return;
		}

		superhero = buy(buff);

		Player* player = static_cast<Player*>(currentUser);
		player->buySuperHero(*superhero);
		delete superhero;
	}
	catch (const File_Error&)
	{
		std::cerr << "File error occured when trying to buy a superhero!" << std::endl;
		delete superhero;
		throw;
	}
	catch (const std::invalid_argument& error)
	{
		delete superhero;
		std::cerr << error.what() << std::endl;
		buySuperHero();
	}
	catch (const std::logic_error& error)
	{
		delete superhero;
		std::cerr << error.what() << std::endl;
		buySuperHero();
	}
	catch (const std::exception& error)
	{
		delete superhero;
		std::cerr << "An exception was thrown when trying to buy a superhero!" << std::endl;
		throw;
	}
	catch (...)
	{
		std::cerr << "Something went wrong when trying to buy a superhero!" << std::endl;
		delete superhero;
		throw;
	}
}

void PlayerSystem::showMarket() const
{
	try
	{
		unsigned count = printSuperheroesAndGetCountOfPrinted(constants::MARKET_SUPERHEROES_FILE_PATH, constants::ENTITIES_TO_LOAD);
		if (count != 0)
		{
			std::cout << "If you want to go back type 'back';" << std::endl << "If you want to buy a superhero enter 'buy' or if you want to sell enter 'sell': ";
			std::cin >> buff;

			if (buff == "back")
			{
				return;
			}
			else if (buff == "buy")
			{
				buySuperHero();
			}
			else if (buff == "sell")
			{
				sellSuperHero();
			}
			else
			{
				throw Input_Error("Invalid keyword was entered!");
			}
		}
		else
		{
			std::cout << "No players to show!" << std::endl;
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

void PlayerSystem::showPlayers() const
{
	try
	{
		unsigned count = printPlayersAndGetCountOfPrinted(constants::ENTITIES_TO_LOAD);
		if (count != 0)
		{
			std::cout << "If you want to go back type 'back';" << std::endl << "If you want to fight with other players type 'battle': ";
			std::cin >> buff;

			if (buff == "back")
			{
				return;
			}
			else if (buff == "battle")
			{
				battle();
			}
			else
			{
				throw Input_Error("Invalid keyword was entered!");
			}
		}
		else
		{
			std::cout << "No players to show!" << std::endl;
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

void PlayerSystem::upgradeSuperHero() const
{
	try
	{
		if (static_cast<Player&>(*currentUser).getNumberOfSuperHeroes() == 0)
		{
			std::cout << "No superheroes to upgrade." << std::endl;
			return;
		}
		std::cout << "Enter 'back' if you want to go back;" << std::endl << "Enter nickname of superhero you want to upgrade: ";
		std::cin >> buff;
		if (buff == "back")
		{
			return;
		}
		static_cast<Player&>(*currentUser).powerUpSuperHero(buff);
	}
	catch (const std::invalid_argument& error)
	{
		std::cerr << error.what() << std::endl;
		upgradeSuperHero();
	}
	catch (const std::logic_error& error)
	{
		std::cerr << error.what() << std::endl;
		upgradeSuperHero();
	}
	catch (const std::exception& error)
	{
		std::cerr << "An exception was thrown when trying to upgrade a superhero!" << std::endl;
		throw;
	}
	catch (...)
	{
		std::cerr << "Something happened when trying to upgrade a superhero!" << std::endl;
		throw;
	}
}

void PlayerSystem::showProfile()
{
	try
	{
		currentUser->printFullInfo();

		std::cout << "If you want to upgrade a superhero type 'upgrade';" << std::endl
			<< "If you wat to delete your profile type 'delete': ";
		std::cin >> buff;

		if (buff == "upgrade")
		{
			upgradeSuperHero();
		}
		else if (buff == "delete")
		{
			deleteProfile();
		}
		else
		{
			throw Input_Error("Keyword is not valid!");
		}
	}
	catch (const Input_Error& error)
	{
		std::cerr << error.what() << std::endl;
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

void PlayerSystem::run()
{
	bool end = false;
	int n = -1;
	try
	{
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
						<< "  3 - show profile" << std::endl
						<< "  4 - exit" << std::endl;

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
					showProfile();
					break;
				case 4:
					logout();
					end = true;
					break;
				default:
					throw Input_Error("Keyword is not valid!");
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
					throw Input_Error("Keyword is not valid!");
				}

			}

		}
	}
	catch (const File_Error& error)
	{
		freeSystem();
		std::cerr << "FATAL ERROR:\n\tFILE_ERROR: " << error.what() << " The program will terminate!" << std::endl;
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
		std::cerr << "FATAL ERROR:\n\tUNKNOWN EXCEPTION: " << error.what() << " The program will terminate!" << std::endl;
		exit(1);
	}
	catch (...)
	{
		freeSystem();
		std::cerr << "FATAL ERROR:\n\tUNKNOWN EXCEPTION: " << "Something wrong happened!" << " The program will terminate!" << std::endl;
		exit(1);
	}
}