#include <iostream>
#include "MyString.h"
#include "User.h"
#include "Regex_Error.h"
int main()
{
	try
	{
		User user("ranko", "Montoya", "kdlsakdlaskQ0", "email@abv.bg", "okokok1", UserRole::Admin);
	}
	catch (const std::exception& error)
	{
		std::cout << error.what() << std::endl;
	}

	MyString str("abcdeft");

	std::cout << str.find("cde", 3);
}
