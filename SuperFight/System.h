#pragma once
#include "User.h"
#include "MyVector.hpp"
#include "Player.h"
#include "Admin.h"

class System
{
protected:
	static System* instance;
	User* currentUser = nullptr;
	System() = default;

	virtual void logout() = 0;
	virtual void login() = 0;
	virtual void reg() = 0;
	virtual void showMarket() const = 0;
	virtual void showPlayers() const = 0;
	virtual void deleteProfile() = 0;
	virtual void showProfile() = 0;
	void freeSystem();
public:
	System(const System&) = delete;
	System& operator=(const System&) = delete;
	virtual bool run() = 0;

	static void freeInstance();
	virtual ~System();
};
