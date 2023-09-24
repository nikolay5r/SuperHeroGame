#pragma once
#include <iostream>

class Regex_Error
{
	std::string message = "";
public:
	explicit Regex_Error(const char* message);
	explicit Regex_Error(const std::string& message);

	virtual ~Regex_Error() noexcept = default;

	virtual const char* what() const noexcept;
};
// virtual to allow sub-classing

