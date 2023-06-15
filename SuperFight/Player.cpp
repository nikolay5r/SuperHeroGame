#include "Player.h"
#include "SuperHero.h"
#include "Constants.h"
#include "Validation.h"
#include <iostream>
#include <stdexcept>
#include <fstream>
#include "File_Error.h"
#include "HelperFunctions.h"

bool Player::attestIndex(size_t index) const
{
	if (index >= superHeroes.size())
	{
		throw std::invalid_argument("There is no such superhero! Error occured when trying to attack!");
	}
}

size_t Player::nicknameToIndex(const MyString& nickname) const noexcept
{
	size_t size = superHeroes.size();

	for (size_t i = 0; i < size; i++)
	{
		if (superHeroes[i].getNickname() == nickname)
		{
			return i;
		}
	}

	return std::string::npos;
}

Player::Player(const MyString& firstName, const MyString& lastName, const MyString& nickname, const MyString& email, const MyString& password)
	: User(firstName, lastName, nickname, email, password, UserRole::Player) {}

Player::Player(const User& user) : User(user) {}

Player::Player(User&& user) : User(std::move(user)) {}

void Player::addSuperHero(const SuperHero& superHero)
{
	if (superHeroes.size() == constants::MAX_NUMBER_OF_SUPERHEROES_PER_PLAYER)
	{
		throw std::logic_error("You reached the limit of superheroes and cannot add more! Have to sell someone first!");
	}

	superHeroes.push_back(superHero);
}

void Player::addSuperHero(SuperHero&& superHero)
{
	if (superHeroes.size() == constants::MAX_NUMBER_OF_SUPERHEROES_PER_PLAYER)
	{
		throw std::logic_error("You reached the limit of superheroes and cannot add more! Have to sell someone first!");
	}

	superHeroes.push_back(std::move(superHero));
}

void Player::addSuperHero(const MyString& firstName, const MyString& lastName, const MyString& nickname, unsigned power, SuperHeroPowerType powerType)
{
	if (superHeroes.size() == constants::MAX_NUMBER_OF_SUPERHEROES_PER_PLAYER)
	{
		throw std::logic_error("You reached the limit of superheroes and cannot add more! Have to sell someone first!");
	}

	superHeroes.push_back(SuperHero(firstName, lastName, nickname, power, powerType));
}

void Player::buySuperHero(const SuperHero* superHero)
{
	if (coins < superHero->getPrice())
	{
		throw std::logic_error("Not enough coins! Cannot afford it!");
	}
	coins -= superHero->getPrice();
	addSuperHero(*superHero);
}

void Player::buySuperHero(const SuperHero& superHero)
{
	if (coins < superHero.getPrice())
	{
		throw std::logic_error("Not enough coins! Cannot afford it!");
	}
	coins -= superHero.getPrice();
	addSuperHero(superHero);
}

void Player::buySuperHero(SuperHero&& superHero)
{
	unsigned price = superHero.getPrice();
	if (coins < price)
	{
		//TODO
		throw std::logic_error("Not enough coins! Cannot afford it!");
	}
	coins -= price;
	addSuperHero(std::move(superHero));
}

void Player::buySuperHero(const MyString& firstName, const MyString& lastName, const MyString& nickname, unsigned power, SuperHeroPowerType powerType)
{
	addSuperHero(firstName, lastName, nickname, power, powerType);
	if (coins < superHeroes[superHeroes.size() - 1].getPrice())
	{
		removeSuperHero(superHeroes.size() - 1);
		throw std::logic_error("Not enough coins! Cannot afford it!");
	}
	coins -= superHeroes[superHeroes.size() - 1].getPrice();
}

void Player::removeSuperHero(const MyString& nickname)
{
	size_t index = nicknameToIndex(nickname);
	removeSuperHero(index);
}

void Player::removeSuperHero(size_t index)
{
	if (index >= superHeroes.size())
	{
		throw std::invalid_argument("Invalid index of superhero when trying to delete!");
	}

	superHeroes.pop_at(index);
}

void Player::sellSuperHero(const MyString& nickname)
{
	size_t index = nicknameToIndex(nickname);
	coins += superHeroes[index].getPrice() / 2;
	removeSuperHero(nickname);
}

void Player::sellSuperHero(size_t index)
{
	if (index >= superHeroes.size())
	{
		throw std::invalid_argument("Invalid index of superhero when trying to delete!");
	}
	coins += superHeroes[index].getPrice() / 2;
	removeSuperHero(index);
}

void Player::attack(Player& defender)
{
	srand(time(0));
	size_t attackerIndex = rand() % superHeroes.size();
	size_t defenderIndex = rand() % defender.superHeroes.size();

	attack(attackerIndex, defender, defenderIndex);
}

