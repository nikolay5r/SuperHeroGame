#include "Regex_Error.h"

Regex_Error::Regex_Error(const char* message) : message(message) {}
Regex_Error::Regex_Error(const std::string& message) : message(message) {}

const char* Regex_Error::what() const noexcept
{
	return message.c_str();
}