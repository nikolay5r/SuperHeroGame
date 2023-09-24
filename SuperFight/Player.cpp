#include <iostream>
#include <stdexcept>
#include <fstream>

#include "Player.h"
#include "SuperHero.h"
#include "Constants.h"
#include "Validation.h"
#include "Regex_Error.h"
#include "Input_Error.h"
#include "File_Error.h"
#include "HelperFunctions.h"
//#include "SystemConfigurations.h"

void Player::checkIfSuperheroIsOwnedAlready(const std::string& nickname) const
{
	if (nicknameToIndex(nickname) != std::string::npos)
	{
		throw std::logic_error("SuperHero is already owned!");
	}
}

void Player::attestIndex(size_t index) const
{
	if (index >= superHeroes.size())
	{
		throw std::invalid_argument("There is no such superhero!");
	}
}

size_t Player::nicknameToIndex(const std::string& nickname) const noexcept
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

Player::Player(const std::string& firstName, const std::string& lastName, const std::string& nickname, const std::string& email, const std::string& password)
	: User(firstName, lastName, nickname, email, password, UserRole::Player), superHeroes(constants::MAX_NUMBER_OF_SUPERHEROES_PER_PLAYER) {}

Player::Player(const User& user) : User(user) {}

Player::Player(User&& user) : User(std::move(user)) {}

void Player::addSuperHero(const SuperHero& superHero)
{
	checkIfSuperheroIsOwnedAlready(superHero.getNickname());

	if (superHeroes.size() == constants::MAX_NUMBER_OF_SUPERHEROES_PER_PLAYER)
	{
		throw std::logic_error("You reached the limit of superheroes and cannot add more! Have to sell someone first!");
	}

	superHeroes.push_back(superHero);
}

void Player::addSuperHero(SuperHero&& superHero)
{
	checkIfSuperheroIsOwnedAlready(superHero.getNickname());
	if (superHeroes.size() == constants::MAX_NUMBER_OF_SUPERHEROES_PER_PLAYER)
	{
		throw std::logic_error("You reached the limit of superheroes and cannot add more! Have to sell someone first!");
	}

	superHeroes.push_back(std::move(superHero));
}

void Player::addSuperHero(const std::string& firstName, const std::string& lastName, const std::string& nickname, unsigned power, SuperHeroPowerType powerType)
{
	checkIfSuperheroIsOwnedAlready(nickname);
	if (superHeroes.size() == constants::MAX_NUMBER_OF_SUPERHEROES_PER_PLAYER)
	{
		throw std::logic_error("You reached the limit of superheroes and cannot add more! Have to sell someone first!");
	}

	superHeroes.push_back(SuperHero(firstName, lastName, nickname, power, powerType));
}

void Player::buySuperHero(const SuperHero* superHero)
{
	checkIfSuperheroIsOwnedAlready(superHero->getNickname());
	if (coins < superHero->getPrice())
	{
		throw std::logic_error("Not enough coins! Cannot afford it!");
	}
	coins -= superHero->getPrice();
	addSuperHero(*superHero);
}

void Player::buySuperHero(const SuperHero& superHero)
{
	checkIfSuperheroIsOwnedAlready(superHero.getNickname());
	if (coins < superHero.getPrice())
	{
		throw std::logic_error("Not enough coins! Cannot afford it!");
	}
	coins -= superHero.getPrice();
	addSuperHero(superHero);
}

void Player::buySuperHero(SuperHero&& superHero)
{
	checkIfSuperheroIsOwnedAlready(superHero.getNickname());
	unsigned price = superHero.getPrice();
	if (coins < price)
	{
		throw std::logic_error("Not enough coins! Cannot afford it!");
	}
	coins -= price;
	addSuperHero(std::move(superHero));
}

void Player::buySuperHero(const std::string& firstName, const std::string& lastName, const std::string& nickname, unsigned power, SuperHeroPowerType powerType)
{
	checkIfSuperheroIsOwnedAlready(nickname);
	addSuperHero(firstName, lastName, nickname, power, powerType);
	if (coins < superHeroes[superHeroes.size() - 1].getPrice())
	{
		removeSuperHero(nickname);
		throw std::logic_error("Not enough coins! Cannot afford it!");
	}
	coins -= superHeroes[superHeroes.size() - 1].getPrice();
}

void Player::removeSuperHero(const std::string& nickname)
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

	superHeroes.erase(superHeroes.begin() + index);
}

void Player::sellSuperHero(const std::string& nickname)
{
	size_t index = nicknameToIndex(nickname);
	removeSuperHero(index);
	coins += superHeroes[index].getPrice() / 2;
}

