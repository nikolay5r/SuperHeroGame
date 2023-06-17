#include "Admin.h"
#include "User.h"
#include "Validation.h"
#include <utility>
#include <fstream>
#include "File_Error.h"
#include "HelperFunctions.h"
#include "Constants.h"

Admin::Admin(const MyString& firstName, const MyString& lastName, const MyString& nickname, const MyString& email, const MyString& password)
	: User(firstName, lastName, nickname, email, password, UserRole::Admin) {}

Admin::Admin(const User& user) : User(user) {}

Admin::Admin(User&& user) : User(std::move(user)) {}

void Admin::printShortInfo() const
{
	std::cout << fullName << " | " << nickname << " | ";
}

void Admin::printFullInfo() const
{
	std::cout << nickname << " | " << fullName << " | " << email;
}

User* AdminFactory::readFromBinary(std::ifstream& file, const MyString& nickname) const
{
	if (!file.is_open())
	{
		throw File_Error("File couldn't open!");
	}
}

User* AdminFactory::createFromConsole() const
{
	MyString firstName;
	MyString lastName;
	MyString nickname;
	MyString email;
	MyString password;

	std::cout << "Enter first name of the admin: ";
	std::cin >> firstName;
	validation::isNameValid(firstName);

	std::cout << "Enter last name of the admin: ";
	std::cin >> lastName;
	validation::isNameValid(lastName);

	std::cout << "Enter nickname of the admin: ";
	std::cin >> nickname;
	validation::isNicknameValid(nickname);

	std::cout << "Enter password for the admin: ";
	std::cin >> password;
	validation::isPasswordValid(password);

	std::cout << "Enter email of the admin: ";
	std::cin >> email;
	validation::isEmailValid(email);

	try
	{
		std::ifstream file(constants::PLAYERS_FILE_PATH.c_str(), std::ios::binary);
		readFromBinary(file, nickname);
		file.close();
	}
	catch (const std::invalid_argument&)
	{
		return new Admin(firstName, lastName, nickname, email, password);
	}

	throw std::invalid_argument("User with that nickname already exists!");

}

UserFactory* AdminFactory::getInstance()
{
	if (UserFactory::instance == nullptr)
	{
		UserFactory::instance = new AdminFactory();
	}

	return UserFactory::instance;
}

void saveToFile(const Admin& admin)
{
	return;
}

void removeFromFile(const Admin& admin)
{

}