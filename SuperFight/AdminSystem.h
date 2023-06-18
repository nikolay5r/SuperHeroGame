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
	void deleteProfile() override;
	void showProfile() override;
public:

	static System* getInstance();
	bool run() override;
};