void Player::sellSuperHero(size_t index)
{
	if (index >= superHeroes.size())
	{
		throw std::invalid_argument("Invalid index of superhero when trying to delete!");
	}
	removeSuperHero(index);
	coins += superHeroes[index].getPrice() / 2;
}

void Player::addCoinsOnLogIn()
{
	coins += constants::COINS_TO_EARN_PERIODICALLY;
}

int Player::attack(Player& defender)
{
	srand(time(0));
	size_t attackerIndex = rand() % superHeroes.size();
	size_t defenderIndex = rand() % defender.superHeroes.size();

	return attack(attackerIndex, defender, defenderIndex);
}

int Player::attack(Player& defender, size_t defenderIndex)
{
	srand(time(0));
	size_t attackerIndex = rand() % superHeroes.size();

	return attack(attackerIndex, defender, defenderIndex);
}

int Player::attack(Player& defender, const std::string& defenderNickname)
{
	size_t defenderIndex = defender.nicknameToIndex(defenderNickname);
	return attack(defender, defenderIndex);
}

int Player::attack(size_t attackerIndex, Player& defender)
{
	srand(time(0));
	size_t defenderIndex = rand() % defender.superHeroes.size();
	return attack(attackerIndex, defender, defenderIndex);
}

int Player::attack(const std::string& attackerNickname, Player& defender)
{
	size_t attackerIndex = nicknameToIndex(attackerNickname);
	return attack(attackerIndex, defender);
}

int Player::attack(size_t attackerIndex, Player& defender, size_t defenderIndex)
{
	attestIndex(attackerIndex);

	unsigned attackerPower = superHeroes[attackerIndex].getPower();

	if (defender.superHeroes.empty())
	{
		defender.coins = defender.coins < attackerPower ? 0 : defender.coins - attackerPower;
		return 2;
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

		return 1;
	}
	else if (superHeroes[attackerIndex].fight(defender.superHeroes[defenderIndex]) == 0)
	{
		coins = coins < constants::COINS_TO_LOSE_WHEN_TIE ? 0 : coins - constants::COINS_TO_LOSE_WHEN_TIE;
		return 0;
	}
	else
	{
		int resultFromAttack = (defenderPower - attackerPower) * 2;
		coins = coins < resultFromAttack ? 0 : coins - resultFromAttack;
		defender.coins += constants::COINS_TO_WIN_AS_DEFENDER;

		removeSuperHero(attackerIndex);
		defender.superHeroes[defenderIndex].gainXP();

		return -1;
	}

}

int Player::attack(const std::string& attackerNickname, Player& defender, const std::string& defenderNickname)
{
	size_t attackerIndex = nicknameToIndex(attackerNickname);
	size_t defenderIndex = defender.nicknameToIndex(defenderNickname);

	return attack(attackerIndex, defender, defenderIndex);
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
		throw std::logic_error("You don't have enough coins to upgrade the power of this superhero!");
	}
}

void Player::powerUpSuperHero(const std::string& nickname)
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
		throw std::logic_error("You don't have enough coins to level up this superhero!");
	}
}

void Player::levelUpSuperHero(const std::string& nickname)
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
		throw std::logic_error("This superhero has just attacked! You cannot change its position right now!");
	}
}

void Player::changePositionOfSuperHero(const std::string& nickname)
{
	size_t index = nicknameToIndex(nickname);
	changePositionOfSuperHero(index);
}

void Player::printFullInfo() const
{
	std::cout << "  " << fullName << " | " << nickname << " | " << email << " | " << coins << " coins" << std::endl;
	if (superHeroes.size() == 0)
	{
		std::cout << "\tNo superheroes." << std::endl;
	}
	else
	{
		for (size_t i = 0; i < superHeroes.size(); i++)
		{
			std::cout << "\t" << i + 1 << ". ";
			superHeroes[i].printFullInfo();
		}
	}
}

void Player::printShortInfo() const
{
	std::cout << "  " << nickname << " " << coins << " coins" << std::endl;
	for (size_t i = 0; i < superHeroes.size(); i++)
	{
		std::cout << "\t" << i + 1 << ". " << superHeroes[i].getNickname() << std::endl;
	}
}

size_t Player::getNumberOfSuperHeroes() const
{
	return superHeroes.size();
}

const std::vector<SuperHero>& Player::getSuperHeroes() const
{
	return superHeroes;
}

unsigned Player::getCoins() const
{
	return coins;
}

const SuperHero& Player::getSuperhero(unsigned index) const
{
	attestIndex(index);
	return superHeroes[index];
}

const SuperHero& Player::getSuperhero(const std::string& nickname) const
{
	unsigned index = nicknameToIndex(nickname);
	return getSuperhero(index);
}

