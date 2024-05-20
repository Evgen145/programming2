#pragma once

#include <stdexcept>

template <typename T, size_t N>
class StackVector {
public:
    explicit StackVector(size_t a_size = 0) : size_(static_cast<int>(a_size)) {
        if (a_size > N) {
            throw std::invalid_argument("Size exceeds capacity");
        }
    }

    T& operator[](size_t index) {
        return data_[index];
    }

    const T& operator[](size_t index) const {
        return data_[index];
    }

    T* begin() {
        return data_;
    }

    T* end() {
        return data_ + size_;
    }

    const T* begin() const {
        return data_;
    }

    const T* end() const {
        return data_ + size_;
    }

    size_t Size() const {
        return static_cast<size_t>(size_);
    }

    size_t Capacity() const {
        return N;
    }

    void PushBack(const T& value) {
        if (size_ >= N) {
            throw std::overflow_error("Vector capacity exceeded");
        }
        data_[size_++] = value;
    }

    T PopBack() {
        if (size_ == 0) {
            throw std::underflow_error("Vector is empty");
        }
        return data_[--size_];
    }

private:
    T data_[N];
    size_t size_;
};
