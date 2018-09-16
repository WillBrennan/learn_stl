#pragma once

#include <type_traits>
#include <utility>

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
    return unique_ptr<Value, Deleter>(new Value(std::forward<Args>(args)...));
}

}  // namespace learn