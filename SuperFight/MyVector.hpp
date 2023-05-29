#pragma once
#include <iostream>

template <typename T>
class MyVector
{
	T* data = nullptr;
	size_t count = 0;
	size_t cap = 0;

	void resize(size_t capacity);
	void moveFrom(MyVector&& other) noexcept;
	void copyFrom(const MyVector& other);
	void free();

	void upsizeIfNeeded();
	void downsizeIfNeeded();

public:

	MyVector();
	explicit MyVector(size_t capacity);
	MyVector(size_t capacity, const T& defaultValue);
	MyVector(const MyVector& other);
	MyVector(MyVector&& other) noexcept;

	MyVector& operator=(const MyVector& other);
	MyVector& operator=(MyVector&& other);

	void push_back(const T& element);
	void push_back(T&& element);
	T pop_back();
	void erase();
	void clear();
	T pop_at(size_t index);
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

static const int INITIAL_CAPACITY = 4;
static const int RESIZE_COEF = 2;

template <typename T>
void MyVector<T>::resize(size_t capacity)
{
	this->cap = capacity;
	T* newData = new T[capacity];

	for (size_t i = 0; i < count; i++)
	{
		newData[i] = std::move(data[i]);
	}

	delete[] data;
	data = newData;
}

template <typename T>
void MyVector<T>::moveFrom(MyVector&& other) noexcept
{
	count = other.count;
	cap = other.cap;
	data = other.data;
	other.data = nullptr;
	other.count = 0;
	other.cap = 0;
}

template <typename T>
void MyVector<T>::copyFrom(const MyVector& other)
{
	count = other.count;
	cap = other.cap;
	data = new T[cap];

	for (int i = 0; i < count; i++)
	{
		data[i] = other.data[i];
	}
}

template <typename T>
void MyVector<T>::free()
{
	if (data)
	{
		delete[] data;
		data = nullptr;
	}
}

template <typename T>
MyVector<T>::MyVector() : MyVector(INITIAL_CAPACITY) {}

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
	count = capacity;
	data = new T[capacity];
}

template <typename T>
MyVector<T>::MyVector(size_t capacity, const T& defaultValue) : MyVector(capacity)
{
	count = capacity;
	for (size_t i = 0; i < count; i++)
	{
		data[i] = defaultValue;
	}
}

template <typename T>
void MyVector<T>::push_back(const T& element)
{
	upsizeIfNeeded();
	data[count++] = element;
}

template <typename T>
void MyVector<T>::push_back(T&& element)
{
	upsizeIfNeeded();
	data[count++] = std::move(element);
}

template <typename T>
T MyVector<T>::pop_back()
{
	if (isEmpty())
	{
		throw std::logic_error("Vector is empty!");
	}

	downsizeIfNeeded();

	return data[--count];
}

template <typename T>
void MyVector<T>::erase()
{
	count = 0;
	cap = INITIAL_CAPACITY;
	resize(cap);
}

template <typename T>
void MyVector<T>::clear()
{
	free();
}

template <typename T>
T MyVector<T>::pop_at(size_t index)
{
	if (index < count)
	{
		throw std::out_of_range("Index out of range!");
	}

	if (isEmpty())
	{
		throw std::logic_error("Vector is empty!");
	}

	for (size_t i = index; i < count - 1; i++)
	{
		T temp = data[i];
		data[i] = data[i + 1];
		data[i + 1] = temp;
	}

	return data[--count];
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

	for (size_t i = count; i > index; i--)
	{
		T temp = data[i];
		data[i] = data[i - 1];
		data[i - 1] = temp;
	}

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

	for (size_t i = count; i > index; i--)
	{
		T temp = data[i];
		data[i] = data[i - 1];
		data[i - 1] = temp;
	}

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
	return cap == 0;
}

template <typename T>
T& MyVector<T>::operator[](size_t index)
{
	return data[index];
}

template <typename T>
T MyVector<T>::operator[](size_t index) const
{
	return data[index];
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
		resize(cap * RESIZE_COEF);
	}
}

template <typename T>
void MyVector<T>::downsizeIfNeeded()
{
	if (count * RESIZE_COEF * RESIZE_COEF <= cap)
	{
		resize(cap / RESIZE_COEF);
	}
}

template <typename T>
bool MyVector<T>::contains(const T& element) const
{
	for (size_t i = 0; i < count; i++)
	{
		if (data[i] == element)
		{
			return true;
		}
	}

	return false;
}