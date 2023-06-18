#pragma once
#include <iostream>
#include "MyString.h"
#include "User.h"
#include "Entity.h"

enum class UserRole
{
	Admin,
	Player
};

class User : public Entity
{
protected:
	MyString email;
	MyString password;
	UserRole role;

	void setEmail(const MyString& email);
	void setPassword(const MyString& password);
public:
	User(const MyString& firstName, const MyString& lastName, const MyString& nickname, const MyString& email, const MyString& password, UserRole role);

	const MyString& getPassword() const noexcept;
	const MyString& getEmail() const noexcept;
	UserRole getRole() const noexcept;

	virtual ~User() = default;
};

class UserFactory
{
protected:
	static UserFactory* instance;
	UserFactory() = default;
public:
	UserFactory(const UserFactory&) = delete;
	UserFactory& operator=(const UserFactory&) = delete;

	virtual User* readFromBinary() const = 0;
	virtual User* readFromBinary(std::ifstream& file) const = 0;
	virtual User* readFromBinary(const MyString& nickname) const = 0;
	virtual User* readFromBinary(std::ifstream& file, const MyString& nickname) const = 0;
	virtual User* createFromConsole() const = 0;
	User* createFromConsoleOnLogin(const MyString& fileName) const;

	static void freeInstance();
	virtual ~UserFactory() = default;
};

void saveToFile(const User& user);
void removeFromFile(const User& user);
void saveChangesToFile(const User& user);