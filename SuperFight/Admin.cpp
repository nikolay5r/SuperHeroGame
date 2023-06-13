#include "Admin.h"
#include "User.h"
#include <utility>

Admin::Admin(const MyString& firstName, const MyString& lastName, const MyString& username, const MyString& email, const MyString& password)
	: User(firstName, lastName, username, email, password, UserRole::Admin) {}

Admin::Admin(const User& user) : User(user) {}

Admin::Admin(User&& user) : User(std::move(user)) {}

void Admin::print() const
{
	std::cout << username << " | " << fullName << " | " << email;
}