#pragma once
#include "System.h"

class AdminSystem : public System
{
	AdminSystem() = default;
	void logout() override;
	void login() override;
	void showMarket() override;
	void showGraveyard();
	void showPlayers() override;
	void showAdmins();
	void deleteOwnProfile() override;
	void showProfile() override;
	void deleteSuperhero(std::vector<SuperHero>& superheroes);
	void deletePlayer();
	void addPlayer();
	void addAdmin();
	void addSuperheroToMarket();
	void addSuperheroFromGraveyard();
	void handleEmptyMarket();
public:

	static System* getInstance();
	void run() override;
};

