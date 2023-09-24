#include "Input_Error.h"
#include <iostream>

Input_Error::Input_Error(const char* message) : message(message) {}
Input_Error::Input_Error(const std::string& message) : message(message) {}

const char* Input_Error::what() const noexcept
{
	return message.c_str();
}