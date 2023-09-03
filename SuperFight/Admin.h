#pragma once
#include "User.h"
#include "MyVector.hpp"

class Admin : public User
{
public:
	Admin(const MyString& firstName, const MyString& lastName, const MyString& nickname, const MyString& email, const MyString& password);
	Admin(const User& user);
	Admin(User&& user);

	void printShortInfo() const override;
	void printFullInfo() const override;
};

MyVector<Admin> readAdminsFromFile(const MyString& fileName);
Admin readAdminFromFile(std::ifstream& file);
Admin createAdminFromConsole();
void saveAdminToFile(std::ofstream& file, const Admin& admin);
void saveAdminsToFile(const MyString& fileName, const MyVector<Admin>& admins);