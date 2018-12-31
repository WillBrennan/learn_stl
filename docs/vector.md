# `vector`
`vector` is the best known standard library container, but how does the container handle a dynamic number of elements and why does it use `allocator_traits`. `vector` is templated on the contained type `T` and an allocator `allocator<T>`. 

## Sample

```cpp
int main() {
    learn::vector<int> values = {0, 5, 7, 8, 8, 9, 9, 7, 6};
    
    std::cout << "num values: " << values.size() << '\n';

    for (const auto& value: values) {
        std::cout << value << ' ';
    }

    std::cout << '\n';
}
```

## How it works
`vector` consists of a dynamically allocated array, its allocated using the allocator, `AllocatorT`, that the vector is templated with, by default this is `learn::allocator<ValueT>>`. The vector stores its size, capactity, and a pointer to the data, in this implementation, we do this by storing `begin_` as a pointer and `size_` and `capacity_` as size types though some implementations store these all as pointers as it makes `vector::end()` cheaper. We can see the basic layout is as follows, 

```cpp
namespace learn {
template <typename ValueT, class AllocatorT = std::allocator<ValueT>>
class vector {
  public:
    using value_type = ValueT;
    using allocator = AllocatorT;
    using size_type = std::size_t;
    // other type definitions...
    
    // some constructors... 

    // some other element-wise access operators...

    reference operator[](size_type index) { return *(begin() + index); }
    const_reference operator[](size_type index) const { return *(begin() + index); }

    // some other iterators... 

    iterator begin() { return begin_; }
    iterator end() { return begin() + size(); }

    // capacity

    size_type size() const { return size_; }
    size_type capacity() const { return capacity_; }

    void reserve(size_type new_capacity);

    // modifiers

    template <class... Args>
    reference emplace_back(Args&&... args);

  private:
    using AllocatorTraits = std::allocator_traits<allocator>;
    allocator allocator_;
    pointer begin_ = nullptr;

    size_type size_ = 0;
    size_type capacity_ = 0;

    size_type recommend(const size_type& size) const;
};
```
. The critical part of `vector` lies in `void vector::reserve(size_type new_capacity);` and how the vector allocates internally. It does this by using `allocator_` with `AllocatorTraits`. `AllocatorTraits` is used to maintain compatability between C++98 and C++0x onwards, because in C++0x [N2946](http://www.open-std.org/Jtc1/sc22/wg21/docs/papers/2009/n2946.pdf) was accepted and the allocator interface was extended, `AllocatorTraits` provides these extensions if they are not implemented. The implementation can use if-constexpr in C++17 onwards to handle trivially constructable values. 

```cpp
template <typename Value, class Allocator>
void vector<Value, Allocator>::reserve(size_type new_capacity) {
    if (new_capacity <= capacity_) {
        return;
    }

    const auto new_begin = AllocatorTraits::allocate(allocator_, new_capacity);
    if (begin_) {
        if
            constexpr(std::is_trivially_constructible<Value>::value) {
                std::memmove(new_begin, begin_, size_ * sizeof(value_type));
            }
        else {
            move(begin_, begin_ + size_, new_begin);
        }

        AllocatorTraits::deallocate(allocator_, begin_, capacity_);
    }

    begin_ = new_begin;
    capacity_ = new_capacity;
};
```
. To `emplace_back` elements onto the vector, we again use `AllocatorTraits`, it lets us construct an element in place.

```cpp
template <class... Args>
reference emplace_back(Args&&... args) {
    if (size_ == capacity_) {
        reserve(recommend(size_));
    }

    AllocatorTraits::construct(allocator_, begin_ + size_, forward<Args>(args)...);
    size_ += 1;

    return back();
}
```