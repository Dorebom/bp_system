#pragma once

#include <iostream>
#include <vector>
#include <mutex>

template <typename T>
class circular_stacker {
public:
    circular_stacker(size_t size) : size_(size), buffer_(size) {
        head_ = tail_ = count_ = 0;
    }

    ~circular_stacker()
    {
        buffer_.clear();
    }

    // 要素を追加
    void push(const T& value) {
        std::lock_guard<std::mutex> lock(mtx_);
        if (count_ < size_) {
            buffer_[tail_] = value;
            tail_ = (tail_ + 1) % size_;
            count_++;
        } else {
            std::cerr << "Circular stacker is full. Cannot push more elements." << std::endl;
        }
    }

    // 要素を取得
    T pop() {
        std::lock_guard<std::mutex> lock(mtx_);
        if (count_ > 0) {
            T value = buffer_[head_];
            head_ = (head_ + 1) % size_;
            count_--;
            return value;
        } else {
            std::cerr << "Circular stacker is empty. Cannot pop elements." << std::endl;
            return T();  // デフォルト値を返す
        }
    }

    // 現在の要素数を取得
    size_t size() const {
        return count_;
    }

    // スタックの最大要素数を取得
    size_t max_size() const {
        return size_;
    }

private:
    size_t size_;
    std::vector<T> buffer_;
    size_t head_;
    size_t tail_;
    size_t count_;
    std::mutex mtx_;
};