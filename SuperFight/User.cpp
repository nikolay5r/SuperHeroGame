#include "User.h"
#include "Player.h"
#include "Admin.h"
#include "Validation.h"
#include "File_Error.h"
#include "Input_Error.h"
#include <fstream>
#include <iostream>

void User::setEmail(const MyString& email)
{
	validation::emailValidation(email);
	this->email = email;
}

void User::setPassword(const MyString& password)
{
	validation::isPasswordValid(password);
	this->password = password;
}

User::User(const MyString& firstName, const MyString& lastName, const MyString& nickname, const MyString& email, const MyString& password, UserRole role)
	: Entity(firstName, lastName, nickname), role(role)
{
	setEmail(email);
	setPassword(password);
}

UserRole User::getRole() const noexcept
{
	return role;
}

const MyString& User::getPassword() const noexcept
{
	return password;
}

const MyString& User::getEmail() const noexcept
{
	return email;
}
