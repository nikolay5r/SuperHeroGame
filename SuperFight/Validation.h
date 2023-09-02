#pragma once

#include "MyString.h"

namespace validation
{
	void nameValidation(const MyString& name);
	void emailValidation(const MyString& email);
	void passwordValidation(const MyString& password);
	void nicknameValidation(const MyString& nickname);
	void powerValidation(unsigned long long power);
	void powerTypeValidation(unsigned long long powerType);
}
