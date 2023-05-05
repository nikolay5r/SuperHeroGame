#include <iostream>
#include "MyString.h"
#include "User.h"
#include "Regex_Error.h"
int main()
{
	try {
		User user("Franko", "Montoya", "!kdlsakdlaskQ0!", "email@abv.bg", "okokokA1", UserRole::Admin);
	}
	catch (const Regex_Error& error)
	{
		std::cout << error.what() << std::endl;
	}
	catch (const std::exception& error)
	{
		std::cout << error.what() << std::endl;
	}
	MyString str("abcdeft");


	std::cout << str.find("cde", 3);
}
