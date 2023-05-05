#include "MyString.h"

#include <iostream>
#include <cstring>
#pragma warning(disable:4996)

const int MyString::SSO_FLAG_MASK = 0b10000000;
const int MyString::SSO_LENGTH_MASK = 0b01111100;

MyString::MyString(size_t capacity)
{
	if (capacity < sizeof(MyString))
	{
		_length = MyString::SSO_FLAG_MASK;
		_length <<= (sizeof(size_t) - 1) * 8;
		setSSOLength(capacity);
	}
	else if (capacity < pow(2, (sizeof(size_t) * 8) - 1))
	{
		_capacity = capacity;
		data._data = new char[_capacity];
	}
	else
		throw std::length_error("String length is out of bounds!");
}

void MyString::move(MyString&& other)
{
	_capacity = other._capacity;
	_length = other._length;

	if (isSSO())
		strcpy(data._smallString, other.data._smallString);
	else
	{
		data._data = other.data._data;
		other.data._data = nullptr;
	}
}

MyString::MyString(MyString&& other) noexcept
{
	move(std::move(other));
}

MyString& MyString::operator=(MyString&& other) noexcept
{
	if (this != &other)
	{
		free();
		move(std::move(other));
	}

	return *this;
}

void MyString::setSSOLength(size_t capacity)
{
	(--capacity) <<= 2;
	capacity <<= (sizeof(size_t) - 1) * 8;
	_length |= capacity;
}

MyString operator+(const MyString& lhs, const MyString& rhs)
{
	size_t newCapacity = lhs.length() + rhs.length() + 1;

	MyString result(newCapacity);

	bool SSOValidation = result.isSSO();

	char* ptr = SSOValidation ? result.data._smallString : result.data._smallString;
	const char* ptrLhs = lhs.isSSO() ? lhs.data._smallString : lhs.data._data;
	const char* ptrRhs = rhs.isSSO() ? rhs.data._smallString : rhs.data._data;

	result[0] = '\0';
	strcat(ptr, ptrLhs);
	strcat(ptr, ptrRhs);

	if (SSOValidation)
		result.setSSOLength(newCapacity);

	return result;
}

MyString& MyString::operator+=(const MyString& other)
{
	size_t totalNewCapacity = length() + other.length() + 1;

	if (totalNewCapacity < sizeof(MyString))
		strcat(data._smallString, other.data._smallString);
	else
	{
		bool SSOValidation = isSSO();

		if (_capacity < totalNewCapacity || SSOValidation)
		{
			char* result = new char[totalNewCapacity];
			const char* ptr = SSOValidation ? data._smallString : data._data;
			const char* ptrOther = other.isSSO() ? other.data._smallString : other.data._data;

			result[0] = '\0';
			strcat(result, ptr);
			strcat(result, ptrOther);

			_capacity = totalNewCapacity;
			if (!SSOValidation)
				delete[] data._data;
			data._data = result;
		}
		else
			strcat(data._data, other.data._data);

		_length = totalNewCapacity - 1;
	}

	return *this;
}

MyString::MyString() : MyString(1) {
	data._smallString[0] = 0;
}

MyString::MyString(const char* data) : MyString(strlen(data) + 1)
{
	if (isSSO())
		strcpy(this->data._smallString, data);
	else
	{
		strcpy(this->data._data, data);
		_length = strlen(data);
	}
}

MyString::MyString(const MyString& other)
{
	copyFrom(other);
}

MyString& MyString::operator=(const MyString& other)
{
	if (this != &other)
	{
		free();
		copyFrom(other);
	}
	return *this;
}

void MyString::free()
{
	if (!isSSO() && data._data == nullptr)
	{
		delete[] data._data;
		data._data = nullptr;
	}
}

MyString::~MyString()
{
	free();
}

size_t MyString::length() const
{
	if (isSSO())
	{
		return getLengthInSSO();
	}
	return _length;
}

void MyString::copyFrom(const MyString& other)
{
	if (other.isSSO())
	{
		strcpy(data._smallString, other.data._smallString);
		_length |= other._length;
	}
	else
	{
		if (_capacity < other._capacity)
		{
			_capacity = other._capacity;
			data._data = new char[_capacity];
		}

		_length = other._length;
		strcpy(data._data, other.data._data);
	}
}

