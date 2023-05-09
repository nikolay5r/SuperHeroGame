#pragma once
#include "User.h"
#include "SuperHero.h"
#include "MyString.h"

class Player : User
{
	SuperHero** superHeroes = nullptr;
	size_t capacity = 4;
	size_t numberOfSuperHeroes = 0;

	void copyFrom(const Player& other);
	void moveFrom(Player&& other) noexcept;
	void free();
	void resize(size_t newCapacity);

public:

	Player(const MyString& firstName, const MyString& lastName, const MyString& username, const MyString& email, const MyString& password);
	Player(const Player& other);
	Player(Player&& other) noexcept;

	Player& operator=(const Player& other);
	Player& operator=(Player&& other) noexcept;

	~Player();
};

