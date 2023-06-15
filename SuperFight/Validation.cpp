#include "Validation.h"
#include "HelperFunctions.h"
#include "MyString.h"
#include "Constants.h"
#include "Regex_Error.h"

#include <string>
#include <stdexcept>

void validation::isNameValid(const MyString& name)
{
	if (helper::isLower(name[0]))
	{
		throw Regex_Error("Name doesn't begin with capital letter");
	}

	size_t lastNameLength = name.length();

	for (size_t i = 1; i < lastNameLength; i++)
	{
		if (helper::isUpper(name[i]))
		{
			throw Regex_Error("There is a capital letter in the middle of the Name!");
		}
	}
}

void validation::isNicknameValid(const MyString& nickname) //at least 3 characters
{
	size_t nicknameLength = nickname.length();

	if (nicknameLength > constants::MAX_USERNAME_LENGTH)
	{
		throw std::length_error("Nickname should be no longer than 16 characters!");
	}

	for (size_t i = 0; i < nicknameLength; i++)
	{
		if (nickname[i] < 'a' || nickname[i] > 'z')
		{
			throw Regex_Error("Invalid characters found in Nickname!");
		}
	}
}

void validation::isEmailValid(const MyString& email) //email@abv.bg
{
	size_t emailLength = email.length();

	size_t index = email.find('@');

	if (index == std::string::npos)
	{
		throw Regex_Error("Invalid Email! Missing '@' in your Email!");
	}

	MyString lastPart = email.substr(index + 1);

	if (lastPart.length() == 0)
	{
		throw Regex_Error("Invalid Email! Missing the address!");
	}

	index = lastPart.find('.');

	lastPart = lastPart.substr(index + 1);

	if (lastPart.length() == 0 || index == std::string::npos)
	{
		throw Regex_Error("Invalid Email! There is no such address!");
	}
}

void validation::isPasswordValid(const MyString& password) //at least one capital, one lower and one number, at least 5 characters and maximum of 16
{
	int countNumbers = 0;
	int countCapitalLetters = 0;
	int countLowerLetters = 0;

	size_t passwordLength = password.length();

	if (passwordLength > constants::MAX_PASSWORD_LENGTH || passwordLength < constants::MIN_PASSWORD_LENGTH)
	{
		//TODO:
		throw std::length_error("Password should be between 5 and 16 characters!");
	}

	for (size_t i = 0; i < passwordLength; i++)
	{
		if (helper::isDigit(password[i]))
			countNumbers++;
		else if (helper::isUpper(password[i]))
			countCapitalLetters++;
		else if (helper::isLower(password[i]))
			countLowerLetters++;
	}

	if (countNumbers == 0)
	{
		throw Regex_Error("You should have at least one number in your Password!");
	}

	if (countCapitalLetters == 0)
	{
		throw Regex_Error("You should have at least one capital letter in your Password!");
	}

	if (countLowerLetters == 0)
	{
		throw Regex_Error("You should have at least one lower letter in your Password!");
	}
}

void validation::isPowerValid(unsigned long long power)
{
	if (power < constants::MIN_POWER)
	{
		throw std::invalid_argument("Invalid power! The power is too little!");
	}
}
