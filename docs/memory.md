# `memory`
This header implements functions for handling dynamic memory management. The following components have been implemented,

- `addressof` - obtains the address of the object or function arg, even in presence of overloaded `operator&`
- `default_delete` - a functor that deletes a pointer, used as the default destruction policy by `unique_ptr`.
- `unique_ptr` - a smart pointer that uniquely owns an object and deletes it on the smart pointers destruction.
- `make_unique` - a factory function to avoid using `new` and avoid possible memory leaks. 
- `allocator` - The default allocator used by the standard library for allocating / deallocating dynamic memory.

## `addressof`
`addressof` obtains the address of the object or function arg, even in presence of overloaded `operator&`. This is commonly used in templating code when you cannot rely on `&T` to produce a pointer.

### Sample
```cpp

struct MyStruct {
    T* operator&() const { return nullptr; }
};

int main() {
    int x = 0;
    auto my_struct = MyStruct{};

    auto good_address = learn::addressof(x) == &x;
    std::cout << "good address for int:" << std::boolalpha << good_address << '\n';
    // good address for int: true

    good_address = learn::addressof(my_struct) == &my_struct;
    std::cout << "good address for MyStruct:" << std::boolalpha << good_address << '\n';
    // good address for int: false
}
```
### How it works
`addressof` works by reinterpreting type `T` into a `char`; this means its not affected by any `operator&` overloads. 

```cpp
template< class T >
T* addressof(T& arg) {
    return reinterpret_cast<T*>(&const_cast<char&>(reinterpret_cast<const volatile char&>(arg)));
}
```
From C++17 onwards, `addressof` became constexpr, this requires compiler support and cannot be done by the language.

## `unique_ptr` 
`unique_ptr` is a smart pointer that uniquely owns an object and deletes it on the smart pointers destruction. 
### Sample

```cpp
int main() {
    using learn::unique_ptr;

    if (auto pointer = unique_ptr<int>(new int{30})) {
        pointer = unique_ptr<int>(new int{20});
    }
}
```

### How it works
`unique_ptr` is simple, it contains a raw pointer, and performs a reset operation on destruction, or if we overwrite the contained pointer. `unique_ptr` is templated on the contained type `T` and a `Deleter` that is used to delete the contained raw pointer. `unique_ptr` is simple internally, 

```cpp
template <class T, class Deleter = default_delete<T>>
class unique_ptr {
  public:
    using element_type = T;
    using pointer = T*;
    using deleter_type = Deleter;

    unique_ptr() noexcept = default;
    unique_ptr(pointer p) noexcept : pointer_(p) {}

    // other constructors and things...

    void reset(pointer ptr = pointer()) noexcept {
        if (pointer_) {
            deleter_type()(pointer_);
        }

        pointer_ = ptr;
    }

    ~unique_ptr() { reset(); }

    // other functions...

    explicit operator bool() const noexcept { return bool(pointer_); }

  private:
    pointer pointer_ = nullptr;
};
```
. As `unique_ptr` fufills the requirements of [aggregate initialization](https://en.cppreference.com/w/cpp/language/aggregate_initialization) operators such as `operator=` must be implemented as the compiler would fall back to aggregate initialization instead. For most instances the deleter should not be changed from `default_delete`, if extra operations do need to be performed then the destructor of the contained type `T` should be modified. 

## `make_unique`
`make_unique` is a factory function for creating `unique_ptr`s to stop memory leaks due to unspecified evaluation order of subexpressions combined, where a subexpression throws an exception.

### Sample
Here we can see an example of why `make_unique` is required, in the following code snippet we have a memory leak! 
```cpp
#include <memory>
#include <stdexcept>

struct ComponentA {};

struct ComponentB {
    ComponentB() { throw std::runtime_error("failed to construct ComponentB"); }
};

class MyClass {
  public:
    using PtrA = std::unique_ptr<ComponentA>;
    using PtrB = std::unique_ptr<ComponentB>;

    MyClass(PtrA a, PtrB b) : a_(std::move(a)), b_(std::move(b)) {}

  private:
    PtrA a_;
    PtrB b_;
};

int main() { 
    using PtrA = MyClass::PtrA;
    using PtrB = MyClass::PtrB;
    
    MyClass(PtrA(new ComponentA{}), PtrB(new ComponentB{}));
}
```
. When we attempt to construct `ComponentB` we throw an error, then `ComponentA` is leaked. This is due to the unspecified evaluation order, by constructing the `unique_ptr` and calling `::operator new` in the same scope we can avoid this. This is the problem that `make_unique` solves. To avoid the leak, we instead create `MyClass` using `make_unique`, 

```cpp
int main() {
    MyClass(make_unique<ComponentA>(), make_unique<ComponentB>());
}
```

### How it works
`make_unique` is a single-line function that uses [perfect-forwarding](https://en.cppreference.com/w/cpp/utility/forward) to construct `Value` in the same scope that `unique_ptr` is created. The unspecified evaluation order issue above has been resolved, if we fail to construct `Value` then `::operator new` won't be called, otherwise we can rely on `unique_ptr`s destructor.

```cpp
template <typename Value, class Deleter = default_delete<Value>, typename... Args>
unique_ptr<Value, Deleter> make_unique(Args&&... args) {
    return unique_ptr<Value, Deleter>(new Value(forward<Args>(args)...));
}
```

## `default_delete`
`default_delete` is a functor used by `unique_ptr` to delete the underlying memory, this means that the type `unique_ptr` is templated on must be a complete type when `unique_ptr` is destructed, unlike `shared_ptr` where it must only be a complete type at construction.

### How it works
Its simple, it a functor that deletes a pointer when `operator()` is called,

```cpp
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
```
. 

## `allocator`
`allocator` wraps up `::operator new` and `::operator delete`, it is an abstraction used by the standard library to allow custom allocators to be used with containers. `allocator` is templated on a type `T` that it allocates. 

### Sample

```cpp
int main() {
    using learn::allocator;

    allocator<int> int_allocator;
    auto* values = int_allocator.allocate(1);

    values[0] = 232;

    int_allocator.deallocate(values, 1);
}
```

### How it works
As you can see from the snippet below, `allocator` is a simple stateless struct which we can use to allocate and deallocate memory. `allocator` can be constructed from an `allocator` templated on a different type, this is done so that containers such as `map` can use `allocator<map>` to allocate other objects such as the leafs within the map, 

```cpp
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
```
.  This is why the comparison operators `operator==` and `operator!=` allow comparisons between two different specialisations of `allocator`, 

```cpp
template <class T1, class T2>
bool operator==(const allocator<T1>& lhs, const allocator<T2>& rhs) noexcept {
    return true;
}
```
. The comparison operators for `allocator` always return the same value as `allocator` is stateless, but custom allocators might contain state. An example of a statefull allocator can be found in `<memory_resource>` in C++17, where `std::pmr::polymorphic_allocator` has an underlying memory pool, they check whether two allocators share the same memory pool. 

Unfortunately, this comparison requires two instances of the allocator to exist to compare them. For stateless allocators, this removes some possible optimizations, so C++17 introduced the type defintion `is_always_equal` which we can use to check if an allocator is stateless. As `allocator` is stateless we define `using is_always_equal = std::true_type;` instead of setting it to `std::empty` in the case of a stateful allocator. 



