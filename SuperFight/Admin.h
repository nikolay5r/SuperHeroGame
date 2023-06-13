#pragma once
#include "User.h"
#include "UserFactory.h"

class Admin : public User
{

public:
	Admin(const MyString& firstName, const MyString& lastName, const MyString& username, const MyString& email, const MyString& password);
	Admin(const User& user);
	Admin(User&& user);

	void print() const override;

	friend class AdminFactory;
};

