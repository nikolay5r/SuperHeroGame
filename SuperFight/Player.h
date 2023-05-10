#pragma once
#include "User.h"
#include "SuperHero.h"
#include "MyString.h"
#include "Constants.h"

class Player : User
{
	SuperHero** superHeroes = nullptr;
	size_t capacity = 2;
	size_t numberOfSuperHeroes = 0;
	unsigned coins = constants::COINS_TO_START;

	void copyFrom(const Player& other);
	void moveFrom(Player&& other) noexcept;
	void free();
	void resize(size_t newCapacity);

	size_t nicknameToIndex(const MyString& nickname) const noexcept;

	bool isIndexValid(size_t index) const;

public:

	Player(const MyString& firstName, const MyString& lastName, const MyString& username, const MyString& email, const MyString& password);
	Player(const User& user);
	Player(const Player& other);
	Player(Player&& other) noexcept;

	Player& operator=(const Player& other);
	Player& operator=(Player&& other) noexcept;

	void addSuperHero(const SuperHero& superHero);
	void removeSuperHero(const MyString& nickname);
	void removeSuperHero(size_t index);
	void attack(Player& defender);
	void attack(Player& defender, size_t defenderIndex);
	void attack(Player& defender, const MyString& defenderNickname);
	void attack(size_t attackerIndex, Player& defender);
	void attack(const MyString& attackerNickname, Player& defender);
	void attack(size_t attackerIndex, Player& defender, size_t defenderIndex);
	void attack(const MyString& attackerNickname, Player& defender, const MyString& defenderNickname);

	~Player();
};

