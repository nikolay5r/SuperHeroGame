#include "Admin.h"
#include "User.h"
#include "Validation.h"
#include <utility>
#include <fstream>
#include "File_Error.h"
#include "HelperFunctions.h"
#include "Constants.h"
#include "SystemConfigurations.h"

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
	std::cout << fullName << " | " << nickname << " | " << email;
}

User* AdminFactory::readFromBinary() const
{
	std::ifstream file(constants::ADMINS_FILE_PATH.c_str(), std::ios::binary);

	User* admin = readFromBinary(file);
	file.close();

	return admin;
}

User* AdminFactory::readFromBinary(std::ifstream& file) const
{
	if (!file.is_open())
	{
		throw File_Error("File couldn't open!");
	}

	size_t n;
	file.read((char*)&n, sizeof(n));
	char* firstName = new char[n + 1];
	file.read(firstName, n + 1);

	file.read((char*)&n, sizeof(n));
	char* lastName = new char[n + 1];
	file.read(lastName, n + 1);

	file.read((char*)&n, sizeof(n));
	char* nickname = new char[n + 1];
	file.read(nickname, n + 1);

	file.read((char*)&n, sizeof(n));
	char* email = new char[n + 1];
	file.read(email, n + 1);

	file.read((char*)&n, sizeof(n));
	char* password = new char[n + 1];
	file.read(password, n + 1);

	Admin* admin = new Admin(firstName, lastName, nickname, email, password);

	delete[] firstName;
	delete[] lastName;
	delete[] nickname;
	delete[] email;
	delete[] password;

	return admin;
}

User* AdminFactory::readFromBinary(const MyString& nickname) const
{
	std::ifstream file(constants::ADMINS_FILE_PATH.c_str(), std::ios::binary);

	if (!file.is_open())
	{
		throw File_Error("File couldn't open!");
	}

	User* user = readFromBinary(file, nickname);

	file.close();
	return user;
}

User* AdminFactory::readFromBinary(std::ifstream& file, const MyString& nickname) const
{
	User* curr = nullptr;

	if (!file.is_open())
	{
		throw File_Error("File couldn't open!");
	}

	while (helper::isEOF(file))
	{
		curr = readFromBinary(file);
		if (curr->getNickname() == nickname)
		{
			return curr;
		}
		delete curr;
		if (file.eof())
		{
			break;
		}
	}

	throw std::invalid_argument("Nickname is not valid!");
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
		std::ifstream file(constants::ADMINS_FILE_PATH.c_str(), std::ios::binary);
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


void saveAdminToFile(const Admin& admin)
{
	std::ofstream file(constants::ADMINS_FILE_PATH.c_str(), std::ios::binary | std::ios::app);

	if (!file.is_open())
	{
		throw File_Error("File couldn't open!");
	}

	int index = file.tellp();

	size_t size = admin.getFirstName().length();
	file.write((const char*)&size, sizeof(size));
	file.write(admin.getFirstName().c_str(), size + 1);

	size = admin.getLastName().length();
	file.write((const char*)&size, sizeof(size));
	file.write(admin.getLastName().c_str(), size + 1);

	size = admin.getNickname().length();
	file.write((const char*)&size, sizeof(size));
	file.write(admin.getNickname().c_str(), size + 1);

	size = admin.getEmail().length();
	file.write((const char*)&size, sizeof(size));
	file.write(admin.getEmail().c_str(), size + 1);

	size = admin.getPassword().length();
	file.write((const char*)&size, sizeof(size));
	file.write(admin.getPassword().c_str(), size + 1);

	file.close();
}

void removeAdminFromFile(const Admin& admin)
{
	int indexStart = -1;
	int indexEnd = -1;

	helper::getStartIndexAndEndIndexOfEntityInFile(constants::ADMINS_FILE_PATH, indexStart, indexEnd, admin);
	helper::deleteDataFromFile(constants::ADMINS_FILE_PATH, indexStart, indexEnd);
	configs::decrementCountOfAdmins();
}

void printAdmins()
{
	std::ifstream file(constants::ADMINS_FILE_PATH.c_str(), std::ios::in | std::ios::binary);;

	if (!file.is_open())
	{
		throw File_Error("File couldn't open!");
	}

	unsigned countOfPrintedAdmins = 0;
	for (int i = 1; !helper::isEOF(file); i++)
	{
		std::cout << i << ". ";
		UserFactory* factory = AdminFactory::getInstance();
		User* admin = factory->readFromBinary(file);
		admin->printShortInfo();
		delete admin;
	}
	std::cout << std::endl;
	file.close();
}