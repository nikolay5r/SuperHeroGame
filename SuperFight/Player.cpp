#include "Player.h"
#include <iostream>

void Player::copyFrom(const Player& other)
{
	numberOfSuperHeroes = other.numberOfSuperHeroes;
	capacity = other.capacity;

	superHeroes = new SuperHero * [numberOfSuperHeroes] { nullptr };
	for (size_t i = 0; i < numberOfSuperHeroes; i++)
	{
		superHeroes[i] = new SuperHero(*other.superHeroes[i]);
	}
}

void Player::moveFrom(Player&& other) noexcept
{
	superHeroes = other.superHeroes;
	capacity = other.capacity;
	numberOfSuperHeroes = other.numberOfSuperHeroes;
	other.superHeroes = nullptr;
}

void Player::free()
{
	if (superHeroes != nullptr)
	{
		for (size_t i = 0; i < numberOfSuperHeroes; i++)
		{
			if (superHeroes[i] != nullptr)
			{
				delete superHeroes[i];
			}
		}

		delete[] superHeroes;
		capacity = 0;
		numberOfSuperHeroes = 0;
	}

}

void Player::resize(size_t newCapacity)
{
	SuperHero** newSuperheroes = new SuperHero * [newCapacity] {nullptr};

	for (size_t i = 0; i < numberOfSuperHeroes; i++)
	{
		newSuperheroes[i] = superHeroes[i];
		superHeroes[i] = nullptr;
	}

	delete[] superHeroes;

	superHeroes = newSuperheroes;
}

Player::Player(const MyString& firstName, const MyString& lastName, const MyString& username, const MyString& email, const MyString& password)
	: User(firstName, lastName, username, email, password) {}

Player::Player(const Player& other) : User(other)
{
	copyFrom(other);
}

Player::Player(Player&& other) noexcept : User(std::move(other))
{
	moveFrom(std::move(other));
}

Player& Player::operator=(const Player& other)
{
	if (this != &other)
	{
		User::operator=(other);
		free();
		copyFrom(other);
	}

	return *this;
}

Player& Player::operator=(Player&& other) noexcept
{
	if (this != &other)
	{
		User::operator=(std::move(other));
		free();
		moveFrom(std::move(other));
	}

	return *this;
}

Player::~Player()
{
	free();
}