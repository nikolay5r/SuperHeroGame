#pragma once
#include "MyString.h"

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

class SuperHero
{
	MyString firstName;
	MyString lastName;
	MyString fullName;
	MyString nickname;
	unsigned power;
	SuperHeroPowerType powerType;
	SuperHeroPosition position = SuperHeroPosition::Attack;
	unsigned price;
	uint8_t level = 1;
	uint8_t xp = 0;
	uint8_t powerLevel = 0;
	uint8_t allowedPowerUpgrades = 1;

	static const unsigned xpNeededPerLevel[10];

	void setFirstName(const MyString& firstName);
	void setLastName(const MyString& lastName);
	void setFullName() noexcept;
	void setNickname(const MyString& nickname);
	void setPrice() noexcept;
	void setPower(unsigned long long power);

public:

	SuperHero(const MyString& firstName, const MyString& lastName, const MyString& nickname, unsigned power, SuperHeroPowerType powerType);
	unsigned getPower() const noexcept;
	unsigned getPrice() const noexcept;
	SuperHeroPowerType getPowerType() const noexcept;
	const MyString& getFirstName() const noexcept;
	const MyString& getLastName() const noexcept;
	const MyString& getFullName() const noexcept;
	const MyString& getNickname() const noexcept;
	SuperHeroPosition getPosition() const noexcept;
	uint8_t getPowerLevel() const noexcept;
	uint8_t getLevel() const noexcept;

	void changePosition() noexcept;
	void levelUp();
	int fight(const SuperHero& other) const noexcept;
	void powerUp();
	void gainXP() noexcept;
};