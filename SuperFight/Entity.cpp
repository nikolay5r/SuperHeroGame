#include "Entity.h"
#include "Validation.h"
#include <iostream>
#include <cstring>

Entity::Entity(const std::string& firstName, const std::string& lastName, const std::string& nickname)
{
	setFirstName(firstName);
	setLastName(lastName);
	setFullName();
	setNickname(nickname);
}

void Entity::setFirstName(const std::string& firstName)
{
	validation::nameValidation(firstName);
	this->firstName = firstName;
}

void Entity::setLastName(const std::string& lastName)
{
	validation::nameValidation(lastName);
	this->lastName = lastName;
}
void Entity::setNickname(const std::string& nickname)
{
	validation::nicknameValidation(nickname);
	this->nickname = nickname;
}

void Entity::setFullName()
{
	fullName = firstName + " " + lastName;
}

const std::string& Entity::getNickname() const noexcept
{
	return nickname;
}

const std::string& Entity::getFullName() const noexcept
{
	return fullName;
}

const std::string& Entity::getFirstName() const noexcept
{
	return firstName;
}

const std::string& Entity::getLastName() const noexcept
{
	return lastName;
}