void savePlayerToFile(std::ofstream& file, const Player& player)
{
	if (!file.is_open())
	{
		throw File_Error("Couldn't open database when reading a player!");
	}

	size_t size = player.getFirstName().length();
	file.write((const char*)&size, sizeof(size));
	file.write(player.getFirstName().c_str(), size + 1);

	size = player.getLastName().length();
	file.write((const char*)&size, sizeof(size));
	file.write(player.getLastName().c_str(), size + 1);

	size = player.getNickname().length();
	file.write((const char*)&size, sizeof(size));
	file.write(player.getNickname().c_str(), size + 1);

	size = player.getEmail().length();
	file.write((const char*)&size, sizeof(size));
	file.write(player.getEmail().c_str(), size + 1);

	size = player.getPassword().length();
	file.write((const char*)&size, sizeof(size));
	file.write(player.getPassword().c_str(), size + 1);

	size = player.getNumberOfSuperHeroes();
	file.write((const char*)&size, sizeof(size));

	const std::vector<SuperHero>& superheroes = player.getSuperHeroes();

	for (size_t i = 0; i < size; i++)
	{
		saveSuperheroToFile(file, superheroes[i]);
	}
}

void savePlayersToFile(const std::string& fileName, const std::vector<Player>& players)
{
	std::ofstream file(fileName.c_str(), std::ios::binary | std::ios::trunc);

	try
	{
		if (!file.is_open())
		{
			throw File_Error("Couldn't open database when saving players!");
		}

		size_t size = players.size();
		file.write((const char*)&size, sizeof(size));

		for (size_t i = 0; i < size; i++)
		{
			savePlayerToFile(file, players[i]);
		}
	}
	catch (const File_Error& err)
	{
		std::cerr << err.what() << std::endl;
		throw;
	}
	catch (const std::exception& err)
	{
		std::cerr << "Exception was thrown when trying to save multiple players!\n" << err.what() << std::endl;
		throw;
	}
	catch (...)
	{
		std::cerr << "Something went wrong when tryin to save multiple players!" << std::endl;
		throw;
	}

	file.close();
}

std::vector<Player> readPlayersFromFile(const std::string& fileName)
{
	std::ifstream file(fileName.c_str(), std::ios::binary);

	try
	{
		if (!file.is_open())
		{
			throw File_Error("Couldn't open database when reading superheroes!");
		}

		size_t size = 0;
		file.read((char*)&size, sizeof(size));

		if (size == 0)
		{
			return std::vector<Player>();
		}

		std::vector<Player> players(size);

		for (size_t i = 0; i < size; i++)
		{
			players[i] = std::move(readPlayerFromFile(file));
		}

		file.close();
		return players;
	}
	catch (const File_Error& err)
	{
		std::cerr << "File Error: " << err.what() << std::endl;
		exit(EXIT_FAILURE);
	}
	catch (const std::exception& err)
	{
		std::cerr << "Exception was thrown when trying to read multiple players!\n" << err.what() << std::endl;
		exit(EXIT_FAILURE);
	}
	catch (...)
	{
		std::cerr << "Something went wrong when trying to read multiple players!" << std::endl;
		exit(EXIT_FAILURE);
	}
}

Player readPlayerFromFile(std::ifstream& file)
{
	if (!file.is_open())
	{
		throw File_Error("Couldn't open database when reading a superhero!");
	}

	size_t size = 0;
	file.read((char*)&size, sizeof(size));
	char* firstName = new char[size + 1];
	file.read(firstName, size + 1);

	file.read((char*)&size, sizeof(size));
	char* lastName = new char[size + 1];
	file.read(lastName, size + 1);

	file.read((char*)&size, sizeof(size));
	char* nickname = new char[size + 1];
	file.read(nickname, size + 1);

	file.read((char*)&size, sizeof(size));
	char* email = new char[size + 1];
	file.read(email, size + 1);

	file.read((char*)&size, sizeof(size));
	char* password = new char[size + 1];
	file.read(password, size + 1);

	Player player(firstName, lastName, nickname, email, password);

	delete[] firstName;
	delete[] lastName;
	delete[] nickname;
	delete[] email;
	delete[] password;

	file.read((char*)&size, sizeof(size));
	for (size_t i = 0; i < size; i++)
	{
		player.addSuperHero(std::move(readSuperheroFromFile(file)));
	}

	return player;
}

Player createPlayerFromConsole()
{
	std::string firstName, lastName, nickname, email, password;

	std::cout << "Creating player: " << std::endl;
	std::cout << "    Enter first name: ";
	std::cin >> firstName;
	std::cout << "    Enter last name: ";
	std::cin >> lastName;
	std::cout << "    Enter nickname: ";
	std::cin >> nickname;
	std::cout << "    Enter email: ";
	std::cin >> email;
	std::cout << "    Enter password: ";
	std::cin >> password;
	return Player(firstName, lastName, nickname, email, password);
}