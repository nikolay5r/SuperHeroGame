#include <iostream>
#include <fstream>
#include "MyString.h"
#include "User.h"
#include "Player.h"
#include "Regex_Error.h"
int main()
{
	try
	{
		Player p1("Aaaaa", "Asdasdasd", "adsadas", "asd@abv.bg", "12NaN3");
		Player p2("Aaa", "Asdasd", "adssaadasaaa", "asdasdd@abv.bg", "1223Naaaa");
		std::cout << sizeof(p1) << " " << sizeof(p2) << std::endl;
	}
	catch (const Regex_Error& error)
	{
		std::cout << error.what();
	}
	catch (const std::exception& error)
	{
		std::cout << error.what();
	}
}
