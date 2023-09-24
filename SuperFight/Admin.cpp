#include <utility>
#include <fstream>
#include <vector>

#include "Admin.h"
#include "User.h"
#include "Validation.h"
#include "Input_Error.h"
#include "Regex_Error.h"
#include "File_Error.h"
#include "HelperFunctions.h"
#include "Constants.h"
#include "SystemConfigurations.h"

Admin::Admin(const std::string& firstName, const std::string& lastName, const std::string& nickname, const std::string& email, const std::string& password)
	: User(firstName, lastName, nickname, email, password, UserRole::Admin) {}

Admin::Admin(const User& user) : User(user) {}

Admin::Admin(User&& user) : User(std::move(user)) {}

void Admin::printShortInfo() const
{
	std::cout << fullName << " | " << nickname << " | " << std::endl;
}

void Admin::printFullInfo() const
{
	std::cout << fullName << " | " << nickname << " | " << email << std::endl;
}

void saveAdminToFile(std::ofstream& file, const Admin& admin)
{
	if (!file.is_open())
	{
		throw File_Error("Couldn't open database when reading an admin!");
	}

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
}

void saveAdminsToFile(const std::string& fileName, const std::vector<Admin>& admins)
{
	std::ofstream file(fileName.c_str(), std::ios::binary | std::ios::trunc);

	try
	{
		if (!file.is_open())
		{
			throw File_Error("Couldn't open database when saving admins!");
		}

		size_t size = admins.size();
		file.write((const char*)&size, sizeof(size));

		for (size_t i = 0; i < size; i++)
		{
			saveAdminToFile(file, admins[i]);
		}
	}
	catch (const File_Error& err)
	{
		std::cerr << err.what() << std::endl;
		exit(EXIT_FAILURE);
	}
	catch (const std::exception& err)
	{
		std::cerr << "Exception was thrown when trying to save multiple admins!\n" << err.what() << std::endl;
		exit(EXIT_FAILURE);
	}
	catch (...)
	{
		std::cerr << "Something went wrong when tryin to save multiple admins!" << std::endl;
		exit(EXIT_FAILURE);
	}

	file.close();
}

std::vector<Admin> readAdminsFromFile(const std::string& fileName)
{
	std::ifstream file(fileName.c_str(), std::ios::binary);

	try
	{
		if (!file.is_open())
		{
			throw File_Error("Couldn't open database when reading admins!");
		}

		size_t size = 0;
		file.read((char*)&size, sizeof(size));

		if (size == 0)
		{
			return std::vector<Admin>();
		}

		std::vector<Admin> admins(size);

		for (size_t i = 0; i < size; i++)
		{
			admins[i] = std::move(readAdminFromFile(file));
		}
		file.close();
		return admins;
	}
	catch (const File_Error& err)
	{
		std::cerr << "File Error: " << err.what() << std::endl;
		exit(EXIT_FAILURE);
	}
	catch (const std::exception& err)
	{
		std::cerr << "Exception was thrown when trying to read multiple admins!\n" << err.what() << std::endl;
		exit(EXIT_FAILURE);
	}
	catch (...)
	{
		std::cerr << "Something went wrong when trying to read multiple admins!" << std::endl;
		exit(EXIT_FAILURE);
	}
}

Admin readAdminFromFile(std::ifstream& file)
{
	if (!file.is_open())
	{
		throw File_Error("Couldn't open database when reading a superhero!");
	}

	size_t size = 0;
	file.read((char*)&size, sizeof(size));
	char* firstName = new char[size + 1];
	file.read(firstName, size + 1);

	file.read((char*)&size, sizeof(size));
	char* lastName = new char[size + 1];
	file.read(lastName, size + 1);

	file.read((char*)&size, sizeof(size));
	char* nickname = new char[size + 1];
	file.read(nickname, size + 1);

	file.read((char*)&size, sizeof(size));
	char* email = new char[size + 1];
	file.read(email, size + 1);

	file.read((char*)&size, sizeof(size));
	char* password = new char[size + 1];
	file.read(password, size + 1);

	Admin admin(firstName, lastName, nickname, email, password);

	delete[] firstName;
	delete[] lastName;
	delete[] nickname;
	delete[] email;
	delete[] password;

	return admin;
}

Admin createAdminFromConsole()
{
	std::string firstName, lastName, nickname, email, password;
	std::cout << "Creating admin: " << std::endl;
	std::cout << "    Enter first name: ";
	std::cin >> firstName;
	std::cout << "    Enter last name: ";
	std::cin >> lastName;
	std::cout << "    Enter nickname: ";
	std::cin >> nickname;
	std::cout << "    Enter email: ";
	std::cin >> email;
	std::cout << "    Enter password: ";
	std::cin >> password;
	return Admin(firstName, lastName, nickname, email, password);
}