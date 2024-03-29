#include "SuperHero.h"
#include "MyString.h"
#include "Validation.h"
#include "Constants.h"
#include <iostream>
#include <fstream>
#include <cstdlib>
#include "File_Error.h"
#include "HelperFunctions.h"
#include "Entity.h"

const unsigned SuperHero::xpNeededPerLevel[10] = { 3, 5, 10, 20, 30, 40, 50, 75, 90, 100 };

static int isPowerGreater(unsigned attackerPower, unsigned defenderPower)
{
	return attackerPower >= defenderPower ? (attackerPower == defenderPower ? 0 : 1) : -1;
}

void SuperHero::setPrice() noexcept
{
	srand(time(0));
	price = constants::INITIAL_PRICE_OF_SUPERHERO * level * (powerLevel + 1) * 0.55 + rand() % 10;
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
	: Entity(firstName, lastName, nickname)
{
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
		throw std::logic_error("Superhero reached its maximum of allowed power upgrades!");
	}

	powerLevel++;
	power += constants::POWER_TO_ADD;
}

void SuperHero::changePosition() noexcept
{
	position = position == SuperHeroPosition::Attack ? SuperHeroPosition::Defense : SuperHeroPosition::Attack;
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

void SuperHero::printFullInfo() const
{
	std::cout << fullName << " | " << nickname << " | " << power << " power" << " | ";
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

	switch (position)
	{
	case SuperHeroPosition::Attack:
		std::cout << "Attack" << " | ";
		break;
	case SuperHeroPosition::Defense:
		std::cout << "Defense" << " | ";
		break;
	default:
		throw std::logic_error("Something went wrong with the position type!");
		break;
	}

	std::cout << price << " coins | " << (unsigned)level << " level | " << (unsigned)powerLevel << " power level | " << (unsigned)xp
		<< "/" << (level >= 10 ? xpNeededPerLevel[9] : xpNeededPerLevel[level - 1]) << " xp." << std::endl;
}

void SuperHero::printShortInfo() const
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

	std::cout << price << " coins | " << (unsigned)level << " level | " << (unsigned)powerLevel << " power level" << std::endl;
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

SuperHero* SuperHeroFactory::readFromBinary(const MyString& fileName, const MyString& nickname) const
{
	std::ifstream file(fileName.c_str(), std::ios::binary);
	SuperHero* superhero = nullptr;
	try
	{
		if (!file.is_open())
		{
			throw File_Error("File couldn't open!");
		}

		superhero = readFromBinary(file, nickname);
		file.close();
		return superhero;
	}
	catch (const File_Error&)
	{
		std::cerr << "File error occured when trying to read superhero from binary file by nickname!" << std::endl;
		delete superhero;
		file.close();
		throw;
	}
	catch (...)
	{
		delete superhero;
		file.close();
		throw;
	}
}

SuperHero* SuperHeroFactory::readFromBinary(std::ifstream& file, const MyString& nickname) const
{
	if (!file.is_open())
	{
		throw File_Error("File couldn't open!");
	}

	SuperHero* curr = nullptr;
	try
	{
		while (!helper::isEOF(file))
		{
			curr = readFromBinary(file);
			if (curr->getNickname() == nickname)
			{
				return curr;
			}
			delete curr;
			curr = nullptr;
		}
		throw std::invalid_argument("Nickname is not valid!");
	}
	catch (const File_Error&)
	{
		std::cerr << "File error occured when trying to read superhero from opened binary file by nickname!" << std::endl;
		delete curr;
		throw;
	}
	catch (...)
	{
		delete curr;
		throw;
	}
}

SuperHero* SuperHeroFactory::createFromConsole() const
{
	MyString firstName;
	MyString lastName;
	MyString nickname;
	unsigned power = 0;
	unsigned typeAsUnsigned = 0;
	SuperHeroPowerType type;

	std::cout << "Enter first name of the superhero: ";
	std::cin >> firstName;
	validation::isNameValid(firstName);

	std::cout << "Enter last name of the superhero: ";
	std::cin >> lastName;
	validation::isNameValid(lastName);

	std::cout << "Enter nickname of the superhero: ";
	std::cin >> nickname;
	validation::isNicknameValid(nickname);

	std::cout << "Enter power of the superhero: ";
	std::cin >> power;
	validation::isPowerValid(power);

	std::cout << "Enter power type of the superhero:" << std::endl
		<< "  - 0 for Air;" << std::endl
		<< "  - 1 for Water;" << std::endl
		<< "  - 2 for Earth;" << std::endl
		<< "  - 3 for Fire;" << std::endl
		<< "Power type: ";
	std::cin >> typeAsUnsigned;

	switch (typeAsUnsigned)
	{
	case 0:
		type = SuperHeroPowerType::Air;
		break;
	case 1:
		type = SuperHeroPowerType::Water;
		break;
	case 2:
		type = SuperHeroPowerType::Earth;
		break;
	case 3:
		type = SuperHeroPowerType::Fire;
		break;
	default:
		throw std::invalid_argument("The power type you entered is not valid!");
		break;
	}

	return new SuperHero(firstName, lastName, nickname, power, type);
}

void SuperHeroFactory::freeInstance()
{
	delete SuperHeroFactory::instance;
	SuperHeroFactory::instance = nullptr;
}

void saveSuperheroToFile(std::ofstream& file, const SuperHero& superhero)
{
	if (!file.is_open())
	{
		throw File_Error("File couldn't open!");
	}

	size_t size = strlen(superhero.getFirstName().c_str());
	file.write((const char*)&size, sizeof(size));
	file.write(superhero.getFirstName().c_str(), size + 1);

	size = strlen(superhero.getLastName().c_str());
	file.write((const char*)&size, sizeof(size));
	file.write(superhero.getLastName().c_str(), size + 1);

	size = strlen(superhero.getNickname().c_str());
	file.write((const char*)&size, sizeof(size));
	file.write(superhero.getNickname().c_str(), size + 1);

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

SuperHero* buy(const MyString& nickname)
{
	SuperHero* superhero = nullptr;
	try
	{
		SuperHeroFactory* factory = SuperHeroFactory::getInstance();
		superhero = factory->readFromBinary(constants::MARKET_SUPERHEROES_FILE_PATH, nickname);
		removeSuperheroFromFile(constants::MARKET_SUPERHEROES_FILE_PATH, *superhero);
		saveSuperheroToFile(constants::SOLD_SUPERHEROES_FILE_PATH, *superhero);

		return superhero;
	}
	catch (const File_Error&)
	{
		std::cerr << "File error occured when trying to execute buy algorithm!" << std::endl;
		delete superhero;
		throw;
	}
	catch (...)
	{
		delete superhero;
		throw;
	}
}

void removeSuperheroFromFile(const MyString& fileName, const MyString& nickname)
{
	SuperHero* superhero = nullptr;
	try
	{
		SuperHeroFactory* factory = SuperHeroFactory::getInstance();
		superhero = factory->readFromBinary(fileName, nickname);
		removeSuperheroFromFile(fileName, *superhero);
		delete superhero;
	}
	catch (const File_Error&)
	{
		std::cerr << "File error occured when trying to remove a superhero from file by nickname!" << std::endl;
		delete superhero;
		throw;
	}
	catch (...)
	{
		delete superhero;
		throw;
	}
}

void removeSuperheroFromFile(const MyString& fileName, const SuperHero& superhero)
{
	int indexStart = -1;
	int indexEnd = -1;
	helper::getStartIndexAndEndIndexOfEntityInFile(fileName, indexStart, indexEnd, superhero);
	helper::deleteDataFromFile(fileName, indexStart, indexEnd);
}

void saveSuperheroToFile(const MyString& fileName, const SuperHero& superhero)
{
	std::ofstream file(fileName.c_str(), std::ios::binary | std::ios::app);

	if (!file.is_open())
	{
		throw File_Error("File couldn't open!");
	}

	saveSuperheroToFile(file, superhero);

	file.close();
}

void sell(const SuperHero& superheroToSell)
{
	try
	{
		saveSuperheroToFile(constants::MARKET_SUPERHEROES_FILE_PATH, superheroToSell);
		removeSuperheroFromFile(constants::SOLD_SUPERHEROES_FILE_PATH, superheroToSell);
	}
	catch (const File_Error&)
	{
		std::cerr << "File error occured when trying to execute sell algorithm!" << std::endl;
		throw;
	}
	catch (...)
	{
		throw;
	}
}
void printSuperheroes(const MyString& fileName)
{
	std::ifstream file(fileName.c_str(), std::ios::in | std::ios::binary);;

	if (!file.is_open())
	{
		throw File_Error("File couldn't open!");
	}

	unsigned countOfPrintedSuperheroes = 0;
	try
	{
		for (int i = 1; !helper::isEOF(file); i++)
		{
			std::cout << i << ". ";
			SuperHeroFactory* factory = SuperHeroFactory::getInstance();
			SuperHero* superhero = factory->readFromBinary(file);
			superhero->printShortInfo();
			countOfPrintedSuperheroes++;
			delete superhero;
		}
		std::cout << std::endl;
		file.close();
	}
	catch (const File_Error&)
	{
		std::cerr << "File error occured when trying to execute print superheroes algorithm!" << std::endl;
		file.close();
		throw;
	}
	catch (...)
	{
		file.close();
		throw;
	}
}