#include "Validation.h"
#include "HelperFunctions.h"
#include "MyString.h"
#include "Constants.h"

#include <string>
#include <stdexcept>

void validation::isNameValid(const MyString& name)
{
	if (helper::isLower(name[0]))
	{
		throw std::runtime_error("Name doesn't begin with capital letter");
	}

	size_t lastNameLength = name.length();

	for (size_t i = 1; i < lastNameLength; i++)
	{
		if (helper::isUpper(name[i]))
		{
			throw std::runtime_error("There is a capital letter in the middle of the Name!");
		}
	}
}

void validation::isUsernameValid(const MyString& username) //at least 3 characters
{
	static MyString invalidUsernameCharacters = ",.?/<>:;\"'({[]})!@#$%^ &*\\|+=";
	size_t usernameLength = username.length();

	if (usernameLength < constants::MIN_USERNAME_LENGTH)
	{
		//TODO:
		throw std::length_error("Username should be at least 3 characters!");
	}

	for (size_t i = 0; i < usernameLength; i++)
	{
		if (invalidUsernameCharacters.find(username[i]) != std::string::npos)
		{
			throw std::runtime_error("Invalid characters found in Username!");
		}
	}
}

void validation::isEmailValid(const MyString& email) //email@abv.bg
{
	size_t emailLength = email.length();

	int index = email.find('@');

	if (index == std::string::npos)
	{
		throw std::runtime_error("Invalid Email! Missing '@' in your Email!");
	}

	MyString lastPart = email.substr(index + 1);

	if (lastPart.length() == 0)
	{
		throw std::runtime_error("Invalid Email! Missing the address!");
	}

	index = lastPart.find('.');

	lastPart = lastPart.substr(index + 1);

	if (lastPart.length() == 0 || index == std::string::npos)
	{
		throw std::runtime_error("Invalid Email! There is no such address!");
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
		throw std::runtime_error("You should have at least one number in your Password!");
	}

	if (countCapitalLetters == 0)
	{
		throw std::runtime_error("You should have at least one capital letter in your Password!");
	}

	if (countLowerLetters == 0)
	{
		throw std::runtime_error("You should have at least one lower letter in your Password!");
	}
}

void validation::isNicknameValid(const MyString& nickname) // between 3 and 16 characters, capital letters, digits, lower letters and 
{
	static MyString invalidNicknameCharacters = ",.?/<>:;\"'({[]})!@#$%^&*\\|+-_=";
	size_t nicknameLength = nickname.length();

	if (nicknameLength < constants::MIN_NICKNAME_LENGTH)
	{
		//TODO:
		throw std::length_error("Nickname should be at least 3!");
	}

	if (!helper::isUpper(nickname[0]))
	{
		throw std::runtime_error("First character of Nickname must be a capital letter!");
	}

	for (size_t i = 1; i < nicknameLength; i++)
	{
		if (invalidNicknameCharacters.find(nickname[i]) == std::string::npos)
		{
			throw std::runtime_error("Invalid characters found in Nickname!");
		}
	}
}

void validation::isPowerValid(unsigned long long power)
{
	if (power < constants::MIN_POWER || power > constants::MAX_POWER)
	{
		throw std::invalid_argument("Power should be between 5 and 50!");
	}
}
