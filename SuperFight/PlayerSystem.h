#pragma once
#include "System.h"

class PlayerSystem : public System
{
	PlayerSystem() = default;
	void logout() override;
	void login() override;
	void reg() override;
	void showMarket() const override;
	void showPlayers() const override;
	void buySuperHero() const;
	void sellSuperHero() const;
	void battle() const;
	void deleteProfile() override;
	void upgradeSuperHero() const;
	void showProfile() override;
	void changePos();
public:

	static System* getInstance();
	void run() override;

};

