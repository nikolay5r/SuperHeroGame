#pragma once
#include <iostream>
#include "MyString.h"

enum class UserRole
{
	Admin,
	Player
};

class User
{
	MyString firstName;
	MyString lastName;
	MyString username;
	MyString email;
	MyString password;
	MyString fullName;
	UserRole role;

protected:
	void setFirstName(const MyString& firstName);
	void setLastName(const MyString& lastName);
	void setEmail(const MyString& email);
	void setPassword(const MyString& password);
	void setUsername(const MyString& username);
	void setFullName();
public:
	User(const MyString& firstName, const MyString& lastName, const MyString& username, const MyString& email, const MyString& password, UserRole role);

	const MyString& getPassword() const noexcept;
	const MyString& getEmail() const noexcept;
	const MyString& getUsername() const noexcept;
	const MyString& getFullName() const noexcept;
	const MyString& getFirstName() const noexcept;
	const MyString& getLastName() const noexcept;
	UserRole getRole() const noexcept;
};

