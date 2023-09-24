#pragma once
#include <iostream>
#include <cstring>
#include "User.h"
#include "Entity.h"
#include <vector>

enum class UserRole
{
	Admin,
	Player
};

class User : public Entity
{
protected:
	std::string email;
	std::string password;
	UserRole role;

	void setEmail(const std::string& email);
	void setPassword(const std::string& password);
public:
	User(const std::string& firstName, const std::string& lastName, const std::string& nickname, const std::string& email, const std::string& password, UserRole role);

	const std::string& getPassword() const noexcept;
	const std::string& getEmail() const noexcept;
	UserRole getRole() const noexcept;

	virtual ~User() = default;
};
