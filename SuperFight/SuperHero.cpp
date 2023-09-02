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
#include "Regex_Error.h"
#include "Input_Error.h"


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

void saveSuperheroToFile(std::ofstream& file, const SuperHero& superhero)
{
	if (!file.is_open())
	{
		throw File_Error("Something went wrong when saving a superhero!");
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

void saveSuperheroesToFile(const MyString& fileName, const MyVector<SuperHero>& superheroes)
{
	std::ofstream file(fileName.c_str(), std::ios::binary | std::ios::trunc);

	try
	{
		if (!file.is_open())
		{
			throw File_Error("Couldn't open database when saving superheroes!");
		}

		size_t size = superheroes.size();
		file.write((const char*)&size, sizeof(size));

		for (size_t i = 0; i < size; i++)
		{
			saveSuperheroToFile(file, superheroes[i]);
		}
	}
	catch (const File_Error& err)
	{
		std::cerr << err.what() << std::endl;
		exit(EXIT_FAILURE);
	}
	catch (const std::exception& err)
	{
		std::cerr << "Error caught when trying to save multiple superheroes!\n" << err.what() << std::endl;
		exit(EXIT_FAILURE);
	}
	catch (...)
	{
		std::cerr << "Something went wrong when tryin to save multiple superheroes!" << std::endl;
		exit(EXIT_FAILURE);
	}

	file.close();
}

SuperHero readSuperheroFromFile(std::ifstream& file)
{
	if (!file.is_open())
	{
		throw File_Error("Couldn't open database when reading a superhero!");
	}

	size_t size = 0;
	file.read((char*)&size, sizeof(size));
	char* firstName = new char[size + 1];
	file.read(firstName, size + 1);

	file.read((char*)&size, sizeof(size));
	char* lastName = new char[size + 1];
	file.read(lastName, size + 1);

	file.read((char*)&size, sizeof(size));
	char* nickname = new char[size + 1];
	file.read(nickname, size + 1);

	unsigned power = 0;
	file.read((char*)&power, sizeof(p));

	SuperHeroPowerType type;
	file.read((char*)&type, sizeof(type));

	SuperHero superhero(firstName, lastName, nickname, power, type);
	file.read((char*)&superhero.position, sizeof(superhero.position));
	file.read((char*)&superhero.price, sizeof(superhero.price));
	file.read((char*)&superhero.level, sizeof(superhero.level));
	file.read((char*)&superhero.xp, sizeof(superhero.xp));
	file.read((char*)&superhero.powerLevel, sizeof(superhero.powerLevel));
	file.read((char*)&superhero.allowedPowerUpgrades, sizeof(superhero.allowedPowerUpgrades));

	return superhero;
}

MyVector<SuperHero> readSuperheroesFromFile(const MyString& fileName)
{
	std::ifstream file(fileName.c_str(), std::ios::binary);

	try
	{
		if (!file.is_open())
		{
			throw File_Error("Couldn't open database when reading superheroes!");
		}

		size_t size = 0;
		file.read((char*)&size, sizeof(size));

		MyVector<SuperHero> superheroes(size);

		for (size_t i = 0; i < size; i++)
		{
			superheroes[i] = std::move(readSuperheroFromFile(file));
		}
	}
	catch (const File_Error& err)
	{
		std::cerr << "File Error: " << err.what() << std::endl;
		exit(EXIT_FAILURE);
	}
	catch (const std::exception& err)
	{
		std::cerr << "Error caught when trying to read multiple superheroes!\n" << err.what() << std::endl;
		exit(EXIT_FAILURE);
	}
	catch (...)
	{
		std::cerr << "Something went wrong when trying to read multiple superheroes!" << std::endl;
		exit(EXIT_FAILURE);
	}

	file.close();
}

SuperHero createSuperheroFromConsole()
{
	MyString firstName, lastName, nickname;
	unsigned long long power, type;
	try
	{
		std::cout << "Creating superhero: " << std::endl;
		std::cout << "    Enter first name: ";
		std::cin >> firstName;

		validation::nameValidation(firstName);
		std::cout << "    Enter last name: ";
		std::cin >> lastName;
		validation::nameValidation(lastName);
		std::cout << "    Enter nickname: ";
		std::cin >> nickname;
		validation::nicknameValidation(nickname);
		std::cout << "    Enter power (" << constants::MIN_INITIAL_POWER << " - " << constants::MAX_INITIAL_POWER << "):";
		std::cin >> power;
		validation::powerValidation(power);

		std::cout << "    Enter power type (" << (unsigned)SuperHeroPowerType::Air << "  - Air | "
			<< (unsigned)SuperHeroPowerType::Water << " - Water | "
			<< (unsigned)SuperHeroPowerType::Earth << " - Earth | "
			<< (unsigned)SuperHeroPowerType::Fire << " - Fire):";
		std::cin >> type;

		return SuperHero(firstName, lastName, nickname, power, static_cast<SuperHeroPowerType>(type));
	}
	catch (const Regex_Error& err)
	{
		std::cerr << "Regex Error: " << err.what() << std::endl;
		createSuperheroFromConsole();
	}
	catch (const Input_Error& err)
	{
		std::cerr << "Input Error: " << err.what() << std::endl;
		createSuperheroFromConsole();
	}
	catch (const std::length_error& err)
	{
		std::cerr << "Length Error: " << err.what() << std::endl;
		createSuperheroFromConsole();
	}
	catch (const std::invalid_argument& err)
	{
		std::cerr << "Invalid Error: " << err.what() << std::endl;
		createSuperheroFromConsole();
	}
	catch (const std::bad_cast& err)
	{
		std::cerr << "Bad Cast Error: " << err.what() << std::endl;
		createSuperheroFromConsole();
	}
	catch (const std::exception& err)
	{
		std::cerr << "Something went wrong when creating superhero from console! " << std::endl << err.what() << std::endl;
		exit(EXIT_FAILURE);
	}
	catch (...)
	{
		std::cerr << "Something went wrong when creating superhero from console! " << std::endl;
		exit(EXIT_FAILURE);
	}
}
