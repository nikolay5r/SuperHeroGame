#include "File_Error.h"

File_Error::File_Error(const char* message) : message(message) {}
File_Error::File_Error(const MyString& message) : message(message) {}

const char* File_Error::what() const noexcept
{
	return message.c_str();
}