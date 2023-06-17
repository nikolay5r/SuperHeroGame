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

static MyString buff;

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
	//try
	removeFromFile(*currentUser);
	saveToFile(*currentUser);

	UserFactory::freeInstance();
	SuperHeroFactory::freeInstance();

	delete currentUser;
	currentUser = nullptr;
}

void PlayerSystem::login()
{
	//try

	//move to factory
	std::ifstream file(constants::PLAYERS_FILE_PATH.c_str(), std::ios::binary);

	if (!file.is_open())
	{
		throw File_Error("File couldn't open!");
	}

	MyString temp;
	std::cout << "Enter nickname: ";
	std::cin >> temp;
	validation::isNicknameValid(temp);

	UserFactory* factory = PlayerFactory::getInstance();
	User* user = factory->readFromBinary(file, temp);
	std::cout << "Enter password: ";
	std::cin >> temp;

	if (user->getPassword() != temp)
	{
		delete user;
		file.close();
		//TODO: password error
		return;
	}

	removeFromFile(*user);
	currentUser = user;
	file.close();
}

void PlayerSystem::reg()
{
	//try
	UserFactory* factory = PlayerFactory::getInstance();
	User* user = factory->createFromConsole();
	currentUser = user;
	srand(time(0));
	static_cast<Player*>(user)->addSuperHero(user->getFirstName(), user->getLastName(), user->getNickname(), rand() % 30 + 5, SuperHeroPowerType::Earth);
}

void PlayerSystem::sellSuperHero() const
{
	//try
	Player* player = static_cast<Player*>(currentUser);
	if (player->getNumberOfSuperHeroes() == 0)
	{
		throw std::logic_error("You have no superheroes to sell!");
	}
	std::cout << "Enter nickname of the superhero you want to sell: ";
	std::cin >> buff;
	player->sellSuperHero(buff);
	sell(buff);
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
	Player* player = static_cast<Player*>(currentUser);
	if (player->getNumberOfSuperHeroes() == 0)
	{
		throw std::logic_error("You have no superheroes! You have to buy at least to start fighting!");
	}

	std::cout << "Enter player you want to attack: ";
	std::cin >> buff;
	UserFactory* factory = PlayerFactory::getInstance();
	User* user = factory->readFromBinary(buff);
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

void PlayerSystem::deleteProfile()
{
	//try
	removeFromFile(*currentUser);

	UserFactory::freeInstance();
	SuperHeroFactory::freeInstance();

	delete currentUser;
	currentUser = nullptr;
	std::cout << "You deleted your account successfully!" << std::endl;
}

void PlayerSystem::buySuperHero() const
{
	SuperHero* superhero = nullptr;
	//try
	std::cout << "Enter the nickname of the superhero: ";
	std::cin >> buff;

	superhero = buy(buff);

	Player* player = static_cast<Player*>(currentUser);
	player->buySuperHero(*superhero);
	delete superhero;
}

void PlayerSystem::showMarket() const
{
	//try
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
			superhero->printShortInfo();
			delete superhero;
		}
		if (file.eof())
		{
			break;
		}
		else
		{
			std::cout << "If you want to see more superheroes enter 'forward' if you want to go back enter 'back';" << std::endl
				<< "If you want to buy a superhero enter 'buy' or if you want to sell enter 'sell': ";
			std::cin >> buff;
			if (buff == "forward")
			{
				continue;
			}
			else if (buff == "back")
			{
				break;
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
				throw std::invalid_argument("Invalid keyword was entered!");
			}
		}
	}

	file.close();
}

void PlayerSystem::showPlayers() const
{
	//try
	std::ifstream file(constants::PLAYERS_FILE_PATH.c_str(), std::ios::in | std::ios::binary);;


	while (!file.eof())
	{
		unsigned count = 0;
		for (size_t i = 0; i < constants::ENTITIES_TO_LOAD; i++)
		{
			if (file.eof())
			{
				break;
			}
			UserFactory* factory = PlayerFactory::getInstance();
			User* player = factory->readFromBinary(file);
			player->printShortInfo();
			count++;
			delete player;
		}
		if (file.eof())
		{
			break;
		}
		else if (count != 0)
		{
			if (count == constants::ENTITIES_TO_LOAD)
			{
				std::cout << "If you want to see more players enter 'forward' or if you want to go back enter 'back';" << std::endl;
			}
			std::cout << "If you want to fight with other players type 'battle': ";
			std::cin >> buff;
			if (buff == "forward")
			{
				continue;
			}
			else if (buff == "back")
			{
				break;
			}
			else if (buff == "battle")
			{
				battle();
			}
			else
			{
				//error
				throw std::invalid_argument("Invalid keyword was entered!");
			}
		}
		else
		{
			std::cout << "No players to print.";
		}
	}

	file.close();
}

void PlayerSystem::upgradeSuperHero() const
{
	std::cout << "Enter nickname of superhero you want to upgrade: ";
	std::cin >> buff;
	static_cast<Player&>(*currentUser).powerUpSuperHero(buff);
}

void PlayerSystem::showProfile()
{
	currentUser->printFullInfo();

	if (static_cast<Player*>(currentUser)->getNumberOfSuperHeroes() != 0)
	{
		std::cout << "If you want to upgrade a superhero type 'upgrade';" << std::endl;
	}
	std::cout << "If you wat to delete your profile type 'delete': ";
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
		//error
	}
}

void PlayerSystem::run()
{
	bool end = false;
	int n = -1;
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