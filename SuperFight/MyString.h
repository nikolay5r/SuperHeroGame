#pragma once
#include <iostream>

class MyString
{
	union Data
	{
		char _smallString[sizeof(char*)];
		char* _data = nullptr;
	} data;
	size_t _capacity = 0;
	size_t _length = 0;

	static const int SSOFlagMask;
	static const int lengthMask;

	void copyFrom(const MyString& data);
	void free();
	void move(MyString&& other);

	bool isSSO() const;
	size_t getLengthInSSO() const;

	void setSSOLength(size_t capacity);

public:
	explicit MyString(size_t capacity);

	MyString();
	MyString(const char* data);
	MyString(const MyString& other);
	MyString(MyString&& other) noexcept;
	MyString& operator=(const MyString& other);
	MyString& operator=(MyString&& other) noexcept;
	~MyString();

	size_t length() const;
	MyString& operator+=(const MyString& other);

	MyString substr(size_t begin, size_t howMany) const;

	char& operator[](size_t index);
	char operator[](size_t index) const;

	const char* c_str() const;

	int find(char data) const;
	int find(char data, unsigned index) const;
	int find(const char* data) const;
	int find(const char* data, unsigned index) const;

	friend MyString operator+(const MyString& lhs, const MyString& rhs);
	friend std::istream& operator>>(std::istream&, MyString& str);
};
MyString operator+(const MyString& lhs, const MyString& rhs);

std::ostream& operator<<(std::ostream& os, const MyString& str);

std::istream& operator>>(std::istream& os, MyString& str);

bool operator<(const MyString& lhs, const MyString& rhs);
bool operator<=(const MyString& lhs, const MyString& rhs);
bool operator>=(const MyString& lhs, const MyString& rhs);
bool operator>(const MyString& lhs, const MyString& rhs);
bool operator==(const MyString& lhs, const MyString& rhs);
bool operator!=(const MyString& lhs, const MyString& rhs);