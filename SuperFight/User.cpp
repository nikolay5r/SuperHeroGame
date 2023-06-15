#include "User.h"
#include "Player.h"
#include "Admin.h"
#include "Validation.h"

void User::setFirstName(const MyString& firstName)
{
	validation::isNameValid(firstName);
	this->firstName = firstName;
}

void User::setLastName(const MyString& lastName)
{
	validation::isNameValid(lastName);
	this->lastName = lastName;
}

void User::setEmail(const MyString& email)
{
	validation::isEmailValid(email);
	this->email = email;
}

void User::setPassword(const MyString& password)
{
	validation::isPasswordValid(password);
	this->password = password;
}

void User::setUsername(const MyString& username)
{
	validation::isUsernameValid(username);
	this->username = username;
}

void User::setFullName()
{
	fullName = firstName + " " + lastName;
}

User::User(const MyString& firstName, const MyString& lastName, const MyString& username, const MyString& email, const MyString& password, UserRole role)
	: role(role)
{
	setFirstName(firstName);
	setLastName(lastName);
	setFullName();
	setEmail(email);
	setPassword(password);
	setUsername(username);
}

UserRole User::getRole() const noexcept
{
	return role;
}

const MyString& User::getPassword() const noexcept
{
	return password;
}

const MyString& User::getEmail() const noexcept
{
	return email;
}

const MyString& User::getUsername() const noexcept
{
	return username;
}

const MyString& User::getFullName() const noexcept
{
	return fullName;
}

const MyString& User::getFirstName() const noexcept
{
	return firstName;
}

const MyString& User::getLastName() const noexcept
{
	return lastName;
}

UserFactory* UserFactory::instance = nullptr;

void UserFactory::freeInstance()
{
	delete UserFactory::instance;
	UserFactory::instance = nullptr;
}

UserFactory::~UserFactory()
{
	delete UserFactory::instance;
	UserFactory::instance = nullptr;
}

void saveToFile(const User& user)
{
	switch (user.getRole())
	{
	case UserRole::Admin:
	{
		const Admin& admin = static_cast<const Admin&>(user);
		saveToFile(admin);
		break;
	}
	case UserRole::Player:
	{
		const Player& player = static_cast<const Player&>(user);
		saveToFile(player);
		break;
	}
	default:
		break;
	}
}

void removeFromFile(const User& user)
{
	switch (user.getRole())
	{
	case UserRole::Admin:
	{
		const Admin& admin = static_cast<const Admin&>(user);
		removeFromFile(admin);
		break;
	}
	case UserRole::Player:
	{
		const Player& player = static_cast<const Player&>(user);
		removeFromFile(player);
		break;
	}
	default:
		break;
	}
}