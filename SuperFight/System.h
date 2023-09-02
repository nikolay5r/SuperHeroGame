#pragma once
#include "User.h"
#include "MyVector.hpp"
#include "Player.h"
#include "Admin.h"
#include "SuperHero.h"

class System
{
protected:
	MyVector<Player> players;
	MyVector<Admin> admins;
	MyVector<SuperHero> superheroes;
	MyVector<SuperHero> deadSuperheroes;

	User* currentUser = nullptr;

	static System* instance;
	System();

	virtual void logout() = 0;
	virtual void login() = 0;
	virtual void showMarket() const = 0;
	virtual void showPlayers() const = 0;
	virtual void deleteProfile() = 0;
	virtual void showProfile() = 0;
	void freeSystem();
	void loadSystem();
public:
	System(const System&) = delete;
	System& operator=(const System&) = delete;
	virtual void run() = 0;

	static void freeInstance();
	virtual ~System();
};
