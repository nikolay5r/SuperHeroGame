#pragma once
#include "MyString.h"
class Input_Error
{
	MyString message = "";
public:
	explicit Input_Error(const char* message);
	explicit Input_Error(const MyString& message);

	virtual ~Input_Error() noexcept = default;

	virtual const char* what() const noexcept;
};

