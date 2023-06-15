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

	bool attestIndex(size_t index) const;

public:

	Player(const MyString& firstName, const MyString& lastName, const MyString& nickname, const MyString& email, const MyString& password);
	Player(const User& user);
	Player(User&& user);

	void addSuperHero(const SuperHero& superHero);
	void addSuperHero(SuperHero&& superHero);
	void addSuperHero(const MyString& firstName, const MyString& lastName, const MyString& nickname, unsigned power, SuperHeroPowerType powerType);
	void removeSuperHero(const MyString& nickname);
	void removeSuperHero(size_t index);

	void attack(Player& defender);
	void attack(Player& defender, size_t defenderIndex);
	void attack(Player& defender, const MyString& defenderNickname);
	void attack(size_t attackerIndex, Player& defender);
	void attack(const MyString& attackerNickname, Player& defender);
	void attack(size_t attackerIndex, Player& defender, size_t defenderIndex);
	void attack(const MyString& attackerNickname, Player& defender, const MyString& defenderNickname);

	void powerUpSuperHero(size_t index);
	void powerUpSuperHero(const MyString& nickname);

	void levelUpSuperHero(size_t index);
	void levelUpSuperHero(const MyString& nickname);

	void changePositionOfSuperHero(size_t index);
	void changePositionOfSuperHero(const MyString& nickname);

	void print() const override;
	size_t getNumberOfSuperHeroes() const;
	const MyVector<SuperHero>& getSuperHeroes() const;

	friend class PlayerFactory;
};

class PlayerFactory : public UserFactory
{
	PlayerFactory() = default;
public:
	static UserFactory* getInstance();

	PlayerFactory(const PlayerFactory&) = delete;
	PlayerFactory& operator=(const PlayerFactory&) = delete;

	User* readFromBinary(std::ifstream& file) const override;
	User* readFromBinary(std::ifstream& file, const MyString& nicknameToFind) const override;
	User* createFromConsole() const override;
};

void saveToFile(const Player& player);
void removeFromFile(const Player& player);