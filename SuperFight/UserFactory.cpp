#include "UserFactory.h"

UserFactory* UserFactory::getInstance()
{

}

void UserFactory::freeInstance()
{

}

Player* UserFactory::createPlayerFromBinary(std::ifstream&) const
{

}

Admin* UserFactory::createAdminFromBinary(std::ifstream&) const
{

}

Player* UserFactory::createPlayerFromConsole(std::istream&) const
{

}

Admin* UserFactory::createAdminFromConsole(std::istream&) const
{

}

UserFactory::~UserFactory()
{

}