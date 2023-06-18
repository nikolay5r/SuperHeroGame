#include "PlayerSystem.h"
#include "System.h"
#include "File_Error.h"
#include "Input_Error.h"
#include "SystemConfigurations.h"
#include "Regex_Error.h"
#include "Player.h"
#include "SuperHero.h"
#include <iostream>

System* PlayerSystem::getInstance()
{
	if (!System::instance)
	{
		System::instance = new PlayerSystem();
	}

	return System::instance;
}

void PlayerSystem::logout()
{
	try
	{
		savePlayerChangesToFile(static_cast<const Player&>(*currentUser));

		AdminFactory::freeInstance();
		PlayerFactory::freeInstance();
		SuperHeroFactory::freeInstance();

		delete currentUser;
		currentUser = nullptr;

		std::cout << "You logged out." << std::endl;
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
		std::cout << "Log in was successful!" << std::endl;
		if (configs::saveLoggedPlayerInThePeriod(currentUser->getNickname()))
		{
			static_cast<Player*>(currentUser)->addCoinsOnLogIn();
			std::cout << "You recieved coins on log in! +" << constants::COINS_TO_EARN_PERIODICALLY << " coins" << std::endl;
			configs::handlePeriod();
		}
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

void PlayerSystem::reg()
{
	try
	{
		UserFactory* factory = PlayerFactory::getInstance();
		User* user = factory->createFromConsole();
		currentUser = user;
		srand(time(0));
		Player* player = static_cast<Player*>(user);
		player->addSuperHero(user->getFirstName(), user->getLastName(), user->getNickname(), rand() % 30 + 5, SuperHeroPowerType::Earth);
		savePlayerToFile(*player);
		saveSuperheroToFile(constants::SOLD_SUPERHEROES_FILE_PATH, player->getSuperHeroes()[0]);
		configs::incrementCountOfPlayers();
		configs::incrementCountOfSoldSuperheroes();
		std::cout << "Registration was successful!" << std::endl;
		std::cout << "You will recieve coins as a price periodically on log in!" << std::endl;
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

void PlayerSystem::sellSuperHero() const
{
	try
	{
		Player* player = static_cast<Player*>(currentUser);
		if (player->getNumberOfSuperHeroes() == 0)
		{
			throw std::logic_error("You have no superheroes to sell!");
		}
		MyString buff;
		std::cout << "Enter 'back' if you want to go back;" << std::endl << "Enter nickname of the superhero you want to sell: ";
		std::cin >> buff;
		if (buff == "back")
		{
			return;
		}

		sell(player->getSuperhero(buff));
		player->sellSuperHero(buff);
		savePlayerChangesToFile(*player);
		configs::incrementCountOfMarketSuperheroes();
		configs::decrementCountOfSoldSuperheroes();

		std::cout << "Superhero was sold!" << std::endl;
	}
	catch (const File_Error& error)
	{
		std::cerr << "File error occured when trying to sell player!" << std::endl;
		throw;
	}
	catch (const std::invalid_argument& error)
	{
		std::cerr << error.what() << std::endl;
		std::cout << std::endl;
		sellSuperHero();
	}
	catch (const std::logic_error& error)
	{
		std::cerr << error.what() << std::endl;
		std::cout << std::endl;
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

static void printResultOfBattle(int result)
{
	switch (result)
	{
	case 1:
		std::cout << "You won!" << std::endl;
		break;
	case 0:
		std::cout << "It was a tie!" << std::endl;
		break;
	case -1:
		std::cout << "You lost!" << std::endl;
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
		MyString buff;
		std::cin >> buff;
		if (buff == "back")
		{
			return;
		}
		else if (buff == player->getNickname())
		{
			throw std::logic_error("You cannot fight yourself!");
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
		std::cout << "Enter nickname of enemy superhero(if you want to attack with random superhero enter '-'): ";
		std::cin >> nickname2;


		if (nickname1 == "-" && nickname2 == "-")
		{
			printResultOfBattle(player->attack(*otherPlayer));
		}
		else if (nickname1 == "-")
		{
			printResultOfBattle(player->attack(*otherPlayer, nickname2));
		}
		else if (nickname2 == "-")
		{
			printResultOfBattle(player->attack(nickname1, *otherPlayer));
		}
		else
		{
			printResultOfBattle(player->attack(nickname1, *otherPlayer, nickname2));
		}

		saveChangesToFile(*player);
		saveChangesToFile(*otherPlayer);
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
		std::cout << std::endl;
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
		configs::decrementCountOfPlayers();

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

void PlayerSystem::buySuperHero() const
{
	SuperHero* superhero = nullptr;
	try
	{
		std::cout << "Enter 'back' if you want to go back;" << std::endl << "Enter the nickname of the superhero: ";
		MyString buff;
		std::cin >> buff;
		if (buff == "back")
		{
			std::cout << std::endl;
			return;
		}

		superhero = buy(buff);

		Player* player = static_cast<Player*>(currentUser);
		player->buySuperHero(*superhero);
		delete superhero;

		savePlayerChangesToFile(*player);

		configs::decrementCountOfMarketSuperheroes();
		configs::incrementCountOfSoldSuperheroes();

		std::cout << "Superhero was bought!" << std::endl;
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
		std::cout << std::endl;
		buySuperHero();
	}
	catch (const std::logic_error& error)
	{
		delete superhero;
		std::cerr << error.what() << std::endl;
		std::cout << std::endl;
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
		if (!configs::isMarketEmpty())
		{
			printSuperheroes(constants::MARKET_SUPERHEROES_FILE_PATH);
			MyString buff;
			std::cout << "If you want to go back enter 'back';" << std::endl
				<< "If you want to buy a superhero enter 'buy';" << std::endl
				<< "If you want to sell enter 'sell':" << std::endl
				<< "Command: ";
			std::cin >> buff;

			if (buff == "back")
			{
				return;
			}
			else if (buff == "buy")
			{
				std::cout << std::endl;
				buySuperHero();
			}
			else if (buff == "sell")
			{
				std::cout << std::endl;
				sellSuperHero();
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

void PlayerSystem::showPlayers() const
{
	try
	{
		if (configs::getCountOfPlayers() != 0)
		{
			printPlayers();
			MyString buff;
			std::cout << "If you want to go back type 'back';" << std::endl
				<< "If you want to fight with other players type 'battle': " << std::endl
				<< "Command: ";
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

			showPlayers();
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
		MyString buff;
		std::cout << "Enter 'back' if you want to go back;" << std::endl << "Enter nickname of superhero you want to upgrade: ";
		std::cin >> buff;
		if (buff == "back")
		{
			return;
		}
		static_cast<Player&>(*currentUser).powerUpSuperHero(buff);
		std::cout << "Superhero was upgraded!" << std::endl;
		saveChangesToFile(*currentUser);
	}
	catch (const std::invalid_argument& error)
	{
		std::cerr << error.what() << std::endl;
		upgradeSuperHero();
		std::cout << std::endl;
	}
	catch (const std::logic_error& error)
	{
		std::cerr << error.what() << std::endl;
		upgradeSuperHero();
		std::cout << std::endl;
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

void PlayerSystem::changePos()
{
	if (static_cast<Player*>(currentUser)->getNumberOfSuperHeroes() == 0)
	{
		throw std::logic_error("You have no superheroes to change position to.");
	}

	MyString buff;
	std::cout << "Enter 'back' if you want to go back;" << std::endl << "Enter the nickname of the superhero: ";
	std::cin >> buff;
	if (buff == "back")
	{
		return;
	}
	static_cast<Player*>(currentUser)->changePositionOfSuperHero(buff);
	std::cout << "Position changed!" << std::endl;
	saveChangesToFile(*currentUser);
}

void PlayerSystem::showProfile()
{
	try
	{
		currentUser->printFullInfo();

		std::cout << std::endl << "If you want to go back enter 'back';" << std::endl
			<< "If you want to sell a superhero enter 'sell';" << std::endl
			<< "If you want to change the attacking position of a superhero enter 'change';" << std::endl
			<< "If you want to upgrade a superhero enter 'upgrade';" << std::endl
			<< "If you want to delete your profile enter 'delete': " << std::endl
			<< "Command: ";
		MyString buff;
		std::cin >> buff;

		if (buff == "back")
		{
			return;
		}
		else if (buff == "change")
		{
			std::cout << std::endl;
			changePos();
		}
		else if (buff == "upgrade")
		{
			std::cout << std::endl;
			upgradeSuperHero();
		}
		else if (buff == "sell")
		{
			std::cout << std::endl;
			sellSuperHero();
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

void PlayerSystem::run()
{
	bool end = false;
	int n = -1;
	try
	{
		configs::initConfigs();
		while (!end)
		{
			if (currentUser)
			{
				std::cout << std::endl;
				std::cout << "Enter a command:" << std::endl
					<< "  0 - logout" << std::endl
					<< "  1 - show all players" << std::endl
					<< "  2 - show market" << std::endl
					<< "  3 - show profile" << std::endl
					<< "  4 - exit" << std::endl
					<< "Command: ";

				std::cin >> n;

				switch (n)
				{
				case 0:
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
					showProfile();
					break;
				case 4:
					logout();
					exit(0);
					break;
				default:
					throw Input_Error("Keyword is not valid!");
				}
			}
			else
			{
				std::cout << "Enter 'back' if you want to go back;" << std::endl
					<< "Enter 'login' if you already have an account and 'register' if you don't: " << std::endl
					<< "Command: ";
				MyString buff;
				std::cin >> buff;

				if (buff == "login")
				{
					std::cout << std::endl;
					login();
				}
				else if (buff == "register")
				{
					std::cout << std::endl;
					reg();
				}
				else if (buff == "back")
				{
					end = true;
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
		std::cerr << "FATAL ERROR:\n\tUNKNOWN EXCEPTION: " << "Something wrong happened!" << " Logging out!" << std::endl;
		exit(1);
	}

	std::cout << "Program is shutting down!" << std::endl;
}
