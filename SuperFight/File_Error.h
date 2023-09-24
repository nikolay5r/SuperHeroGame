#pragma once
#include <iostream>

class File_Error
{
	std::string message = "";
public:
	explicit File_Error(const char* message);
	explicit File_Error(const std::string& message);

	virtual ~File_Error() noexcept = default;

	virtual const char* what() const noexcept;
};