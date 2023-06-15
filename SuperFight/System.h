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
public:
	System(const System&) = delete;
	System& operator=(const System&) = delete;
	virtual void run() = 0;

	static void freeInstance();
	virtual ~System();
};

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
public:

	static System* getInstance();
	void run() override;

};

class AdminSystem : public System
{
	AdminSystem() = default;
	void logout() override {};
	void login() override {};
	void reg() override {};
	void showMarket() const override {};
	void showPlayers() const override {};
	void deleteProfile() override {};
public:

	static System* getInstance();
	void run() override {};
};