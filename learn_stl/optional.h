#pragma once

#include <type_traits>
#include <typeinfo>

namespace learn {
namespace detail {

struct dummy_t {};

template <typename T>
union optional_storage {
    static_assert(std::is_trivially_destructible<T>::value, "");

    dummy_t dummy_;
    T value_;

    constexpr optional_storage()  // null-state ctor
        : dummy_{} {}

    constexpr optional_storage(T const& v)  // value ctor
        : value_{v} {}

    ~optional_storage() = default;  // trivial dtor
};
}  // namespace detail

template <typename Object>
class optional {
  public:
    optional() : has_value_(false) {}

    optional(Object&& object) : has_value_(true), storage_(std::forward<Object>(object)) {}

    optional(const Object& object) : has_value_(true), storage_(object) {}

    optional& operator=(const Object& object) {
        has_value_ = true;
        storage_ = object;
        return *this;
    }

    bool has_value() const noexcept { return has_value_; }
    explicit operator bool() const noexcept { return has_value_; }

    Object& value() {
        if (!has_value_) {
            throw std::bad_cast();
        }
        return storage_.value_;
    }

    const Object& value() const {
        if (!has_value_) {
            throw std::bad_cast();
        }
        return storage_.value_;
    }

    template <typename AltObject>
    Object value_or(AltObject&& default_value) const {
        if (!has_value_) {
            return default_value;
        }

        value();
    }

    void swap(optional& other) noexcept {
        if (!has_value_ && !other.has_value_) {
            return;
        }

        if (has_value_ && !other.has_value) {
            other = std::move(**this);
            this->operator~();
        }

        if (!has_value_ && other.has_value) {
            **this = std::move(other);
            other->operator~();
        }

        std::swap(**this, *other);
    }

    void reset() noexcept { has_value_ = false; }

    template <typename... Args>
    Object& emplace(Args&&... args) {
        has_value_ = true;
        storage_ = std::move(Object(args...));
        return storage_.value_;
    }

  private:
    using Storage = detail::optional_storage<Object>;
    bool has_value_;
    Storage storage_;
};

}  // namespace learn