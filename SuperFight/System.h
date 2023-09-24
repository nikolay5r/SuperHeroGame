#pragma once
#include <vector>
#include <cstring>
#include "User.h"
#include "Player.h"
#include "Admin.h"
#include "SuperHero.h"

class System
{
protected:
	std::vector<Player> players;
	std::vector<Admin> admins;
	std::vector<SuperHero> marketSuperheroes;
	std::vector<SuperHero> deadSuperheroes;

	User* currentUser = nullptr;

	template<typename T>
	int findIndexOfEntity(const std::vector<T>& collection, const std::string& nickname);

	static System* instance;
	System();

	virtual void logout() = 0;
	virtual void login() = 0;
	virtual void showMarket() = 0;
	virtual void showPlayers() = 0;
	virtual void deleteOwnProfile() = 0;
	virtual void showProfile() = 0;
	void freeSystem();

public:
	System(const System&) = delete;
	System& operator=(const System&) = delete;
	virtual void run() = 0;

	static void freeInstance();
	virtual ~System();
};
