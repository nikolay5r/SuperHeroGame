#pragma once
#include <fstream>
#include <iostream>
#include "Player.h"
#include "Admin.h"

class UserFactory
{
	static UserFactory* instance;
	UserFactory();

public:
	static UserFactory* getInstance();
	static void freeInstance();

	virtual User* readFromBinary(std::ifstream&) const = 0;
	virtual User* createFromConsole() const = 0;

	virtual ~UserFactory();
};

class PlayerFactory : public UserFactory
{
public:
	User* readFromBinary(std::ifstream&) const;
	User* createFromConsole() const;
};

class AdminFactory : public UserFactory
{
public:
	User* readFromBinary(std::ifstream&) const override;
	User* createFromConsole() const override;
};