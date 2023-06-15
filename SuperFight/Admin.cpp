#include "Admin.h"
#include "User.h"
#include "Validation.h"
#include <utility>
#include <fstream>
#include "File_Error.h"
#include "HelperFunctions.h"
#include "Constants.h"

Admin::Admin(const MyString& firstName, const MyString& lastName, const MyString& username, const MyString& email, const MyString& password)
	: User(firstName, lastName, username, email, password, UserRole::Admin) {}

Admin::Admin(const User& user) : User(user) {}

Admin::Admin(User&& user) : User(std::move(user)) {}

void Admin::print() const
{
	std::cout << username << " | " << fullName << " | " << email;
}

User* AdminFactory::readFromBinary(std::ifstream& file, const MyString& username) const
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
	MyString username;
	MyString email;
	MyString password;

	std::cout << "Enter first name of the admin: ";
	std::cin >> firstName;
	validation::isNameValid(firstName);

	std::cout << "Enter last name of the admin: ";
	std::cin >> lastName;
	validation::isNameValid(lastName);

	std::cout << "Enter username of the admin: ";
	std::cin >> username;
	validation::isUsernameValid(username);

	std::cout << "Enter password for the admin: ";
	std::cin >> password;
	validation::isPasswordValid(password);

	std::cout << "Enter email of the admin: ";
	std::cin >> email;
	validation::isEmailValid(email);

	try
	{
		std::ifstream file(constants::PLAYERS_FILE_PATH.c_str(), std::ios::binary);
		readFromBinary(file, username);
		file.close();
	}
	catch (const std::invalid_argument&)
	{
	return new Admin(firstName, lastName, username, email, password);
}

	throw std::invalid_argument("User with that username already exists!");

}

UserFactory* AdminFactory::getInstance()
{
	if (UserFactory::instance == nullptr)
	{
		UserFactory::instance = new AdminFactory();
	}

	return UserFactory::instance;
}