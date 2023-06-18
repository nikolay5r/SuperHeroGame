#pragma once
#include "System.h"

class AdminSystem : public System
{
	AdminSystem() = default;
	void logout() override;
	void login() override;
	void reg() override;
	void showMarket() const override;
	void showSoldMarket() const;
	void showPlayers() const override;
	void showAdmins() const;
	void deleteProfile() override;
	void showProfile() override;
	void deleteSuperheroFromMarket() const;
	void deleteSuperheroFromSold() const;
	void deletePlayer() const;
	void addPlayer() const;
	void addAdmin() const;
	void addSuperhero() const;
	void addSuperheroFromSold() const;
	void handleEmptyMarket() const;
public:

	static System* getInstance();
	void run() override;
};

