#pragma once
#include "MyString.h"

class Entity
{
protected:
	MyString firstName;
	MyString lastName;
	MyString nickname;
	MyString fullName;

	void setFirstName(const MyString& firstName);
	void setLastName(const MyString& lastName);
	void setNickname(const MyString& nickname);
	void setFullName();
public:

	Entity(const MyString& firstName, const MyString& lastName, const MyString& nickname);

	const MyString& getNickname() const noexcept;
	const MyString& getFullName() const noexcept;
	const MyString& getFirstName() const noexcept;
	const MyString& getLastName() const noexcept;

	virtual void print() const = 0;
	virtual ~Entity() = default;
};

