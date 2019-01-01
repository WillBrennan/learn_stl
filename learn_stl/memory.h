#pragma once

#include <new>
#include <type_traits>

#include "utility.h"

namespace learn {

template <class T>
constexpr T* addressof(T& v) {
    return reinterpret_cast<T*>(&const_cast<char&>(reinterpret_cast<const volatile char&>(v)));
}

template <typename Object>
struct default_delete {
    static_assert(!std::is_function<Object>::value,
                  "default_delete cannot be instantiated for function types");

    constexpr default_delete() noexcept = default;

    void operator()(Object* ptr) const noexcept {
        static_assert(sizeof(ptr) > 0, "default_delete can not delete incomplete type");
        static_assert(!std::is_void<Object>::value,
                      "default_delete can not delete incomplete type");
        delete ptr;
    }
};

template <class T, class Deleter = default_delete<T>>
class unique_ptr {
  public:
    using element_type = T;
    using pointer = T*;
    using deleter_type = Deleter;

    unique_ptr() noexcept = default;
    unique_ptr(pointer p) noexcept : pointer_(p) {}

    unique_ptr(unique_ptr&& other) : pointer_(other.pointer_) { other.pointer_ = nullptr; }

    unique_ptr& operator=(unique_ptr&& other) {
        reset(other.pointer_);
        other.pointer_ = nullptr;
        return *this;
    }

    unique_ptr& operator=(std::nullptr_t) noexcept { reset(nullptr); }

    void reset(pointer ptr = pointer()) noexcept {
        if (pointer_) {
            deleter_type()(pointer_);
        }

        pointer_ = ptr;
    }

    ~unique_ptr() { reset(); }

    pointer operator->() const noexcept { return pointer_; }
    typename std::add_lvalue_reference<T>::type operator*() const { return *pointer_; }

    pointer get() const noexcept { return pointer_; }

    explicit operator bool() const noexcept { return bool(pointer_); }

  private:
    pointer pointer_ = nullptr;
};

template <typename Value, class Deleter = default_delete<Value>, typename... Args>
unique_ptr<Value, Deleter> make_unique(Args&&... args) {
    return unique_ptr<Value, Deleter>(new Value(forward<Args>(args)...));
}

template <class T>
struct allocator {
    using value_type = T;
    using size_type = std::size_t;
    using difference_type = std::ptrdiff_t;
    using propagate_on_container_move_assignment = std::true_type;
    using is_always_equal = std::true_type;

    allocator() noexcept = default;
    allocator(const allocator& other) noexcept = default;

    template <class U>
    allocator(const allocator<U>& other) noexcept {};

    ~allocator() = default;

    value_type* allocate(size_type n) {
        const auto num_bytes = size_type{n * sizeof(T)};
        const auto alignment = std::align_val_t{alignof(T)};

        return static_cast<T*>(::operator new(num_bytes, alignment));
    }

    void deallocate(T* p, std::size_t n) {
        const auto num_bytes = size_type{n * sizeof(T)};
        const auto alignment = std::align_val_t{alignof(T)};

        ::operator delete(p, alignment);
    }

    size_type max_size() const noexcept { return size_type(~0) / sizeof(T); }
};

template <class T1, class T2>
bool operator==(const allocator<T1>& lhs, const allocator<T2>& rhs) noexcept {
    return true;
}

template <class T1, class T2>
bool operator!=(const allocator<T1>& lhs, const allocator<T2>& rhs) noexcept {
    return false;
}

}  // namespace learn