void Player::attack(Player& defender, size_t defenderIndex)
{
	srand(time(0));
	size_t attackerIndex = rand() % superHeroes.size();

	attack(attackerIndex, defender, defenderIndex);
}

void Player::attack(Player& defender, const MyString& defenderNickname)
{
	size_t defenderIndex = defender.nicknameToIndex(defenderNickname);
	attack(defender, defenderIndex);
}

void Player::attack(size_t attackerIndex, Player& defender)
{
	srand(time(0));
	size_t defenderIndex = rand() % defender.superHeroes.size();
	attack(attackerIndex, defender, defenderIndex);
}

void Player::attack(const MyString& attackerNickname, Player& defender)
{
	size_t attackerIndex = nicknameToIndex(attackerNickname);
	attack(attackerIndex, defender);
}

void Player::attack(size_t attackerIndex, Player& defender, size_t defenderIndex)
{
	attestIndex(attackerIndex);

	unsigned attackerPower = superHeroes[attackerIndex].getPower();

	if (defender.superHeroes.isEmpty())
	{
		defender.coins = defender.coins < attackerPower ? 0 : defender.coins - attackerPower;
	}

	defender.attestIndex(defenderIndex);

	unsigned defenderPower = defender.superHeroes[defenderIndex].getPower();

	if (superHeroes[attackerIndex].fight(defender.superHeroes[defenderIndex]) == 1)
	{
		int resultFromAttack = attackerPower - defenderPower;

		if (defender.superHeroes[defenderIndex].getPosition() == SuperHeroPosition::Defense)
		{
			defender.coins = defender.coins < resultFromAttack ? 0 : defender.coins - resultFromAttack;
		}
		coins += resultFromAttack;

		superHeroes[attackerIndex].gainXP();
		defender.removeSuperHero(defenderIndex);
	}
	else if (superHeroes[attackerIndex].fight(defender.superHeroes[defenderIndex]) == 0)
	{
		coins = coins < constants::COINS_TO_LOSE_WHEN_TIE ? 0 : coins - constants::COINS_TO_LOSE_WHEN_TIE;
	}
	else
	{
		int resultFromAttack = (defenderPower - attackerPower) * 2;
		coins = coins < resultFromAttack ? 0 : coins - resultFromAttack;
		defender.coins += constants::COINS_TO_WIN_AS_DEFENDER;

		removeSuperHero(attackerIndex);
		defender.superHeroes[defenderIndex].gainXP();
	}

}

void Player::attack(const MyString& attackerNickname, Player& defender, const MyString& defenderNickname)
{
	size_t attackerIndex = nicknameToIndex(attackerNickname);
	size_t defenderIndex = nicknameToIndex(defenderNickname);

	attack(attackerIndex, defender, defenderIndex);
}

void Player::powerUpSuperHero(size_t index)
{
	attestIndex(index);

	unsigned price = constants::INITIAL_PRICE_OF_POWER_UP * (superHeroes[index].getPowerLevel() + 1) * superHeroes[index].getLevel() / 2;

	if (coins >= price)
	{
		coins -= price;
		superHeroes[index].powerUp();
	}
	else
	{
		//TODO:
		throw std::exception("You don't have enough coins to upgrade the power of this superhero!");
	}
}

void Player::powerUpSuperHero(const MyString& nickname)
{
	size_t index = nicknameToIndex(nickname);
	powerUpSuperHero(index);
}

void Player::levelUpSuperHero(size_t index)
{
	attestIndex(index);
	unsigned price = constants::INITIAL_PRICE_OF_POWER_UP * superHeroes[index].getLevel() * 1.2;

	if (coins >= price)
	{
		coins -= price;
		superHeroes[index].levelUp();
	}
	else
	{
		//TODO:
		throw std::exception("You don't have enough coins to level up this superhero!");
	}
}

void Player::levelUpSuperHero(const MyString& nickname)
{
	size_t index = nicknameToIndex(nickname);
	powerUpSuperHero(index);
}

void Player::changePositionOfSuperHero(size_t index)
{
	attestIndex(index);
	if (!superHeroes[index].getAttackInfo())
	{
		superHeroes[index].changePosition();
	}
	else
	{
		//TODO:
		throw std::exception("This superhero has just attacked! You cannot change its position right now!");
	}
}

void Player::changePositionOfSuperHero(const MyString& nickname)
{
	size_t index = nicknameToIndex(nickname);
	changePositionOfSuperHero(index);
}

void Player::print() const
{
	std::cout << nickname << " " << coins << " coins" << std::endl;
	for (size_t i = 0; i < superHeroes.size(); i++)
	{
		std::cout << "\t" << i << ". " << superHeroes[i].getNickname() << std::endl;
	}
}

size_t Player::getNumberOfSuperHeroes() const
{
	return superHeroes.size();
}

const MyVector<SuperHero>& Player::getSuperHeroes() const
{
	return superHeroes;
}

