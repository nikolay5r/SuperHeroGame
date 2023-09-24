#pragma once
#include <iostream>

class Input_Error
{
	std::string message = "";
public:
	explicit Input_Error(const char* message);
	explicit Input_Error(const std::string& message);

	virtual ~Input_Error() noexcept = default;

	virtual const char* what() const noexcept;
};

