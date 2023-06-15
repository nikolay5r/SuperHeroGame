#pragma once
#include "User.h"

class Admin : public User
{

public:
	Admin(const MyString& firstName, const MyString& lastName, const MyString& username, const MyString& email, const MyString& password);
	Admin(const User& user);
	Admin(User&& user);

	void print() const override;

	friend class AdminFactory;
};

class AdminFactory : public UserFactory
{
	AdminFactory() = default;
public:

	static UserFactory* getInstance();

	AdminFactory(const AdminFactory&) = delete;
	AdminFactory& operator=(const AdminFactory&) = delete;

	User* readFromBinary(std::ifstream& file) const override { return nullptr; };
	User* readFromBinary(std::ifstream& file, const MyString& usernameToFind) const override;
	User* createFromConsole() const override;
};
