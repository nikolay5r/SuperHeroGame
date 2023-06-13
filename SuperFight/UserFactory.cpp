#include "UserFactory.h"
#include "Validation.h"

UserFactory* UserFactory::getInstance()
{
	UserFactory::instance = new UserFactory();
	return UserFactory::instance;
}

void UserFactory::freeInstance()
{
	delete UserFactory::instance;
}

User* PlayerFactory::readFromBinary(std::ifstream& ifs) const
{
	if (ifs.is_open())
	{
		//TODO: File error;
		return nullptr;
	}
}

User* AdminFactory::readFromBinary(std::ifstream& ifs) const
{
	if (ifs.is_open())
	{
		//TODO: File error;
		return nullptr;
	}
}

User* PlayerFactory::createFromConsole() const
{
	MyString firstName;
	MyString lastName;
	MyString username;
	MyString email;
	MyString password;

	std::cout << "Enter first name of the player: ";
	std::cin >> firstName;
	validation::isNameValid(firstName);

	std::cout << "Enter last name of the player: ";
	std::cin >> lastName;
	validation::isNameValid(lastName);

	std::cout << "Enter username of the player: ";
	std::cin >> username;
	validation::isUsernameValid(username);

	std::cout << "Enter password for the player: ";
	std::cin >> password;
	validation::isPasswordValid(password);

	std::cout << "Enter email of the player: ";
	std::cin >> email;
	validation::isEmailValid(email);

	return new Player(firstName, lastName, username, email, password);
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

	return new Admin(firstName, lastName, username, email, password);
}

UserFactory::~UserFactory()
{
	delete UserFactory::instance;
}