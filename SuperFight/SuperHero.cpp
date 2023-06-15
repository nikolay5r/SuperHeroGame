#include "SuperHero.h"
#include "MyString.h"
#include "Validation.h"
#include "Constants.h"
#include <iostream>
#include <fstream>
#include <cstdlib>
#include "File_Error.h"

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
	price = constants::INITIAL_PRICE_OF_SUPERHERO * level * (powerLevel + 1) * 0.55;
}

void SuperHero::setPower(unsigned long long power)
{
	if ((level == 1 && powerLevel == 0 && power > constants::MAX_POWER_ON_FIRST_LEVEL) || power < constants::MIN_POWER)
	{
		throw std::invalid_argument("Invalid power!");
	}
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
	hasAttacked = true;

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
	if (level != constants::MAX_LEVEL)
	{
		srand(time(0));
		xp += rand() % 5 + 1;
	}
}

uint8_t SuperHero::getPowerLevel() const noexcept
{
	return powerLevel;
}

uint8_t SuperHero::getLevel() const noexcept
{
	return level;
}

bool SuperHero::getAttackInfo() const noexcept
{
	return hasAttacked;
}

void SuperHero::print() const
{
	std::cout << nickname << " | " << power << " | ";
	switch (powerType)
	{
	case SuperHeroPowerType::Air:
		std::cout << "Air" << " | ";
		break;
	case SuperHeroPowerType::Water:
		std::cout << "Water" << " | ";
		break;
	case SuperHeroPowerType::Earth:
		std::cout << "Earth" << " | ";
		break;
	case SuperHeroPowerType::Fire:
		std::cout << "Fire" << " | ";
		break;
	default:
		throw std::logic_error("Something went wrong with the power type!");
		break;
	}

	std::cout << price << " coins | " << level << " level | " << powerLevel << " power level" << std::endl;
}

uint8_t SuperHero::getXP() const noexcept
{
	return xp;
}

uint8_t SuperHero::getAllowedPowerUpgrades() const noexcept
{
	return allowedPowerUpgrades;
}

/// 

SuperHeroFactory* SuperHeroFactory::getInstance()
{
	if (!instance)
	{
		SuperHeroFactory::instance = new SuperHeroFactory();
	}

	return SuperHeroFactory::instance;
}


SuperHeroFactory* SuperHeroFactory::instance = nullptr;

SuperHero* SuperHeroFactory::readFromBinary(std::ifstream& file) const
{
	if (!file.is_open())
	{
		throw File_Error("File couldn't open!");
	}

	size_t n;
	file.read((char*)&n, sizeof(n));
	char* firstName = new char[n + 1];
	file.read(firstName, n + 1);

	file.read((char*)&n, sizeof(n));
	char* lastName = new char[n + 1];
	file.read(lastName, n + 1);

	file.read((char*)&n, sizeof(n));
	char* nickname = new char[n + 1];
	file.read(nickname, n + 1);

	unsigned power = 0;
	file.read((char*)&power, sizeof(power));
	unsigned temp = 0;
	file.read((char*)&temp, sizeof(temp));
	SuperHeroPowerType type = static_cast<SuperHeroPowerType>(temp);

	SuperHero* superhero = new SuperHero(firstName, lastName, nickname, power, type);

	file.read((char*)&temp, sizeof(temp));
	superhero->position = static_cast<SuperHeroPosition>(temp);

	file.read((char*)&superhero->price, sizeof(superhero->price));
	file.read((char*)&superhero->level, sizeof(superhero->level));
	file.read((char*)&superhero->xp, sizeof(superhero->xp));
	file.read((char*)&superhero->powerLevel, sizeof(superhero->powerLevel));
	file.read((char*)&superhero->allowedPowerUpgrades, sizeof(superhero->allowedPowerUpgrades));

	return superhero;
}

void SuperHeroFactory::readFromBinary(std::ifstream&, const MyString& nickname) const
{
	return;
}

void SuperHeroFactory::createFromConsole() const
{
	return;
}

void SuperHeroFactory::freeInstance()
{
	delete SuperHeroFactory::instance;
	SuperHeroFactory::instance = nullptr;
}

SuperHeroFactory::~SuperHeroFactory()
{
	delete SuperHeroFactory::instance;
	SuperHeroFactory::instance = nullptr;
}

void saveToFile(std::ofstream& file, const SuperHero& superhero)
{
	std::ofstream nicknamesFile(constants::OWNED_SUPERHEROES_NICKNAMES_FILE_PATH.c_str(), std::ios::binary);

	if (!file.is_open() || !nicknamesFile.is_open())
	{
		throw File_Error("File couldn't open!");
	}

	int index = file.tellp();

	size_t size = superhero.getFirstName().length();
	file.write((const char*)&size, sizeof(size));
	file.write(superhero.getFirstName().c_str(), size + 1);

	size = superhero.getLastName().length();
	file.write((const char*)&size, sizeof(size));
	file.write(superhero.getLastName().c_str(), size + 1);

	size = superhero.getNickname().length();
	file.write((const char*)&size, sizeof(size));
	file.write(superhero.getNickname().c_str(), size + 1);

	nicknamesFile.write((const char*)&size, sizeof(size));
	nicknamesFile.write(superhero.getNickname().c_str(), size + 1);
	nicknamesFile.write((const char*)&index, sizeof(index));

	unsigned p = superhero.getPower();
	file.write((const char*)&p, sizeof(p));

	SuperHeroPowerType type = superhero.getPowerType();
	file.write((const char*)&type, sizeof(type));

	SuperHeroPosition pos = superhero.getPosition();
	file.write((const char*)&pos, sizeof(pos));

	p = superhero.getPrice();
	file.write((const char*)&p, sizeof(p));

	uint8_t n = superhero.getLevel();
	file.write((const char*)&n, sizeof(n));

	n = superhero.getXP();
	file.write((const char*)&n, sizeof(n));

	n = superhero.getPowerLevel();
	file.write((const char*)&n, sizeof(n));

	n = superhero.getAllowedPowerUpgrades();
	file.write((const char*)&n, sizeof(n));
}