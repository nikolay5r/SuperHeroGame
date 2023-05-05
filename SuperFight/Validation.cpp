#include "Validation.h"
#include "HelperFunctions.h"
#include "MyString.h"
#include "Regex_Error.h"
#include "Constants.h"

#include <string>
#include <stdexcept>

void validation::isNameValid(const MyString& name)
{
	if (helper::isLower(name[0]))
		throw Regex_Error("Name doesn't begin with capital letter");

	size_t lastNameLength = name.length();

	for (size_t i = 1; i < lastNameLength; i++)
		if (helper::isUpper(name[i]))
			throw Regex_Error("There is a capital letter in the middle of the name!");
}

void validation::isUserNameValid(const MyString& userName) //at least 5 characters, max - 20
{
	static MyString invalidCharacters = ",.?/<>:;\"'({[]})!@#$%^ &*\\|+";
	size_t userNameLength = userName.length();

	if (userNameLength > constants::MAX_USERNAME_LENGTH)
		throw std::length_error("UserName is too long!");

	if (userNameLength < constants::MIN_USERNAME_LENGTH)
		throw std::length_error("UserName is too short!");

	for (size_t i = 0; i < userNameLength; i++)
		if (invalidCharacters.find(userName[i]) != std::string::npos)
			throw Regex_Error("Invalid characters found in userName! Characters cannot be " + invalidCharacters + " !");
}

void validation::isEmailValid(const MyString& email) //email@abv.bg
{
	size_t emailLength = email.length();

	MyString lastPart = email.substr(email.find('@') + 1);

	if (lastPart.length() == 0)
		throw Regex_Error("Invalid Email!");

	lastPart = lastPart.substr(lastPart.find('.') + 1);

	if (lastPart.length() == 0)
		throw Regex_Error("Invalid Email!");
}

void validation::isPasswordValid(const MyString& password) //at least one capital, one lower and one number, at least 5 characters and maximum of 16
{
	int countNumbers = 0;
	int countCapitalLetters = 0;
	int countLowerLetters = 0;

	size_t passwordLength = password.length();

	if (passwordLength > constants::MAX_PASSWORD_LENGTH)
		throw std::length_error("Password is too long!");

	if (passwordLength < constants::MIN_PASSWORD_LENGTH)
		throw std::length_error("Password is too short!");

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
		throw Regex_Error("You should have at least one number in your Password!");

	if (countCapitalLetters == 0)
		throw Regex_Error("You should have at least one capital letter in your Password!");

	if (countLowerLetters == 0)
		throw Regex_Error("You should have at least one lower letter in your Password!");
}