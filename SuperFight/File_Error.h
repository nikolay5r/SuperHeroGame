#pragma once
#include "MyString.h"
class File_Error : public std::exception
{
	MyString message = "";
public:
	explicit File_Error(const char* message);
	explicit File_Error(const MyString& message);

	virtual ~File_Error() noexcept = default;

	virtual const char* what() const noexcept;
};