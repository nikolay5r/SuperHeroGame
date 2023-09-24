#pragma once
#include "User.h"
#include <vector>

class Admin : public User
{
public:
	Admin(const std::string& firstName, const std::string& lastName, const std::string& nickname, const std::string& email, const std::string& password);
	Admin(const User& user);
	Admin(User&& user);

	void printShortInfo() const override;
	void printFullInfo() const override;
};

std::vector<Admin> readAdminsFromFile(const std::string& fileName);
Admin readAdminFromFile(std::ifstream& file);
Admin createAdminFromConsole();
void saveAdminToFile(std::ofstream& file, const Admin& admin);
void saveAdminsToFile(const std::string& fileName, const std::vector<Admin>& admins);