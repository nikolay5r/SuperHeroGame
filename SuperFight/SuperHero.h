#pragma once
#include "MyString.h"
#include "Entity.h"

enum class SuperHeroPowerType
{
	Air,
	Water,
	Earth,
	Fire
};

enum class SuperHeroPosition
{
	Attack,
	Defense
};

class SuperHero : public Entity
{
	unsigned power = 0;
	SuperHeroPowerType powerType = SuperHeroPowerType::Air;
	SuperHeroPosition position = SuperHeroPosition::Attack;
	unsigned price = 0;
	uint8_t level = 1;
	uint8_t xp = 0;
	uint8_t powerLevel = 0;
	uint8_t allowedPowerUpgrades = 1;
	mutable bool hasAttacked = false;

	static const unsigned xpNeededPerLevel[10];

	void setPrice() noexcept;
	void setPower(unsigned long long power);

public:
	SuperHero(const MyString& firstName, const MyString& lastName, const MyString& nickname, unsigned power, SuperHeroPowerType powerType);
	unsigned getPower() const noexcept;
	unsigned getPrice() const noexcept;
	SuperHeroPowerType getPowerType() const noexcept;
	SuperHeroPosition getPosition() const noexcept;
	uint8_t getPowerLevel() const noexcept;
	uint8_t getLevel() const noexcept;
	uint8_t getXP() const noexcept;
	uint8_t getAllowedPowerUpgrades() const noexcept;
	bool getAttackInfo() const noexcept;

	void changePosition() noexcept;
	void levelUp();
	int fight(const SuperHero& other) const noexcept;
	void powerUp();
	void gainXP() noexcept;

	void printShortInfo() const;
	void printFullInfo() const;

	friend class SuperHeroFactory;
};

class SuperHeroFactory
{
	static SuperHeroFactory* instance;
	SuperHeroFactory() = default;
public:
	static SuperHeroFactory* getInstance();

	SuperHeroFactory(const SuperHeroFactory&) = delete;
	SuperHeroFactory& operator=(const SuperHeroFactory&) = delete;

	SuperHero* readFromBinary(std::ifstream&) const;
	SuperHero* readFromBinary(const MyString& fileName, const MyString& nickname) const;
	SuperHero* readFromBinary(std::ifstream&, const MyString& nickname) const;
	SuperHero* createFromConsole() const;

	static void freeInstance();
	virtual ~SuperHeroFactory() = default;
};

void saveToFile(std::ofstream& file, const SuperHero& superhero);
void saveToFile(const MyString& fileName, const SuperHero& superhero);
void removeFromFile(const MyString& fileName, const SuperHero& superhero);
SuperHero* buy(const MyString& nickname);
void sell(const SuperHero& superheroToSell);
unsigned printSuperheroesAndGetCountOfPrinted(const MyString& fileName);