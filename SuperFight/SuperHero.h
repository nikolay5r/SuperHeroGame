#pragma once
#include "MyString.h"

enum class SuperHeroPowerType
{
	Fire,
	Water,
	Earth,
	Air
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
	/*unsigned xp;
	unsigned xpNeeded = 5;*/

	void setFirstName(const MyString& firstName);
	void setLastName(const MyString& lastName);
	void setFullName();
	void setNickname(const MyString& nickname);
	void setPrice(unsigned long long price);
	void setPower(unsigned long long power);

public:

	SuperHero(const MyString& firstName, const MyString& lastName, const MyString& nickname, unsigned power, SuperHeroPowerType powerType, unsigned price);
	unsigned getPower() const;
	unsigned getPrice() const;
	SuperHeroPowerType getPowerType() const;
	const MyString& getFirstName() const;
	const MyString& getLastName() const;
	const MyString& getFullName() const;
	const MyString& getNickname() const;

	//void levelUp();
};