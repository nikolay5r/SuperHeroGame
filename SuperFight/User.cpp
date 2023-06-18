#include "User.h"
#include "Player.h"
#include "Admin.h"
#include "Validation.h"
#include "File_Error.h"
#include "Input_Error.h"
#include <fstream>
#include <iostream>

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

User::User(const MyString& firstName, const MyString& lastName, const MyString& nickname, const MyString& email, const MyString& password, UserRole role)
	: Entity(firstName, lastName, nickname), role(role)
{
	setEmail(email);
	setPassword(password);
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

UserFactory* UserFactory::instance = nullptr;

void UserFactory::freeInstance()
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

User* UserFactory::createFromConsoleOnLogin(const MyString& fileName) const
{
	User* user = nullptr;

	std::ifstream file(fileName.c_str(), std::ios::binary);
	if (!file.is_open())
	{
		throw File_Error("File couldn't open!");
	}
	try
	{
		MyString temp;
		std::cout << "Enter nickname: ";
		std::cin >> temp;
		validation::isNicknameValid(temp);

		user = readFromBinary(file, temp);
		std::cout << "Enter password: ";
		std::cin >> temp;
		validation::isPasswordValid(temp);

		if (user->getPassword() != temp)
		{
			throw Input_Error("Password was incorrect");
		}

		file.close();

		return user;
	}
	catch (const File_Error&)
	{
		std::cerr << "File error occured when trying to create a user from console on login!" << std::endl;
		delete user;
		file.close();
		throw;
	}
	catch (...)
	{
		delete user;
		file.close();
		throw;
	}
}

void saveChangesToFile(const User& user)
{
	switch (user.getRole())
	{
	case UserRole::Admin:
	{
		const Admin& admin = static_cast<const Admin&>(user);
		saveChangesToFile(admin);
		break;
	}
	case UserRole::Player:
	{
		const Player& player = static_cast<const Player&>(user);
		saveChangesToFile(player);
		break;
	}
	default:
		break;
	}
}