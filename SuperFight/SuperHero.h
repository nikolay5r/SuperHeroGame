#pragma once
#include "MyString.h"

enum class SuperHeroPowerType
{
	Air,
	Water,
	Earth,
	Fire
};

class SuperHero
{
	MyString firstName;
	MyString lastName;
	MyString fullName;
	MyString nickname;
	unsigned power;
	SuperHeroPowerType powerType;
	unsigned price;
	uint8_t level = 1;
	unsigned xp = 0;
	static const unsigned xpNeededPerLevel[10];

	void setFirstName(const MyString& firstName);
	void setLastName(const MyString& lastName);
	void setFullName();
	void setNickname(const MyString& nickname);
	void setPrice();
	void setPower(unsigned long long power);

public:

	SuperHero(const MyString& firstName, const MyString& lastName, const MyString& nickname, unsigned power, SuperHeroPowerType powerType);
	unsigned getPower() const;
	unsigned getPrice() const;
	SuperHeroPowerType getPowerType() const;
	const MyString& getFirstName() const;
	const MyString& getLastName() const;
	const MyString& getFullName() const;
	const MyString& getNickname() const;

	void levelUp();
	int fight(const SuperHero& other);
	void powerUp();
};