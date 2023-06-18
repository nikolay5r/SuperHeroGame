#pragma once
#include "User.h"

class Admin : public User
{

public:
	Admin(const MyString& firstName, const MyString& lastName, const MyString& nickname, const MyString& email, const MyString& password);
	Admin(const User& user);
	Admin(User&& user);

	void printShortInfo() const override;
	void printFullInfo() const override;

	friend class AdminFactory;
};

class AdminFactory : public UserFactory
{
	AdminFactory() = default;
public:

	static UserFactory* getInstance();

	AdminFactory(const AdminFactory&) = delete;
	AdminFactory& operator=(const AdminFactory&) = delete;

	User* readFromBinary() const override;
	User* readFromBinary(std::ifstream& file) const override;
	User* readFromBinary(const MyString& nicknameToFind) const override;
	User* readFromBinary(std::ifstream& file, const MyString& nicknameToFind) const override;

	User* createFromConsole() const override;
};

void saveToFile(const Admin& admin);
void removeFromFile(const Admin& admin);
void printAdmins();