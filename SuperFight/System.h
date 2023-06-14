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

	void logout();
	void login();
	void reg();
public:

	static System* getInstance();
	void run() override;

};

class AdminSystem : public System
{
	AdminSystem() = default;
public:

	static System* getInstance();
	void run() override {};
};