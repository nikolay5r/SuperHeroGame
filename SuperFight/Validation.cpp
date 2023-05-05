#include "Validation.h"
#include "HelperFunctions.h"
#include "MyString.h"
#include "Regex_Error.h"

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

void validation::isUserNameValid(const MyString& userName)
{
	static MyString invalidCharacters = ",.?/<>:;\"'({[]})!@#$%^&*\\|+";
	size_t userNameLength = userName.length();

	if (userNameLength > 16)
		throw std::length_error("UserName is too long!");

	for (size_t i = 0; i < userNameLength; i++)
		if (invalidCharacters.find(userName[i]) != std::string::npos)
			throw Regex_Error("Invalid characters found in userName! Characters cannot be " + invalidCharacters + "!");
}

void validation::isEmailValid(const MyString& email) //email@abv.bg
{
	size_t emailLength = email.length();

	MyString lastPart = email.substr(email.find('@') + 1);

	if (lastPart.length() < 1)
		throw Regex_Error("Invalid Email!");

	lastPart = lastPart.substr(lastPart.find('.') + 1);

	if (lastPart.length() < 1)
		throw Regex_Error("Invalid Email!");
}

void validation::isPasswordValid(const MyString& password) //at least one capital, one lower and one number
{

}