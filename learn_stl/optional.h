#pragma once

#include <type_traits>
#include <typeinfo>

namespace learn {

template <typename Object>
class optional {
  public:
    optional() : has_value_(false) {}
    optional(Object&& object) : has_value_(true) {
        new (&storage_) Object(std::forward<Object>(object));
    }
    optional(const Object& object) : has_value_(true) { new (&storage_) Object(object); }

    optional& operator=(const Object& object) {
        has_value_ = true;
        new (&storage_) Object(object);
        return *this;
    }

    bool has_value() const noexcept { return has_value_; }
    explicit operator bool() const noexcept { return has_value_; }

    Object& value() {
        if (!has_value_) {
            throw std::bad_cast();
        }
        return *reinterpret_cast<Object*>(&storage_);
    }

    const Object& value() const {
        if (!has_value_) {
            throw std::bad_cast();
        }
        return *reinterpret_cast<const Object*>(&storage_);
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
        return *new (&storage_) Object(args...);
    }

  private:
    using Storage = std::aligned_storage<sizeof(Object)>;
    bool has_value_;
    Storage storage_;
};

}  // namespace learn