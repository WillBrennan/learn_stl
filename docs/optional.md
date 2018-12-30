# `optional`
`optional` is a vocabulary container that contains an optional value, which is commonly used as a return value of a function that may fail. This is one of the simplest containers, and is designed to replace where `unique_ptr` is used as an optional return type but remove any dynamic allocation. `optional` has many similar properties to `unique_ptr` such as implicit conversion to bool, and `operator->` and `operator*` to access the contained value.

## Sample
```cpp
struct Result {
    int sum;
    double other;
};

learn::optional<Result> myFunction(int value) {
    if (value < 10) {
        return Result{value + 10, 2.5 * value};
    }

    return {};
}

int main() {
    if (const auto maybeResult = myFunction(4)) {
        const Result& result = maybeResult.value();
        std::cout "my results: " << result.sum << ' ' << result.other << '\n';
    } else {
        std::cout "my function didnt produce a result\n";
    }
}
```

## How it works
So how can `optional` be empty and not require any dynamic allocation? This trick is performed using a union type to form an `optional_storage` struct, this struct can contain a value `T` or a placeholder `dummy_t` to represent the empty state, 

```cpp
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
```
. As `dummy_t` is an empty class; [Empty base optimization](https://en.cppreference.com/w/cpp/language/ebo) can be used, this means that `sizeof(optional_storage) == sizeof(T)`. Unfortunately, `sizeof(optional<T>) > sizeof(T)` as `optional_storage` does not tell us what value its storing, `optional` holds this information as a bool. This lets `optional` perform the same operations as `unique_ptr` without dynamic allocation, 

```cpp
template <typename Object>
class optional {
  public:
    constexpr optional() : has_value_(false) {}

  private:
    using Storage = detail::optional_storage<Object>;
    bool has_value_;
    Storage storage_;
};
```
. Finally, we can see that the `optional` has a default constructor and relies on the default constructor of `optional_storage`. As such, the order of values within `optional_storage` is important, `dummy_t` must be before `T` so that `optional_storage` default constructs `dummy_t`.