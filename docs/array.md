# `array`
`array` is a container encapsulating fixed sized arrays and do not decay to T* unlike C arrays. It provides iterators, element-wise access, memeber types, everything you would expect from a standard library container. Its templated on the type, `typename T`, and the size, `std::size_t N`. But it has a few oddities; the constructor and assignment operator are implicitly declared, and how are `begin()` and `end()` defined when the size is zero?

## Sample
To construct an `array` we use [aggregate-initialisation](https://en.cppreference.com/w/cpp/language/aggregate_initialization), but this leads to a few potential problems. We can initialise `values` with less than three elements. In this scenario, the unspecified elements are [value-initialised](https://en.cppreference.com/w/cpp/language/value_initialization), which in the case of `double` falls back to [zero-initialisation](https://en.cppreference.com/w/cpp/language/zero_initialization). 

```cpp
template <typename Value, std::size_t N>
void print(const learn::array<Value, N>& values) {
    for (const auto& value: values) {
        std::cout << value << ' ';
    }

    std::cout << '\n';
}

learn::array<int, 3> values = {0, -2, 3};
print(values); // 0 -2 3

values = {3, 1};
print(values) // 3 1 0
```
This means that when we construct an `array` with no values, [value-initialisation](https://en.cppreference.com/w/cpp/language/value_initialization) is performed on all elements.

```cpp 
learn::array<double, 3> values2;

print(values) // 0.0 0.0 0.0
```

## How it works
`array` is a struct, containing a C-style array. We can perform aggregate-initialisation as `array` has, 

- no private or protected non-static data members
- no user-provided, inherited, or explicit constructors (explicitly defaulted or deleted constructors are allowed)
- no virtual, private, or protected (since C++17) base classes
- no virtual member functions
 
. We define internal C-style array so that it contains one element when the size is zero. This lets `array` have a size of zero and have defined behaviour. This is in contrast to ISO C / C++ which does not allow arrays of size zero. 

```cpp
template <typename Value, std::size_t Size>
struct array {
    using value_type = Value;
    
    // some type declerations... 
    // some member function declerations...

    value_type elems_[Size > 0 ? Size : 1];
};
```

By defining the size of `elems_` this way we can define the begin and end iterators, which are valid even when `Size == 0`.

```cpp
    // some member function declerations...

    constexpr iterator begin() noexcept { return iterator(elems_); }

    constexpr iterator end() { return iterator(elems_ + Size); }
    value_type elems_[Size > 0 ? Size : 1];
};
```