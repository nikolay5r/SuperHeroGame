#pragma once
#include <cstring>
#include "Entity.h"
#include <vector>

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

	explicit SuperHero();

public:
	SuperHero(const std::string& firstName, const std::string& lastName, const std::string& nickname, unsigned power, SuperHeroPowerType powerType);
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

	friend SuperHero readSuperheroFromFile(std::ifstream& file);
	friend std::vector<SuperHero> readSuperheroesFromFile(const std::string& fileName);
};

void saveSuperheroToFile(std::ofstream& file, const SuperHero& superhero);
void saveSuperheroesToFile(const std::string& fileName, const std::vector<SuperHero>& superhero);
SuperHero createSuperheroFromConsole();
SuperHero readSuperheroFromFile(std::ifstream& file);
std::vector<SuperHero> readSuperheroesFromFile(const std::string& fileName);
