#include <iostream>
#include "MyString.h"
#include "User.h"
#include "Regex_Error.h"
int main()
{
	try
	{
	}
	catch (const std::exception& error)
	{
		std::cout << error.what() << std::endl;
	}

	MyString str("abcdeft");

	std::cout << str.find("cde", 3);
}
