#include "User.h"
#include "Player.h"
#include "Admin.h"
#include "Validation.h"
#include "File_Error.h"
#include "Input_Error.h"
#include <fstream>
#include <iostream>

void User::setEmail(const std::string& email)
{
	validation::emailValidation(email);
	this->email = email;
}

void User::setPassword(const std::string& password)
{
	validation::passwordValidation(password);
	this->password = password;
}

User::User(const std::string& firstName, const std::string& lastName, const std::string& nickname, const std::string& email, const std::string& password, UserRole role)
	: Entity(firstName, lastName, nickname), role(role)
{
	setEmail(email);
	setPassword(password);
}

UserRole User::getRole() const noexcept
{
	return role;
}

const std::string& User::getPassword() const noexcept
{
	return password;
}

const std::string& User::getEmail() const noexcept
{
	return email;
}
