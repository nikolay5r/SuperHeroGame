#include "Entity.h"
#include "Validation.h"

Entity::Entity(const MyString& firstName, const MyString& lastName, const MyString& nickname)
{
	setFirstName(firstName);
	setLastName(lastName);
	setFullName();
	setNickname(nickname);
}

void Entity::setFirstName(const MyString& firstName)
{
	validation::isNameValid(firstName);
	this->firstName = firstName;
}

void Entity::setLastName(const MyString& lastName)
{
	validation::isNameValid(lastName);
	this->lastName = lastName;
}
void Entity::setNickname(const MyString& nickname)
{
	validation::isNicknameValid(nickname);
	this->nickname = nickname;
}

void Entity::setFullName()
{
	fullName = firstName + " " + lastName;
}

const MyString& Entity::getNickname() const noexcept
{
	return nickname;
}

const MyString& Entity::getFullName() const noexcept
{
	return fullName;
}

const MyString& Entity::getFirstName() const noexcept
{
	return firstName;
}

const MyString& Entity::getLastName() const noexcept
{
	return lastName;
}