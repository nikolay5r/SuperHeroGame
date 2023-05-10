#include "Player.h"
#include "SuperHero.h"
#include "Constants.h"
#include <iostream>
#include <stdexcept>

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
	SuperHero** newSuperHeroes = new SuperHero * [newCapacity] {nullptr};

	for (size_t i = 0; i < numberOfSuperHeroes; i++)
	{
		newSuperHeroes[i] = superHeroes[i];
		superHeroes[i] = nullptr;
	}

	delete[] superHeroes;

	superHeroes = newSuperHeroes;
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

void Player::addSuperHero(const SuperHero& superHero)
{
	if (numberOfSuperHeroes == capacity)
	{
		resize(capacity * 2);
	}

	superHeroes[numberOfSuperHeroes++] = new SuperHero(superHero);
}

void Player::removeSuperHero(size_t index)
{
	if (index >= numberOfSuperHeroes)
	{
		throw std::out_of_range("Index is not valid");
	}

	if (superHeroes[index])
	{
		delete superHeroes[index];
		superHeroes[index] = superHeroes[--numberOfSuperHeroes];
		superHeroes[numberOfSuperHeroes + 1] = nullptr;
	}

	if (numberOfSuperHeroes < capacity / 4)
	{
		resize(capacity / 2);
	}
}