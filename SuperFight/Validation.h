#pragma once

#include "MyString.h"

namespace validation
{
	void isNameValid(const MyString& name);
	void isUsernameValid(const MyString& username);
	void isEmailValid(const MyString& email);
	void isPasswordValid(const MyString& password);
	void isNicknameValid(const MyString& nickname);
}
