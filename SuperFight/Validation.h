#pragma once
#include <iostream>

namespace validation
{
	void nameValidation(const std::string& name);
	void emailValidation(const std::string& email);
	void passwordValidation(const std::string& password);
	void nicknameValidation(const std::string& nickname);
	void powerValidation(unsigned long long power);
	void powerTypeValidation(unsigned long long powerType);
}
