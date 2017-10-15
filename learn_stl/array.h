#pragma once

#include <algorithm>
#include <cstdlib>
#include <iterator>
#include <stdexcept>

#include "tuple.h"

namespace learn {
template <typename Value, std::size_t Size>
struct array {
    using value_type = Value;
    using size_type = std::size_t;
    using reference = value_type&;
    using const_reference = const value_type&;
    using pointer = value_type*;
    using const_pointer = const value_type*;
    using iterator = value_type*;
    using const_iterator = const value_type*;
    using reverse_iterator = std::reverse_iterator<iterator>;
    using const_reverse_iterator = std::reverse_iterator<const_iterator>;

    value_type elems_[Size > 0 ? Size : 1];

    iterator begin() noexcept { return iterator(elems_); }
    const_iterator begin() const noexcept { return const_iterator(elems_); }
    const_iterator cbegin() const noexcept { return const_iterator(elems_); }

    iterator end() { return iterator(elems_ + Size); }
    const_iterator end() const noexcept { return const_iterator(elems_ + Size); }
    const_iterator cend() const noexcept { return const_iterator(elems_ + Size); }

    reverse_iterator rbegin() noexcept { return reverse_iterator(end()); }
    const_reverse_iterator rbegin() const noexcept { return const_reverse_iterator(end()); }

    reverse_iterator rend() noexcept { return reverse_iterator(begin()); }
    const_reverse_iterator rend() const noexcept { return const_reverse_iterator(begin()); }

    bool empty() const noexcept { return Size == 0; }
    size_type size() const noexcept { return Size; }
    size_type max_size() const noexcept { return Size; }

    void fill(const value_type& value) { std::fill_n(elems_, Size, value); }
    void swap(array& other) { std::swap(elems_, other.elems_); }

    reference at(size_type pos) {
        if (pos >= Size) {
            throw std::out_of_range("out of range access of array");
        }

        return elems_[pos];
    }

    const_reference at(size_type pos) const {
        if (pos >= Size) {
            throw std::out_of_range("out of range access of array");
        }

        return elems_[pos];
    }

    reference operator[](size_type pos) { return elems_[pos]; }
    const_reference operator[](size_type pos) const { return elems_[pos]; }

    reference front() { return elems_[0]; }
    const_reference front() const { return elems_[0]; }

    reference back() { return elems_[Size > 0 ? Size - 1 : 0]; }
    const_reference back() const { return elems_[Size > 0 ? Size - 1 : 0]; }

    pointer data() { return pointer(elems_); }
    const_pointer data() const { return const_pointer(elems_); }
};

template <size_t I, class Type, std::size_t Size>
class tuple_element<I, array<Type, Size>> {
    static_assert(I < Size, "Index out of bounds in learn::tuple_element<> (learn::array)");

  public:
    using type = Type;
};

template <size_t I, class Type, std::size_t Size>
Type& get(array<Type, Size>& data) {
    static_assert(I < Size, "Index out of bounds in learn::get<> (learn::array)");
    return data[I];
}

template <size_t I, class Type, std::size_t Size>
const Type& get(const array<Type, Size>& data) {
    static_assert(I < Size, "Index out of bounds in learn::get<> (learn::array)");
    return data[I];
}

template <size_t I, class Type, std::size_t Size>
Type&& get(array<Type, Size>&& data) {
    static_assert(I < Size, "Index out of bounds in learn::get<> (learn::array)");
    return data[I];
}

template <size_t I, class Type, std::size_t Size>
const Type&& get(const array<Type, Size>&& data) {
    static_assert(I < Size, "Index out of bounds in learn::get<> (learn::array)");
    return data[I];
}

}  // namespace learn