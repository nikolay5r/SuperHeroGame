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
	MyString nickName;
	unsigned power;
	SuperHeroPowerType powerType;
	unsigned price;
	unsigned xp;
	unsigned xpNeeded = 5;

	void setFirstName(const MyString& firstName);
	void setLastName(const MyString& secondName);
	void setFullName();
	void setNickName(const MyString& nickName);
	void setPrice(unsigned long long price);
	void setPower(unsigned long long power);
	void setPowerType(SuperHeroPowerType powerType);
public:
	SuperHero(const MyString& firstName, const MyString& lastName, const MyString& nickName, unsigned power, SuperHeroPowerType powerType, unsigned price);
	unsigned getPower() const;
	unsigned getPrice() const;
	SuperHeroPowerType getPowerType() const;
	const MyString& getFirstName() const;
	const MyString& getLastName() const;
	const MyString& getFullName() const;
	const MyString& getNickName() const;

	void levelUp();
};