#ifndef SIMPLE_VECTOR_H
#define SIMPLE_VECTOR_H

#include <iostream>
#include <algorithm>

template<typename T>
class SimpleVector {
public:
    SimpleVector() = default;
    explicit SimpleVector(size_t size);
    SimpleVector(const SimpleVector& other);
    ~SimpleVector();

    T& operator[](size_t index);

    T* begin();
    T* end();
    const T* begin() const;
    const T* end() const;

    size_t Size() const;
    size_t Capacity() const;
    void PushBack(const T& value);

    // Assignment operator
    void operator=(const SimpleVector& other);

private:
    T* data = nullptr;
    size_t size = 0;
    size_t capacity = 0;
};

template<typename T>
SimpleVector<T>::SimpleVector(size_t size) :
    data(new T[size]),
    size(size),
    capacity(size) {
}

template<typename T>
SimpleVector<T>::SimpleVector(const SimpleVector<T>& other) :
    data(new T[other.capacity]),
    size(other.size),
    capacity(other.capacity) {
    std::copy(other.begin(), other.end(), begin());
}

template<typename T>
SimpleVector<T>::~SimpleVector() {
    delete[] data;
}

template<typename T>
T& SimpleVector<T>::operator[](size_t index) {
    return data[index];
}

template<typename T>
size_t SimpleVector<T>::Size() const {
    return size;
}

template<typename T>
size_t SimpleVector<T>::Capacity() const {
    return capacity;
}

template<typename T>
void SimpleVector<T>::PushBack(const T& value) {
    if (size >= capacity) {
        auto new_cap = capacity == 0 ? 1 : 2 * capacity;
        auto new_data = new T[new_cap];
        std::copy(begin(), end(), new_data);
        delete[] data;
        data = new_data;
        capacity = new_cap;
    }
    data[size++] = value;
}

template<typename T>
T* SimpleVector<T>::begin() {
    return data;
}

template<typename T>
const T* SimpleVector<T>::begin() const {
    return data;
}

template<typename T>
T* SimpleVector<T>::end() {
    return data + size;
}

template<typename T>
const T* SimpleVector<T>::end() const {
    return data + size;
}

template<typename T>
void SimpleVector<T>::operator=(const SimpleVector& other) {
    // Check for self-assignment
    if (this == &other) {
        return;
    }

    // Allocate new memory with the same capacity as the other vector
    T* new_data = new T[other.capacity];

    // Copy the elements from the other vector to the new memory
    std::copy(other.begin(), other.end(), new_data);

    // Update the size and capacity of the current vector
    size = other.size;
    capacity = other.capacity;

    // Delete the old memory
    delete[] data;

    // Update the data pointer to point to the new memory
    data = new_data;
}

#endif // SIMPLE_VECTOR_H
