#include <iostream>
#include "MyString.h"

int main()
{
	MyString str("abcdeft");

	std::cout << str.find("cde", 3);
}
