# `variant`
`variant` is a type-safe union of types, similar to `union` but where the stored type is recorded and any bad access results in an exception being thrown. `variant` relies on similar template meta-programming techniques to `tuple`, which we will look at these in detail.
## Sample
```cpp
int main() {
    using learn::variant;
    using learn::get;

    variant<double, int, char> value = 'r';
    std::cout << "contained value index: " << value.index() << '\n';
    // contained value index: 2

    value = 3.323232;
    std::cout << "contained value index: " << value.index() << '\n';
    // contained value index: 0

    const double& contained_value = get<double>(value);

    const int& gonna_throw = get<int>(value); // this throws as `value` does not contain an int
}
```
## How it works
### `variant`
`variant` can store one of an arbitary number of types. To avoid dynamic allocation, `variant` must be able to store the largest type,  with the largest alignment. This must be computed from the parameter pack, we do this by unpacking the parameter pack into an initializer_list by applying `sizeof`/`alignof` to the individual parameters,

```cpp
namespace learn {
// some forward declerations...
namespace detail {

template <typename... Types>
constexpr std::size_t max_sizeof() {
    return std::max({sizeof(Types)...});
}

template <typename... Types>
constexpr std::size_t max_align() {
    return std::max({alignof(Types)...});
}

// other definitions....

} // namespace detail
```
. We can use these functions to declare an instance of `aligned_storage` which we will use to store our values. This leads to the following outline of `variant`,

```cpp
template <typename... Types>
class variant {
  public:
    static_assert(0 < sizeof...(Types), "variant must consist of at least one alternative");

    // functions we are going to talk about
    template <class T>
    constexpr variant(T&& t);

    // other functions... 
    template <std::size_t I, typename... OtherTypes>
    friend const auto& get(const variant<OtherTypes...>& value);

  private:
    using size_type = std::size_t;
    using storage = aligned_storage<detail::max_sizeof<Types...>(), detail::max_align<Types...>()>;

    size_type type_id_ = 0;
    storage storage_;
};
} // namespace learn
```
. Now that we have a way of storing the values, we've got to work out how to assign a value to the storage. This requires several helper functions, 

- `holds_alternative` - checks that type `T` exists in `Types`, this is a standard library function
- `detail::index_of` - returns the index of `T` in `Types`, this is used to perform checks with `type_id_`
- `detail::variant_helper` - perform operations on the `index`th type in `Types` such as construction, destruction, and moving

, these are the core functions used by `variant` internally. We can see how `variant` can be constructed from one of its alternative types, `T`,

```cpp
template <typename... Types>
template <class T>
constexpr variant<Types...>::variant(T&& t) {
    static_assert(holds_alternative<T, Types...>, "varient does not contain type T");

    type_id_ = detail::index_of<T, Types...>();

    const auto index = sizeof...(Types) - 1 - type_id_;
    detail::variant_helper<Types...>::move(index, &t, &storage_);
}
```
.

### `detail::variant_helper`
`detail::variant_helper` is the most complex part of `variant`, it works by recursively traversing `Types...` and deincrementing the index its called with until the index is zero and it performs the operation. In the snippet below we can see how we recursively `move` a value, this member function requires the definition to be invariant of the type its operating on, to do this it exploits `void*`.

```cpp
template <typename... Types>
struct variant_helper;

template <typename T, typename... Types>
struct variant_helper<T, Types...> {
    static void destroy(std::size_t id, void* data) {
        if (id == sizeof...(Types)) {
            reinterpret_cast<T*>(data)->~T();
        } else {
            variant_helper<Types...>::destroy(id, data);
        }
    }

    static void move(std::size_t old_t, void* old_v, void* new_v) {
        if (old_t == sizeof...(Types)) {
            new (new_v) T(std::move(*reinterpret_cast<T*>(old_v)));
        } else {
            variant_helper<Types...>::move(old_t, old_v, new_v);
        }
    }

    static void copy(std::size_t old_t, const void* old_v, void* new_v) {
        if (old_t == sizeof...(Types)) {
            new (new_v) T(*reinterpret_cast<T*>(old_v));
        } else {
            variant_helper<Types...>::move(old_t, old_v, new_v);
        }
    }
};

template <>
struct variant_helper<> {
    static void destroy(size_t id, void* data) {}
    static void move(size_t old_t, void* old_v, void* new_v) {}
    static void copy(size_t old_t, const void* old_v, void* new_v) {}
};
```

