
User::User(MyString firstName, MyString lastName, MyString userName, MyString email, MyString password)
{
	setFirstName(firstName);
	setLastName(lastName);
	setFullName();
	setEmail(email);
	setPassword(password);
	setUsername(userName);
}

const MyString& User::getPassword() const noexcept
{
	return password;
}

const MyString& User::getEmail() const noexcept
{
	return email;
}

const MyString& User::getUserName() const noexcept
{
	return userName;
}

const MyString& User::getFullName() const noexcept
{
	return fullName;
}

const MyString& User::getFirstName() const noexcept
{
	return firstName;
}

const MyString& User::getLastName() const noexcept
{
	return lastName;
}