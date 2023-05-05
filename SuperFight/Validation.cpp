#include "Validation.h"
#include "HelperFunctions.h"
#include "MyString.h"
#include "Regex_Error.h"

void validation::isNameValid(const MyString& name)
{
	if (!helper::isUpper(name[0]))
		throw Regex_Error("Name doesn't begin with capital letter");

	size_t lastNameLength = name.length();

	for (size_t i = 1; i < lastNameLength; i++)
		if (isUpper(name[i]))
			throw Regex_Error("There is a capital letter in the middle of the name!");
}