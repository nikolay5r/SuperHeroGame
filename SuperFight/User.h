#pragma once
#include "MyString.h"

enum class UserRole
{
	Admin,
	Player
};

class User
{
protected:
	MyString firstName = "";
	MyString lastName = "";
	MyString userName = "";
	MyString email = "";
	MyString password = "";
	MyString fullName = "";
	UserRole role = UserRole::Player;

	void setFirstName(const MyString& firstName);
	void setLastName(const MyString& lastName);
	void setEmail(const MyString& email);
	void setPassword(const MyString& password);
	void setUserName(const MyString& username);
	void setFullName();
public:
	User(const MyString& firstName, const MyString& lastName, const MyString& username, const MyString& email, const MyString& password, UserRole role);

	const MyString& getPassword() const noexcept;
	const MyString& getEmail() const noexcept;
	const MyString& getUserName() const noexcept;
	const MyString& getFullName() const noexcept;
	const MyString& getFirstName() const noexcept;
	const MyString& getLastName() const noexcept;
	UserRole getRole() const noexcept;
};

