#pragma once
#include "User.h"
#include "SuperHero.h"
#include "MyString.h"
#include "Constants.h"
#include "MyVector.hpp"

class Player : public User
{
	MyVector<SuperHero> superHeroes;
	unsigned coins = constants::COINS_TO_START;

	size_t nicknameToIndex(const MyString& nickname) const noexcept;

	void checkIfSuperheroIsOwnedAlready(const MyString& nickname) const;
	void attestIndex(size_t index) const;

public:

	Player(const MyString& firstName, const MyString& lastName, const MyString& nickname, const MyString& email, const MyString& password);
	Player(const User& user);
	Player(User&& user);

	void addSuperHero(const SuperHero& superHero);
	void addSuperHero(SuperHero&& superHero);
	void addSuperHero(const MyString& firstName, const MyString& lastName, const MyString& nickname, unsigned power, SuperHeroPowerType powerType);

	void addCoinsOnLogIn();

	void buySuperHero(const SuperHero* superHero);
	void buySuperHero(const SuperHero& superHero);
	void buySuperHero(SuperHero&& superHero);
	void buySuperHero(const MyString& firstName, const MyString& lastName, const MyString& nickname, unsigned power, SuperHeroPowerType powerType);

	void removeSuperHero(const MyString& nickname);
	void removeSuperHero(size_t index);

	void sellSuperHero(const MyString& nickname);
	void sellSuperHero(size_t index);

	int attack(Player& defender);
	int attack(Player& defender, size_t defenderIndex);
	int attack(Player& defender, const MyString& defenderNickname);
	int attack(size_t attackerIndex, Player& defender);
	int attack(const MyString& attackerNickname, Player& defender);
	int attack(size_t attackerIndex, Player& defender, size_t defenderIndex);
	int attack(const MyString& attackerNickname, Player& defender, const MyString& defenderNickname);

	void powerUpSuperHero(size_t index);
	void powerUpSuperHero(const MyString& nickname);

	void levelUpSuperHero(size_t index);
	void levelUpSuperHero(const MyString& nickname);

	void changePositionOfSuperHero(size_t index);
	void changePositionOfSuperHero(const MyString& nickname);

	void printShortInfo() const override;
	void printFullInfo() const override;
	unsigned getCoins() const;
	size_t getNumberOfSuperHeroes() const;
	const SuperHero& getSuperhero(unsigned index) const;
	const SuperHero& getSuperhero(const MyString& nickname) const;
	const MyVector<SuperHero>& getSuperHeroes() const;

	friend class PlayerFactory;
};

class PlayerFactory : public UserFactory
{
	static UserFactory* instance;
	PlayerFactory() = default;
public:
	static UserFactory* getInstance();

	PlayerFactory(const PlayerFactory&) = delete;
	PlayerFactory& operator=(const PlayerFactory&) = delete;

	User* readFromBinary() const override;
	User* readFromBinary(std::ifstream& file) const override;
	User* readFromBinary(const MyString& nicknameToFind) const override;
	User* readFromBinary(std::ifstream& file, const MyString& nicknameToFind) const override;

	User* createFromConsole() const override;

	static void freeInstance();
	~PlayerFactory() = default;
};

void savePlayerToFile(const Player& player);
void removePlayerFromFile(const MyString& nickname);
void removePlayerFromFile(const Player& player);
void printPlayers();
void printPlayersForAdmins();
void savePlayerChangesToFile(const Player& player);