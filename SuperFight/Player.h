#pragma once
#include "User.h"
#include "SuperHero.h"
#include <cstring>
#include "Constants.h"
#include <vector>

class Player : public User
{
	std::vector<SuperHero> superHeroes;
	unsigned coins = constants::COINS_TO_START;

	size_t nicknameToIndex(const std::string& nickname) const noexcept;

	void checkIfSuperheroIsOwnedAlready(const std::string& nickname) const;
	void attestIndex(size_t index) const;

public:

	Player(const std::string& firstName, const std::string& lastName, const std::string& nickname, const std::string& email, const std::string& password);
	Player(const User& user);
	Player(User&& user);

	void addSuperHero(const SuperHero& superHero);
	void addSuperHero(SuperHero&& superHero);
	void addSuperHero(const std::string& firstName, const std::string& lastName, const std::string& nickname, unsigned power, SuperHeroPowerType powerType);

	void addCoinsOnLogIn();

	void buySuperHero(const SuperHero* superHero);
	void buySuperHero(const SuperHero& superHero);
	void buySuperHero(SuperHero&& superHero);
	void buySuperHero(const std::string& firstName, const std::string& lastName, const std::string& nickname, unsigned power, SuperHeroPowerType powerType);

	void removeSuperHero(const std::string& nickname);
	void removeSuperHero(size_t index);

	void sellSuperHero(const std::string& nickname);
	void sellSuperHero(size_t index);

	int attack(Player& defender);
	int attack(Player& defender, size_t defenderIndex);
	int attack(Player& defender, const std::string& defenderNickname);
	int attack(size_t attackerIndex, Player& defender);
	int attack(const std::string& attackerNickname, Player& defender);
	int attack(size_t attackerIndex, Player& defender, size_t defenderIndex);
	int attack(const std::string& attackerNickname, Player& defender, const std::string& defenderNickname);

	void powerUpSuperHero(size_t index);
	void powerUpSuperHero(const std::string& nickname);

	void levelUpSuperHero(size_t index);
	void levelUpSuperHero(const std::string& nickname);

	void changePositionOfSuperHero(size_t index);
	void changePositionOfSuperHero(const std::string& nickname);

	void printShortInfo() const override;
	void printFullInfo() const override;
	unsigned getCoins() const;
	size_t getNumberOfSuperHeroes() const;
	const SuperHero& getSuperhero(unsigned index) const;
	const SuperHero& getSuperhero(const std::string& nickname) const;
	const std::vector<SuperHero>& getSuperHeroes() const;
};

std::vector<Player> readPlayersFromFile(const std::string& fileName);
Player readPlayerFromFile(std::ifstream& file);
Player createPlayerFromConsole();
void savePlayerToFile(std::ofstream& file, const Player& player);
void savePlayersToFile(const std::string& fileName, const std::vector<Player>& players);