#pragma once
#include "MyString.h"

class User
{
protected:
	MyString firstName = "";
	MyString secondName = "";
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
	User(MyString firstName, MyString secondName, MyString username, MyString email, MyString password);

	const MyString& getPassword() const;
	const MyString& getEmail() const;
	const MyString& getUserName() const;
	const MyString& getFullName() const;
	const MyString& getFirstName() const;
	const MyString& getLastName() const;
};

