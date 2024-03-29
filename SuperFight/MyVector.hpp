#pragma once
#include "Constants.h"
#include <iostream>

template <typename T>
class MyVector
{
	void* data = nullptr;
	size_t count = 0;
	size_t cap = 0;

	void resize(size_t capacity);
	void copyFrom(const MyVector<T>& other);
	void moveFrom(MyVector<T>&& other) noexcept;
	void free();

	void upsizeIfNeeded();
	void downsizeIfNeeded();

public:

	MyVector();
	explicit MyVector(size_t capacity);
	MyVector(size_t capacity, const T& defaultValue);
	MyVector(const MyVector<T>& other);
	MyVector(MyVector<T>&& other) noexcept;

	MyVector<T>& operator=(const MyVector<T>& other);
	MyVector<T>& operator=(MyVector<T>&& other);

	void push_back(const T& element);
	void push_back(T&& element);
	T& pop_back();
	void erase();
	void clear();
	T& pop_at(size_t index);
	void push_at(const T& element, size_t index);
	void push_at(T&& element, size_t index);

	bool contains(const T& element) const;
	size_t size() const;
	size_t capacity() const;

	bool isEmpty() const;

	T& operator[](size_t index);
	T operator[](size_t index) const;

	~MyVector();
};

template <typename T>
void MyVector<T>::resize(size_t capacity)
{
	if (cap == capacity)
	{
		return;
	}

	this->cap = capacity;
	T* newData = static_cast<T*>(data);
	T* oldData = static_cast<T*>(data);

	for (size_t i = 0; i < count; i++)
	{
		newData[i] = std::move(oldData[i]);
	}

	delete[] oldData;
	data = newData;
}

template <typename T>
void MyVector<T>::moveFrom(MyVector<T>&& other) noexcept
{
	count = other.count;
	cap = other.cap;
	data = other.data;
	other.data = nullptr;
	other.count = 0;
	other.cap = 0;
}

template <typename T>
void MyVector<T>::copyFrom(const MyVector<T>& other)
{
	count = other.count;
	cap = other.cap;
	T* myData = static_cast<T*>(data);
	T* otherData = static_cast<T*>(other.data);

	for (int i = 0; i < count; i++)
	{
		myData[i] = otherData[i];
	}
}

template <typename T>
void MyVector<T>::free()
{
	if (data)
	{
		count = 0;
		cap = 0;
		std::free(data);
		data = nullptr;
	}
}

template <typename T>
MyVector<T>::MyVector() : MyVector(constants::INITIAL_CAPACITY) {}

template <typename T>
MyVector<T>::MyVector(const MyVector<T>& other)
{
	copyFrom(other);
}

template <typename T>
MyVector<T>::MyVector(MyVector<T>&& other) noexcept
{
	moveFrom(std::move(other));
}

template <typename T>
MyVector<T>& MyVector<T>::operator=(const MyVector<T>& other)
{
	if (this != &other)
	{
		free();
		copyFrom(other);
	}

	return *this;
}

template <typename T>
MyVector<T>& MyVector<T>::operator=(MyVector<T>&& other)
{
	if (this != &other)
	{
		free();
		moveFrom(std::move(other));
	}

	return *this;
}

template <typename T>
MyVector<T>::MyVector(size_t capacity) : cap(capacity)
{
	data = malloc(capacity * sizeof(T));
}

template <typename T>
MyVector<T>::MyVector(size_t capacity, const T& defaultValue) : MyVector(capacity)
{
	count = capacity;

	T* myData = static_cast<T*>(data);
	for (size_t i = 0; i < count; i++)
	{
		myData[i] = defaultValue;
	}
}

template <typename T>
void MyVector<T>::push_back(const T& element)
{
	upsizeIfNeeded();

	T* myData = static_cast<T*>(data);
	myData[count++] = element;
}

template <typename T>
void MyVector<T>::push_back(T&& element)
{
	upsizeIfNeeded();

	T* myData = static_cast<T*>(data);
	myData[count++] = std::move(element);
}

template <typename T>
T& MyVector<T>::pop_back()
{
	if (isEmpty())
	{
		throw std::logic_error("Vector is empty!");
	}

	downsizeIfNeeded();

	T* myData = static_cast<T*>(data);
	return myData[--count];
}

template <typename T>
void MyVector<T>::erase()
{
	count = 0;
	cap = constants::INITIAL_CAPACITY;
	resize(cap);
}

template <typename T>
void MyVector<T>::clear()
{
	free();
}

template <typename T>
T& MyVector<T>::pop_at(size_t index)
{
	if (index > count)
	{
		throw std::out_of_range("Index out of range!");
	}

	if (isEmpty())
	{
		throw std::logic_error("Vector is empty!");
	}

	T* myData = static_cast<T*>(data);
	for (size_t i = index; i < count - 1; i++)
	{
		T temp = myData[i];
		myData[i] = myData[i + 1];
		myData[i + 1] = temp;
	}

	return myData[--count];
}

template <typename T>
void MyVector<T>::push_at(const T& element, size_t index)
{
	if (index < count)
	{
		throw std::out_of_range("Index out of range!");
	}

	if (isEmpty())
	{
		throw std::logic_error("Vector is empty!");
	}

	upsizeIfNeeded();


	T* myData = static_cast<T*>(data);
	for (size_t i = count; i > index; i--)
	{
		T temp = myData[i];
		myData[i] = myData[i - 1];
		myData[i - 1] = temp;
	}

	count++;
	data[index] = element;
}

template <typename T>
void MyVector<T>::push_at(T&& element, size_t index)
{
	if (index < count)
	{
		throw std::out_of_range("Index out of range!");
	}

	if (isEmpty())
	{
		throw std::logic_error("Vector is empty!");
	}

	upsizeIfNeeded();


	T* myData = static_cast<T*>(data);
	for (size_t i = count; i > index; i--)
	{
		T temp = data[i];
		data[i] = data[i - 1];
		data[i - 1] = temp;
	}

	count++;
	data[index] = std::move(element);
}


template <typename T>
size_t MyVector<T>::size() const
{
	return count;
}

template <typename T>
size_t MyVector<T>::capacity() const
{
	return cap;
}

template <typename T>
bool MyVector<T>::isEmpty() const
{
	return count == 0;
}

template <typename T>
T& MyVector<T>::operator[](size_t index)
{
	T* myData = static_cast<T*>(data);
	return myData[index];
}

template <typename T>
T MyVector<T>::operator[](size_t index) const
{
	T* myData = static_cast<T*>(data);
	return myData[index];
}

template <typename T>
MyVector<T>::~MyVector()
{
	free();
}

template <typename T>
void MyVector<T>::upsizeIfNeeded()
{
	if (count == cap)
	{
		resize(cap * constants::RESIZE_COEF);
	}
}

template <typename T>
void MyVector<T>::downsizeIfNeeded()
{
	if (count * constants::RESIZE_COEF * constants::RESIZE_COEF <= cap)
	{
		resize(cap / constants::RESIZE_COEF);
	}
}

template <typename T>
bool MyVector<T>::contains(const T& element) const
{
	T* myData = static_cast<T*>(data);
	for (size_t i = 0; i < count; i++)
	{
		if (myData[i] == element)
		{
			return true;
		}
	}

	return false;
}