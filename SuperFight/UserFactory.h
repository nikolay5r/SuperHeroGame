#pragma once
#include <fstream>
#include <iostream>
#include "Player.h"
#include "Admin.h"

class UserFactory
{
public:
	virtual User* readFromBinary(std::ifstream&) const = 0;
	virtual User* createFromConsole() const = 0;

	virtual ~UserFactory() = default;
};

class PlayerFactory : public UserFactory
{
public:
	PlayerFactory() = default;

	PlayerFactory(const PlayerFactory&) = delete;
	PlayerFactory& operator=(const PlayerFactory&) = delete;

	User* readFromBinary(std::ifstream&) const override;
	User* createFromConsole() const override;
};

class AdminFactory : public UserFactory
{
public:
	AdminFactory() = default;

	AdminFactory(const AdminFactory&) = delete;
	AdminFactory& operator=(const AdminFactory&) = delete;

	User* readFromBinary(std::ifstream&) const override;
	User* createFromConsole() const override;
};