### `detail::index_of`
This function returns the index of type `T` in types `Types...`. It does this by performing the `initializer_list` trick with `std::is_same_v`, this lets us search for the type. To make the function constexpr we implement our own `find` function, in C++20 onwards, `find` and `find_if` have been made constexpr.

```cpp
template <class Iter, class Value>
constexpr Iter find(Iter iter, Iter end, Value value) {
    for (; iter != end; ++iter) {
        if (*iter == value) {
            return iter;
        }
    }

    return end;
}

template <typename T, typename... Types>
constexpr std::size_t index_of() {
    const auto values = {std::is_same_v<T, Types>...};
    const auto iter = find(values.begin(), values.end(), true);

    return (iter != values.end()) ? ::std::distance(values.begin(), iter) : variant_npos;
}
```

### `holds_alternative`
`holds_alternative` is simple to implement, its done with `is_same` and `disjunction`, this lets us check if any of the `is_same` values are true, 
```cpp
template <class T, class... Types>
constexpr bool holds_alternative(const variant<Types...>& v) noexcept {
    return std::disjunction<std::is_same<T, Types>...>::value;
}
```
. In C++20 onwards, this may also be implemented by using the `initializer_list` trick with `any_of` to check if any of the values are true as `any_of` has been made constexpr.

### `get`
`get` works by performing a `reinterpret_cast` on the `aligned_storage` within variant. `get` needs to check that the `variant` contains the correct value, and in the case of the index variant of `get`, it needs to work out the type to cast to. We can see how the index variant of `get` is implemented,

```cpp
template <std::size_t I, typename... OtherTypes>
const auto& get(const variant<OtherTypes...>& value) {
    static_assert(I < sizeof...(OtherTypes), "index exceeds number of stored types");
    using T = typename detail::type_at_index<I, OtherTypes...>::type;
    if (I != value.index()) {
        throw bad_variant_access{};
    }

    return *reinterpret_cast<const T*>(&value.storage_);
}
```
. We work out the type to cast to using `detail::type_at_index` which uses the [type-selection idiom](https://en.wikibooks.org/wiki/More_C%2B%2B_Idioms/Type_Selection). In this implementation we keep the index variant of `get` as a friend function, then the value variant of `get` calls the index variant, 

```cpp
template <class T, typename... OtherTypes>
const auto& get(const variant<OtherTypes...>& value) {
    constexpr auto index = detail::index_of<T, OtherTypes...>();
    static_assert(index != variant_npos, "type T not in variant");

    if (index != value.index()) {
        throw bad_variant_access{};
    }

    return get<index>(value);
}
```
. To determine the index of the type `T` we use `detail::index_of` as was seen earlier. Static asserts are used in both to provide more meaningful error messages.

### `detail::type_at_index`
`type_at_index` uses the [type-selection idiom](https://en.wikibooks.org/wiki/More_C%2B%2B_Idioms/Type_Selection) to select a type at an index. It is also used within `tuple`. `type_at_index` is a struct templated on an index, and a varaidic template. It provides a recursive type definition, that recursively calls and deincrements the index until it hits a template specialisation for `0`. This is a common technique used for handling parameter packs.

```cpp
namespace detail {
template <size_t I, typename Head, typename... Tail>
struct type_at_index {
    using type = typename type_at_index<I - 1, Tail...>::type;
};

template <typename Head, typename... Tail>
struct type_at_index<0, Head, Tail...> {
    using type = Head;
};

template <size_t I, typename... Types>
using type_at_index_t = typename type_at_index<I, Types...>::type;
} // namespace detail

int main() {
    using Type0 = type_at_index_t<0, double, char, int>; // double
    using Type1 = type_at_index_t<1, double, char, int>; // char
    using Type2 = type_at_index_t<2, double, char, int>; // int
}
```

