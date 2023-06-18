#pragma once
#include "MyString.h"

namespace configs
{
	void initConfigs();
	void incrementCountOfAdmins();
	void incrementCountOfPlayers();
	void incrementCountOfMarketSuperheroes();
	void incrementCountOfSoldSuperheroes();
	void decrementCountOfAdmins();
	void decrementCountOfPlayers();
	void decrementCountOfMarketSuperheroes();
	void decrementCountOfSoldSuperheroes();
	bool saveLoggedPlayerInThePeriod(const MyString& nickname);
	bool checkIfPeriodIsOver();
	bool isMarketEmpty();
	bool isSoldEmpty();
	unsigned getCountOfAdmins();
	unsigned getCountOfPlayers();
	unsigned getCountOfMarket();
	unsigned getCountOfSold();
	unsigned getCountOfLogged();
}