char& MyString::operator[](size_t index)
{
	if (isSSO())
		return data._smallString[index];
	return data._data[index];
}

char MyString::operator[](size_t index) const
{
	if (isSSO())
		return data._smallString[index];
	return data._data[index];
}

MyString MyString::substr(size_t begin, size_t howMany) const
{
	if (begin + howMany > length())
		throw std::length_error("Error, substrs out of range");

	MyString res(howMany + 1);

	bool SSOValidation = res.isSSO();

	char* ptrRes = SSOValidation ? res.data._smallString : res.data._data;
	const char* ptr = isSSO() ? data._smallString : data._data;

	for (int i = 0; i < howMany; i++)
		ptrRes[i] = ptr[begin + i];
	res[howMany] = '\0';

	if (SSOValidation)
		res.setSSOLength(howMany + 1);

	return res;
}

const char* MyString::c_str() const
{
	if (isSSO())
	{
		return data._smallString;
	}
	return data._data;
}

std::ostream& operator<<(std::ostream& os, const MyString& str)
{
	return os << str.c_str();
}

std::istream& operator>>(std::istream& is, MyString& str)
{
	char buff[1024];
	is.getline(buff, 1024);
	size_t buffLength = strlen(buff);

	if (!str.isSSO())
		delete[] str.data._data;

	if (buffLength + 1 < sizeof(MyString))
	{
		strcpy(str.data._smallString, buff);

		size_t SSOLength = MyString::SSO_FLAG_MASK;
		buffLength <<= 2;

		SSOLength |= buffLength;
		SSOLength <<= (sizeof(size_t) - 1) * 8;

		str._length |= SSOLength;
	}
	else
	{
		if (str._capacity <= buffLength)
		{
			str.data._data = new char[buffLength + 1];
			str._capacity = buffLength + 1;
		}

		strcpy(str.data._data, buff);
		str._length = buffLength;
	}
	return is;
}


bool operator<(const MyString& lhs, const MyString& rhs)
{
	return strcmp(lhs.c_str(), rhs.c_str()) < 0;
}

bool operator<=(const MyString& lhs, const MyString& rhs)
{
	return strcmp(lhs.c_str(), rhs.c_str()) <= 0;
}
bool operator>=(const MyString& lhs, const MyString& rhs)
{
	return strcmp(lhs.c_str(), rhs.c_str()) >= 0;
}
bool operator>(const MyString& lhs, const MyString& rhs)
{
	return strcmp(lhs.c_str(), rhs.c_str()) > 0;
}
bool operator==(const MyString& lhs, const MyString& rhs)
{
	return strcmp(lhs.c_str(), rhs.c_str()) == 0;
}
bool operator!=(const MyString& lhs, const MyString& rhs)
{
	return strcmp(lhs.c_str(), rhs.c_str()) != 0;
}

bool MyString::isSSO() const
{
	size_t temp = _length >> (sizeof(size_t) - 1) * 8;
	return temp & MyString::SSO_FLAG_MASK;
}

size_t MyString::getLengthInSSO() const
{
	size_t temp = _length >> (sizeof(size_t) - 1) * 8;
	return ((temp & MyString::SSO_LENGTH_MASK) >> 2);
}

size_t MyString::find(char data, unsigned index) const
{
	const char* ptr = isSSO() ? this->data._smallString : this->data._data;
	size_t len = length();

	for (size_t i = index; i < len; i++)
		if (ptr[i] == data)
			return i;

	return std::string::npos;
}

size_t MyString::find(char data) const
{
	return find(data, 0);
}

size_t MyString::find(const char* data) const
{
	return find(data, 0);
}

size_t MyString::find(const char* data, unsigned index) const
{
	size_t dataLength = strlen(data);

	const char* ptr = isSSO() ? this->data._smallString : this->data._data;

	while ((index = find(data[0], index)) != -1)
	{
		for (size_t i = 1; i < dataLength; i++)
		{
			if (data[i] != ptr[i + index])
				break;
		}

		return index;
	}

	return std::string::npos;
}

MyString MyString::substr(size_t begin) const
{
	return substr(begin, length() - begin);
}