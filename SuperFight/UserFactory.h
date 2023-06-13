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

	Player* createPlayerFromBinary(std::ifstream&) const;
	Admin* createAdminFromBinary(std::ifstream&) const;
	Player* createPlayerFromConsole(std::istream&) const;
	Admin* createAdminFromConsole(std::istream&) const;

	~UserFactory();
};

