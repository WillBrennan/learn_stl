#pragma once

namespace learn {

template <class T>
constexpr T* addressof(T& v) {
    return reinterpret_cast<T*>(&const_cast<char&>(reinterpret_cast<const volatile char&>(v)));
}

}  // namespace learn