#include "SuperHero.h"
#include "MyString.h"
#include "Validation.h"

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

void SuperHero::setFullName()
{
	fullName = firstName + " " + lastName;
}

void SuperHero::setNickname(const MyString& nickname)
{
	validation::isNicknameValid(nickname);
	this->nickname = nickname;
}

void SuperHero::setPrice()
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

unsigned SuperHero::getPower() const
{
	return power;
}

unsigned SuperHero::getPrice() const
{
	return price;
}

SuperHeroPowerType SuperHero::getPowerType() const
{
	return powerType;
}

const MyString& SuperHero::getFirstName() const
{
	return firstName;
}

const MyString& SuperHero::getLastName() const
{
	return lastName;
}

const MyString& SuperHero::getFullName() const
{
	return fullName;
}

const MyString& SuperHero::getNickname() const
{
	return nickname;
}

void SuperHero::levelUp()
{
	if (xp >= xpNeededPerLevel[9] || xp >= xpNeededPerLevel[level - 1])
	{
		level++;
		if (level >= 10)
		{
			xp -= xpNeededPerLevel[9];
		}
		else
		{
			xp -= xpNeededPerLevel[level - 1];
		}
	}
	else
	{
		//TODO: 
		throw std::exception("You cannot level up your hero! Not enough xp!");
	}
}

int SuperHero::fight(const SuperHero& other)
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

}