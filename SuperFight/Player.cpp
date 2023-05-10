#include "Player.h"
#include "SuperHero.h"
#include "Constants.h"
#include <iostream>
#include <stdexcept>

bool Player::isIndexValid(size_t index) const
{
	if (index >= numberOfSuperHeroes)
	{
		throw std::invalid_argument("There is no such superhero! Error occured when trying to attack!");
	}
}

size_t Player::nicknameToIndex(const MyString& nickname) const noexcept
{
	for (size_t i = 0; i < numberOfSuperHeroes; i++)
	{
		if (superHeroes[i]->getNickname() == nickname)
		{
			return i;
		}
	}

	return std::string::npos;
}

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
	: User(firstName, lastName, username, email, password, UserRole::Player) {}

Player::Player(const User& user) : User(user) {}

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

void Player::removeSuperHero(const MyString& nickname)
{
	size_t index = nicknameToIndex(nickname);
	removeSuperHero(index);
}

void Player::removeSuperHero(size_t index)
{
	if (index >= numberOfSuperHeroes)
	{
		throw std::invalid_argument("Invalid index of superhero when trying to delete!");
	}

	if (superHeroes[index])
	{
		delete superHeroes[index];
		superHeroes[index] = superHeroes[numberOfSuperHeroes - 1];
		superHeroes[numberOfSuperHeroes--] = nullptr;
	}

	if (numberOfSuperHeroes < capacity / 4)
	{
		resize(capacity / 2);
	}
}

void Player::attack(Player& defender)
{
	srand(time(0));
	size_t attackerIndex = rand() % numberOfSuperHeroes;
	size_t defenderIndex = rand() % defender.numberOfSuperHeroes;

	attack(attackerIndex, defender, defenderIndex);
}

void Player::attack(Player& defender, size_t defenderIndex)
{
	srand(time(0));
	size_t attackerIndex = rand() % numberOfSuperHeroes;

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
	size_t defenderIndex = rand() % defender.numberOfSuperHeroes;
	attack(attackerIndex, defender, defenderIndex);
}

void Player::attack(const MyString& attackerNickname, Player& defender)
{
	size_t attackerIndex = nicknameToIndex(attackerNickname);
	attack(attackerIndex, defender);
}

void Player::attack(size_t attackerIndex, Player& defender, size_t defenderIndex)
{
	isIndexValid(attackerIndex);

	unsigned attackerPower = superHeroes[attackerIndex]->getPower();

	if (defender.numberOfSuperHeroes == 0)
	{
		defender.coins = defender.coins < attackerPower ? 0 : defender.coins - attackerPower;
	}

	defender.isIndexValid(defenderIndex);

	unsigned defenderPower = defender.superHeroes[defenderIndex]->getPower();

	if (superHeroes[attackerIndex]->fight(*defender.superHeroes[defenderIndex]) == 1)
	{
		int resultFromAttack = attackerPower - defenderPower;

		if (defender.superHeroes[defenderIndex]->getPosition() == SuperHeroPosition::Defense)
		{
			defender.coins = defender.coins < resultFromAttack ? 0 : defender.coins - resultFromAttack;
		}
		coins += resultFromAttack;

		superHeroes[attackerIndex]->gainXP();
		defender.removeSuperHero(defenderIndex);
	}
	else if (superHeroes[attackerIndex]->fight(*defender.superHeroes[defenderIndex]) == 0)
	{
		coins = coins < constants::COINS_TO_LOSE_WHEN_TIE ? 0 : coins - constants::COINS_TO_LOSE_WHEN_TIE;
	}
	else
	{
		int resultFromAttack = (defenderPower - attackerPower) * 2;
		coins = coins < resultFromAttack ? 0 : coins - resultFromAttack;
		defender.coins += constants::COINS_TO_WIN_AS_DEFENDER;

		removeSuperHero(attackerIndex);
		defender.superHeroes[defenderIndex]->gainXP();
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
	isIndexValid(index);

	unsigned price = constants::INITIAL_PRICE_OF_POWER_UP * superHeroes[index]->getPowerLevel() * superHeroes[index]->getLevel() / 2;

	if (coins >= price)
	{
		coins -= price;
		superHeroes[index]->powerUp();
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
	isIndexValid(index);
	unsigned price = constants::INITIAL_PRICE_OF_POWER_UP * superHeroes[index]->getLevel() * 1.2;

	if (coins >= price)
	{
		coins -= price;
		superHeroes[index]->levelUp();
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