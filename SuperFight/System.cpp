#include "System.h"
#include "MyString.h"
#include "Player.h"
#include "Admin.h"
#include "User.h"
#include "MyVector.hpp"
#include <fstream>
#include <iostream>
#include "Validation.h"
#include "Regex_Error.h"
#include "File_Error.h"
#include "SuperHero.h"
#include "Input_Error.h"
#include "SystemConfigurations.h"

System* System::instance = nullptr;

void System::freeSystem()
{
	delete System::instance;
	System::instance = nullptr;

	AdminFactory::freeInstance();
	PlayerFactory::freeInstance();
	SuperHeroFactory::freeInstance();
}

void System::freeInstance()
{
	delete System::instance;
	System::instance = nullptr;
}

System::~System()
{
	delete currentUser;
	currentUser = nullptr;
}
