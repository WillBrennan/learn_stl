#pragma once

#include <cstring>

#include <algorithm>
#include <iterator>
#include <memory>

#include "learn_stl/algorithm.h"

namespace learn {
template <typename ValueT, class AllocatorT = std::allocator<ValueT>>
class vector {
  public:
    using value_type = ValueT;
    using allocator = AllocatorT;
    using size_type = std::size_t;
    using difference_type = std::ptrdiff_t;
    using reference = value_type&;
    using const_reference = const value_type&;
    using pointer = value_type*;
    using const_pointer = const value_type*;
    using iterator = pointer;
    using const_iterator = const_pointer;
    using reverse_iterator = std::reverse_iterator<iterator>;
    using const_reverse_iterator = std::reverse_iterator<const_iterator>;

    vector() noexcept(noexcept(allocator())) : vector(allocator()) {}
    explicit vector(const allocator& new_allocator) noexcept : allocator_(new_allocator) {}

    vector(size_type count, const value_type& value, const allocator& alloc = allocator())
        : allocator_(alloc) {
        reserve(count);

        for (size_type i = 0; i < count; ++i) {
            emplace_back(value);
        }
    }

    ~vector() {
        if (begin_) {
            clear();
            AllocatorTraits::deallocate(allocator_, begin_, capacity());
        }
    }

    // element access

    reference at(size_type index) {
        if (index >= size()) {
            throw std::out_of_range("indexed the vector out of bounds");
        }

        return operator[](index);
    }

    const_reference at(size_type index) const {
        if (index >= size()) {
            throw std::out_of_range("indexed the vector out of bounds");
        }

        return operator[](index);
    }

    reference operator[](size_type index) { return *(begin() + index); }
    const_reference operator[](size_type index) const { return *(begin() + index); }

    reference front() { return *begin_; }
    const_reference front() const { return *begin_; }

    reference back() { return *(begin_ + (size() - 1)); }
    const_reference back() const { return *(begin_ + (size() - 1)); }

    pointer data() { return begin_; }
    const_pointer data() const { return begin_; }

    // iterators

    iterator begin() { return begin_; }
    iterator end() { return begin() + size(); }

    const_iterator begin() const { return begin_; }
    const_iterator end() const { return begin() + size(); }

    // capacity

    size_type size() const { return size_; }
    size_type capacity() const { return capacity_; }
    size_type max_size() const;

    void reserve(size_type new_capacity);

    // modifiers

    template <class... Args>
    reference emplace_back(Args&&... args) {
        if (size_ == capacity_) {
            reserve(recommend(size_));
        }

        AllocatorTraits::construct(allocator_, begin_ + size_, std::forward<Args>(args)...);
        size_ += 1;

        return back();
    }

    void erase(iterator pos) { erase(pos, std::next(pos, 1)); }

    void erase(iterator first, iterator last);

    void clear();

  private:
    using AllocatorTraits = std::allocator_traits<allocator>;
    allocator allocator_;
    pointer begin_ = nullptr;

    size_type size_ = 0;
    size_type capacity_ = 0;

    size_type recommend(const size_type& size) const;
};

template <typename Value, class Allocator>
typename vector<Value, Allocator>::size_type vector<Value, Allocator>::max_size() const {
    const auto max_size = AllocatorTraits::max_size(allocator_);
    const auto max_numeric = std::numeric_limits<difference_type>::max();

    return std::min<size_type>(max_size, max_numeric);
}

template <typename Value, class Allocator>
void vector<Value, Allocator>::reserve(size_type new_capacity) {
    if (new_capacity <= capacity_) {
        return;
    }

    const auto new_begin = AllocatorTraits::allocate(allocator_, new_capacity);
    if (begin_) {
        if
            constexpr(std::is_trivially_constructible<Value>::value) {
                std::memmove(new_begin, begin_, size_ * sizeof(value_type));
            }
        else {
            std::move(begin_, begin_ + size_, new_begin);
        }

        AllocatorTraits::deallocate(allocator_, begin_, capacity_);
    }

    begin_ = new_begin;
    capacity_ = new_capacity;
};

template <typename Value, class Allocator>
typename vector<Value, Allocator>::size_type vector<Value, Allocator>::recommend(
    const size_type& size) const {
    const size_type max_allowed = max_size();

    if (size > max_allowed) {
        throw std::runtime_error("error! cant allocate this size!");
    }

    const size_type current_capacity = capacity();

    const auto new_capacity = std::max(size_type(5), 3 * size / 2);

    return std::clamp(new_capacity, size, max_allowed);
}

template <typename Value, class Allocator>
void vector<Value, Allocator>::erase(iterator first, iterator last) {
    // rotate so that first->last is at the end of the vector
    rotate(first, last, end());
    size_ -= std::distance(first, last);
}

template <typename Value, class Allocator>
void vector<Value, Allocator>::clear() {
    erase(begin(), end());
}

}  // namespace learn