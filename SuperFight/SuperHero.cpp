#include "SuperHero.h"
#include "MyString.h"
#include "Validation.h"

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

void SuperHero::setPrice(unsigned long long price)
{

}

void SuperHero::setPower(unsigned long long power)
{

}

SuperHero::SuperHero(const MyString& firstName, const MyString& lastName, const MyString& nickname, unsigned power, SuperHeroPowerType powerType, unsigned price)
{
	setFirstName(firstName);
	setLastName(lastName);
	setFullName();
	setNickname(nickname);
	this->powerType = powerType;
	setPower(power);
	setPrice(price);
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