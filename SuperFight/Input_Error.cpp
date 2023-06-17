#include "Input_Error.h"

Input_Error::Input_Error(const char* message) : message(message) {}
Input_Error::Input_Error(const MyString& message) : message(message) {}

const char* Input_Error::what() const noexcept
{
	return message.c_str();
}