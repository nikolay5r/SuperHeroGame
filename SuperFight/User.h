#pragma once
#include "MyString.h"

class User
{
protected:
	MyString firstName = "";
	MyString lastName = "";
	MyString userName = "";
	MyString email = "";
	MyString password = "";

	MyString fullName = "";

	void setFirstName(MyString firstName);
	void setLastName(MyString fullName);
	void setEmail(MyString email);
	void setPassword(MyString password);
	void setUsername(MyString username);
	void setFullName();
public:
	User(MyString firstName, MyString lastName, MyString username, MyString email, MyString password);

	const MyString& getPassword() const noexcept;
	const MyString& getEmail() const noexcept;
	const MyString& getUserName() const noexcept;
	const MyString& getFullName() const noexcept;
	const MyString& getFirstName() const noexcept;
	const MyString& getLastName() const noexcept;
};

