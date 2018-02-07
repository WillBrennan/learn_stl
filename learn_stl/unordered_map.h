#pragma once

#include <utility>

#include "detail/unordered_container.h"

namespace learn {
template <class Key, class Object, class Hash = std::hash<Key>, class KeyEqual = std::equal_to<Key>,
          class Allocator = std::allocator<std::pair<const Key, Object>>>
class unordered_map {
  public:
    using key_type = Key;
    using mapped_type = Object;
    using value_type = std::pair<const key_type, mapped_type>;
    using size_type = std::size_t;
    using difference_type = std::ptrdiff_t;
    using Data = detail::Unordered<value_type, Hash, KeyEqual, Allocator>;
    using iterator = typename Data::Iterator;
    using const_iterator = typename Data::ConstIterator;

    unordered_map() = default;

    // iterators
    iterator begin() noexcept { return data_.begin(); }
    iterator end() noexcept { return data_.end(); }

    const_iterator begin() const noexcept { return data_.begin(); }
    const_iterator end() const noexcept { return data_.end(); }

    // capacity
    bool empty() const noexcept { return data_.empty(); }
    size_type size() const noexcept { return data_.size(); }
    size_type max_size() const noexcept { return data_.max_size(); }

    // modifiers
    template <class... Args>
    std::pair<iterator, bool> emplace(Args&&... args) {
        return data_.emplace(std::forward<Args>(args)...);
    }

    // lookup

    // bucket interface

    // hash policy

  private:
    Data data_;
};

}  // namespace learn