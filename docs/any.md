# `any`
`any` is a type-safe container for values of any type, where the contained value can be accessed using `any_cast`. 
Several variants of `any_cast` exist, allowing it to be accessed in different ways. 

## Sample
```cpp
learn::any any_value = 12;

// Cast 1- return copy of internal value
const int my_value0 = learn::any_cast<int>(any_value);

// Cast 1 - throws a std::bad_any_cast as type is bad
const double my_value1 = learn::any_cast<double>(any_value);

// Cast 2 - returns a nullptr if the type is bad! 
const int* my_ptr = learn::any_cast<int>(&any_value);

// Cast 3 - moves the value out of the any container
const int my_value2 = learn::any_cast<int>(learn::move(any_value));
```

## How it works
But how does `any` do this? Internally, `any` has a `ContainerInterface` and a templated `Container`, which is templated on the stored value type. 

```cpp
struct ContainerInterface {
    using Ptr = ErasedPtr<ContainerInterface>;

    virtual ~ContainerInterface() = default;
    virtual const std::type_info& type() const = 0;

    virtual Ptr copy() const = 0;
};

template <typename ObjectT>
struct Container : ContainerInterface {
    using Object = std::decay_t<ObjectT>;

    template <typename... Args>
    static Ptr make(Args&&... args) {
        return Ptr(new Container(forward<Args>(args)...));
    }

    explicit Container(const Object& _data) : data(_data) {}
    explicit Container(Object&& _data) : data(forward<Object>(_data)) {}
    Object data;

    const std::type_info& type() const final { return typeid(data); }

    Ptr copy() const final { return make(data); }
};
```
We store a unique ptr to a `Container` through `ContainerInterface`, allowing the `ContainerInterface` pointer to store a `Container` for any type. To access the stored value, we perform a `dynamic_cast` to cast down the inheritance hierarchy. If the cast fails, then it returns a nullptr. Below you can see how Cast 2 is done.

```cpp
template <typename Object>
const Object* any_cast(const any* value) {
    auto container_ptr = dynamic_cast<const any::Container<Object>*>(value->container_.get());

    return container_ptr ? addressof(container_ptr->data) : nullptr;
}
```

This function can be used to implement the other `any_cast` functions.