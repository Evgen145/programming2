#pragma once

#include <cstdlib>
#include <algorithm>

template <typename T>
class SimpleVector {
public:
    SimpleVector() : data(nullptr), size_(0), capacity_(0) {}

    explicit SimpleVector(size_t size) : data(new T[size]), size_(size), capacity_(size) {}

    ~SimpleVector() {
        delete[] data;
    }

    T& operator[](size_t index) {
        return data[index];
    }

    T* begin() {
        return data;
    }

    T* end() {
        return data + size_;
    }

    size_t Size() const {
        return size_;
    }

    size_t Capacity() const {
        return capacity_;
    }

    void PushBack(const T& value) {
        if (size_ == capacity_) {
            size_t newCapacity = capacity_ ? 2 * capacity_ : 1;
            T* newData = new T[newCapacity];
            std::copy(data, data + size_, newData);
            delete[] data;
            data = newData;
            capacity_ = newCapacity;
        }
        data[size_++] = value;
    }

private:
    T* data;
    size_t size_;
    size_t capacity_;
};