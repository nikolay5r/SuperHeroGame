#include "SuperHero.h"
#include "MyString.h"
#include "Validation.h"
#include "Constants.h"

#include <cstdlib>

const unsigned SuperHero::xpNeededPerLevel[10] = { 3, 5, 10, 20, 30, 40, 50, 75, 90, 100 };

static int isPowerGreater(unsigned attackerPower, unsigned defenderPower)
{
	return attackerPower >= defenderPower ? (attackerPower == defenderPower ? 0 : 1) : -1;
}

void SuperHero::setFirstName(const MyString& firstName)
{
	validation::isNameValid(firstName);
	this->firstName = firstName;
}

void SuperHero::setLastName(const MyString& lastName)
{
	validation::isNameValid(lastName);
	this->lastName = lastName;
}

void SuperHero::setFullName() noexcept
{
	fullName = firstName + " " + lastName;
}

void SuperHero::setNickname(const MyString& nickname)
{
	validation::isNicknameValid(nickname);
	this->nickname = nickname;
}

void SuperHero::setPrice() noexcept
{
	price = power * 3 / 2 - 23;
}

void SuperHero::setPower(unsigned long long power)
{
	validation::isPowerValid(power);
	this->power = power;
}

SuperHero::SuperHero(const MyString& firstName, const MyString& lastName, const MyString& nickname, unsigned power, SuperHeroPowerType powerType)
{
	setFirstName(firstName);
	setLastName(lastName);
	setFullName();
	setNickname(nickname);
	this->powerType = powerType;
	setPower(power);
	setPrice();
}

unsigned SuperHero::getPower() const noexcept
{
	return power;
}

unsigned SuperHero::getPrice() const noexcept
{
	return price;
}

SuperHeroPowerType SuperHero::getPowerType() const noexcept
{
	return powerType;
}

const MyString& SuperHero::getFirstName() const noexcept
{
	return firstName;
}

const MyString& SuperHero::getLastName() const noexcept
{
	return lastName;
}

const MyString& SuperHero::getFullName() const noexcept
{
	return fullName;
}

const MyString& SuperHero::getNickname() const noexcept
{
	return nickname;
}

SuperHeroPosition SuperHero::getPosition() const noexcept
{
	return position;
}
void SuperHero::levelUp()
{
	if (level < constants::MAX_LEVEL && ((level >= 10 && xp >= xpNeededPerLevel[9]) || (level < 10 && xp >= xpNeededPerLevel[level - 1])))
	{
		if (level >= 10)
		{
			xp -= xpNeededPerLevel[9];
		}
		else
		{
			xp -= xpNeededPerLevel[level - 1];
		}
		level++;
		(allowedPowerUpgrades += 1) *= 1.5;
	}
	else
	{
		//TODO: 
		throw std::exception("You cannot level up your hero!");
	}
}

int SuperHero::fight(const SuperHero& other) const noexcept
{
	if ((powerType == SuperHeroPowerType::Fire && other.powerType == SuperHeroPowerType::Water) ||
		(powerType == SuperHeroPowerType::Water && other.powerType == SuperHeroPowerType::Earth) ||
		(powerType == SuperHeroPowerType::Earth && other.powerType == SuperHeroPowerType::Fire))
	{
		return isPowerGreater(power, other.power * 2);
	}

	if ((powerType == SuperHeroPowerType::Fire && other.powerType == SuperHeroPowerType::Earth) ||
		(powerType == SuperHeroPowerType::Water && other.powerType == SuperHeroPowerType::Fire) ||
		(powerType == SuperHeroPowerType::Earth && other.powerType == SuperHeroPowerType::Water))
	{
		return isPowerGreater(power * 2, other.power);
	}

	return isPowerGreater(power, other.power);
}

void SuperHero::powerUp()
{
	if (powerLevel == allowedPowerUpgrades)
	{
		//TODO:
		throw std::exception("You cannot upgrade your superhero right now!");
	}

	powerLevel++;
	power += constants::POWER_TO_ADD;
}

void SuperHero::changePosition() noexcept
{
	position == SuperHeroPosition::Attack ? SuperHeroPosition::Defense : SuperHeroPosition::Attack;
}

void SuperHero::gainXP() noexcept
{
	srand(time(0));
	xp += rand() % 5 + 1;
}

uint8_t SuperHero::getPowerLevel() const noexcept
{
	return powerLevel;
}

uint8_t SuperHero::getLevel() const noexcept
{
	return level;
}