User* PlayerFactory::readFromBinary(std::ifstream& file) const
{
	if (!file.is_open())
	{
		throw File_Error("File couldn't open!");
	}

	size_t n;
	file.read((char*)&n, sizeof(n));
	char* firstName = new char[n + 1];
	file.read(firstName, n + 1);

	file.read((char*)&n, sizeof(n));
	char* lastName = new char[n + 1];
	file.read(lastName, n + 1);

	file.read((char*)&n, sizeof(n));
	char* nickname = new char[n + 1];
	file.read(nickname, n + 1);

	file.read((char*)&n, sizeof(n));
	char* email = new char[n + 1];
	file.read(email, n + 1);

	file.read((char*)&n, sizeof(n));
	char* password = new char[n + 1];
	file.read(password, n + 1);

	Player* player = new Player(firstName, lastName, nickname, email, password);

	delete[] firstName;
	delete[] lastName;
	delete[] nickname;
	delete[] email;
	delete[] password;

	size_t count;
	file.read((char*)&count, sizeof(count));
	SuperHeroFactory* factory = SuperHeroFactory::getInstance();
	SuperHero* superhero;
	for (size_t i = 0; i < count; i++)
	{
		superhero = factory->readFromBinary(file);
		player->addSuperHero(*superhero);
		delete superhero;
	}

	return player;
}

User* PlayerFactory::readFromBinary(std::ifstream& file, const MyString& nicknameToFind) const
{
	if (!file.is_open())
	{
		throw File_Error("File couldn't open!");
	}

	while (!file.eof())
	{
		User* curr = readFromBinary(file);
		if (file.eof())
		{
			break;
		}
		if (curr->getNickname() == nicknameToFind)
		{
			return curr;
		}
	}

	throw std::invalid_argument("Nickname is not valid!");
}

UserFactory* PlayerFactory::getInstance()
{
	if (UserFactory::instance == nullptr)
	{
		UserFactory::instance = new PlayerFactory();
	}

	return UserFactory::instance;
}

User* PlayerFactory::createFromConsole() const
{
	MyString firstName;
	MyString lastName;
	MyString nickname;
	MyString email;
	MyString password;

	std::cout << "Enter first name of the player: ";
	std::cin >> firstName;
	validation::isNameValid(firstName);

	std::cout << "Enter last name of the player: ";
	std::cin >> lastName;
	validation::isNameValid(lastName);

	std::cout << "Enter nickname of the player: ";
	std::cin >> nickname;
	validation::isNicknameValid(nickname);

	std::cout << "Enter password for the player: ";
	std::cin >> password;
	validation::isPasswordValid(password);

	std::cout << "Enter email of the player: ";
	std::cin >> email;
	validation::isEmailValid(email);

	try
	{
		std::ifstream file(constants::PLAYERS_FILE_PATH.c_str(), std::ios::binary);
		readFromBinary(file, nickname);
		file.close();
	}
	catch (const std::invalid_argument&)
	{
		return new Player(firstName, lastName, nickname, email, password);
	}

	throw std::invalid_argument("User with that nickname already exists!");
}

void saveToFile(const Player& player)
{
	std::ofstream playersFile(constants::PLAYERS_FILE_PATH.c_str(), std::ios::binary | std::ios::app);

	if (!playersFile.is_open())
	{
		throw File_Error("File couldn't open!");
	}

	int index = playersFile.tellp();

	size_t size = player.getFirstName().length();
	playersFile.write((const char*)&size, sizeof(size));
	playersFile.write(player.getFirstName().c_str(), size + 1);

	size = player.getLastName().length();
	playersFile.write((const char*)&size, sizeof(size));
	playersFile.write(player.getLastName().c_str(), size + 1);

	size = player.getNickname().length();
	playersFile.write((const char*)&size, sizeof(size));
	playersFile.write(player.getNickname().c_str(), size + 1);

	size = player.getEmail().length();
	playersFile.write((const char*)&size, sizeof(size));
	playersFile.write(player.getEmail().c_str(), size + 1);

	size = player.getPassword().length();
	playersFile.write((const char*)&size, sizeof(size));
	playersFile.write(player.getPassword().c_str(), size + 1);

	size = player.getNumberOfSuperHeroes();
	playersFile.write((const char*)&size, sizeof(size));

	const MyVector<SuperHero>& superheroes = player.getSuperHeroes();

	for (size_t i = 0; i < size; i++)
	{
		saveToFile(playersFile, superheroes[i]);
	}
}


void removeFromFile(const Player& player)
{
	int indexStart = -1;
	int indexEnd = -1;

	helper::getStartIndexAndEndIndexOfEntityInFile(constants::PLAYERS_FILE_PATH, indexStart, indexEnd, player);
	helper::deleteDataFromFile(constants::PLAYERS_FILE_PATH, indexStart, indexEnd);
}