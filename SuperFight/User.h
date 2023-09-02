#pragma once
#include <iostream>
#include "MyString.h"
#include "User.h"
#include "Entity.h"
#include "MyVector.hpp"

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
