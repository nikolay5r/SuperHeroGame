#include "File_Error.h"
#include <iostream>

File_Error::File_Error(const char* message) : message(message) {}
File_Error::File_Error(const std::string& message) : message(message) {}

const char* File_Error::what() const noexcept
{
	return message.c_str();
}