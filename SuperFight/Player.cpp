#include "Player.h"
#include "SuperHero.h"
#include "Constants.h"
#include "Validation.h"
#include <iostream>
#include <stdexcept>
#include <fstream>
#include "File_Error.h"

static unsigned findPlayerIndexInFileThroughUsername(const MyString& usernameToFind)
{
	std::ifstream usernamesFile("playerUsernames.bin", std::ios::binary);

	unsigned indexInPlayersFile = 0;
	while (!usernamesFile.eof())
	{
		size_t length = 0;
		usernamesFile.read((char*)&length, sizeof(length));
		char* username = new char[length + 1];
		usernamesFile.read(username, length + 1);
		usernamesFile.read((char*)&indexInPlayersFile, sizeof(indexInPlayersFile));

		if (usernameToFind == username)
		{
			usernamesFile.close();
			delete[] username;
			return indexInPlayersFile;
		}

		delete[] username;

		if (usernamesFile.eof())
		{
			break;
		}
	}

	usernamesFile.close();
	throw std::invalid_argument("Username is not valid exists!");
}

static size_t getFileSize(std::ifstream& file)
{
	size_t curr = file.tellg();
	file.seekg(0, std::ios::end);
	size_t size = file.tellg();
	file.seekg(curr);

	return size;
}

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

Player::Player(const MyString& firstName, const MyString& lastName, const MyString& username, const MyString& email, const MyString& password)
	: User(firstName, lastName, username, email, password, UserRole::Player) {}

Player::Player(const User& user) : User(user) {}

Player::Player(User&& user) : User(std::move(user)) {}

void Player::addSuperHero(const SuperHero& superHero)
{
	superHeroes.push_back(superHero);
}

void Player::addSuperHero(SuperHero&& superHero)
{
	superHeroes.push_back(std::move(superHero));
}

void Player::addSuperHero(const MyString& firstName, const MyString& lastName, const MyString& nickname, unsigned power, SuperHeroPowerType powerType)
{
	superHeroes.push_back(SuperHero(firstName, lastName, nickname, power, powerType));
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
	std::cout << username << " | " << fullName << " | " << email << " | " << superHeroes.size() << " | " << coins;
}

User* PlayerFactory::readFromBinary(std::ifstream& file) const
{
	if (file.is_open())
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
	char* username = new char[n + 1];
	file.read(username, n + 1);

	file.read((char*)&n, sizeof(n));
	char* email = new char[n + 1];
	file.read(email, n + 1);

	file.read((char*)&n, sizeof(n));
	char* password = new char[n + 1];
	file.read(password, n + 1);

	Player* player = new Player(firstName, lastName, username, email, password);

	delete[] firstName;
	delete[] lastName;
	delete[] username;
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

User* PlayerFactory::readFromBinaryByIndex(std::ifstream& file, unsigned index) const
{
	if (!file.is_open())
	{
		throw File_Error("File couldn't open!");
		return nullptr;
	}

	unsigned fileIndex = file.tellg();

	if (index < getFileSize(file))
	{
		throw std::invalid_argument("Given index is invalid! Bigger than size of file!");
	}

	file.seekg(index);
	User* user = readFromBinary(file);
	file.seekg(fileIndex);

	return user;
}

User* PlayerFactory::readFromBinaryByUsername(std::ifstream& file, const MyString& usernameToFind) const
{
	if (!file.is_open())
	{
		throw File_Error("File couldn't open!");
	}

	unsigned index = findPlayerIndexInFileThroughUsername(usernameToFind);

	return readFromBinaryByIndex(file, index);
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
	MyString username;
	MyString email;
	MyString password;

	std::cout << "Enter first name of the player: ";
	std::cin >> firstName;
	validation::isNameValid(firstName);

	std::cout << "Enter last name of the player: ";
	std::cin >> lastName;
	validation::isNameValid(lastName);

	std::cout << "Enter username of the player: ";
	std::cin >> username;
	validation::isUsernameValid(username);

	std::cout << "Enter password for the player: ";
	std::cin >> password;
	validation::isPasswordValid(password);

	std::cout << "Enter email of the player: ";
	std::cin >> email;
	validation::isEmailValid(email);

	return new Player(firstName, lastName, username, email, password);
}
