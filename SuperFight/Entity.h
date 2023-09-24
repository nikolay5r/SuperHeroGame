#pragma once
#include <iostream>
#include <cstring>

class Entity
{
protected:
	std::string firstName;
	std::string lastName;
	std::string nickname;
	std::string fullName;

	void setFirstName(const std::string& firstName);
	void setLastName(const std::string& lastName);
	void setNickname(const std::string& nickname);
	void setFullName();
public:

	Entity(const std::string& firstName, const std::string& lastName, const std::string& nickname);

	const std::string& getNickname() const noexcept;
	const std::string& getFullName() const noexcept;
	const std::string& getFirstName() const noexcept;
	const std::string& getLastName() const noexcept;

	virtual void printShortInfo() const = 0;
	virtual void printFullInfo() const = 0;
	virtual ~Entity() = default;
};

