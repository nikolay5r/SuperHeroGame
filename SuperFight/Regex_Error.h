#pragma once
#include <stdexcept>
#include "MyString.h"
class Regex_Error
{
	MyString message = "";
public:
	explicit Regex_Error(const char* message);
	explicit Regex_Error(const MyString message);

	virtual ~Regex_Error() noexcept;

	virtual const char* what() const noexcept;
};
// virtual to allow sub-classing

