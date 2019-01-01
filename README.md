# Learn STL
### Learning about the C++ Standard Library by Implementation
Ever wondered how std::tuple is implemented? How std::get works? Well I started implementing this to find out just that.

The standard library heavily relies on many [C++ Idioms](https://en.wikibooks.org/wiki/More_C%2B%2B_Idioms) and unusual language features, by implementing the library you can get a bit more familiar with them!

Each of the components have documentation in *[/docs](https://github.com/WillBrennan/learn_stl/tree/master/docs)* explaining how they work, what idioms the use, and whats interesting about them. 

Hopefully you'll find this an interesting read!

Feel free to submit a PR adding more components or improving library / documentation!

### Vocab Types
#### [`any`](https://github.com/WillBrennan/learn_stl/blob/master/docs/any.md)
Any is the de-facto type-erasure method in C++, it provides type-safe container for single values of any type. The implementation provides an introduction to polymorphism in C++.

#### [`optional`](https://github.com/WillBrennan/learn_stl/blob/master/docs/optional.md)
How can you store an object without a default-constructor on the stack? Well with a `union` is how, but what is this weird special type.

#### [`tuple`](https://github.com/WillBrennan/learn_stl/blob/master/docs/tuple.md)
Tuple heavily depends on variadic templates and `std::index_sequence`, and isn't obvious how its implemented. It's a refreshing look at 
features which aren't used day-to-day.

#### [`variant`](https://github.com/WillBrennan/learn_stl/blob/master/docs/variant.md)
Another component thats heavily dependent on variadic templates, it employs more template metaprogramming tricks than `tuple`. It also provides an interesting use case of `aligned_storage`.

### Containers
#### [`array`](https://github.com/WillBrennan/learn_stl/blob/master/docs/array.md)
Array is deceptively simple, but how are its constructors and destructors implicity declared? Why is array constexpr but vector isn't, and what is aggregate-initialization?
Understanding Array requires a strong comprehension of these often over-looked language features.

#### [`vector`](https://github.com/WillBrennan/learn_stl/blob/master/docs/vector.md)
Everyone knows `std::vector` right? But why is reserving so important? And what is `std::allocator` and why wrap it in `std::allocator_traits`?

#### [`valarray`](https://github.com/WillBrennan/learn_stl/blob/master/docs/valarray.md)
`valarray` provides an introduction to expression-templates. It stores elements in a vector, and it provides element-wise unary and binary operations. It won't create any temporaries and will only perform one iteration as it evaluates the expression for each resultant element.

### Memory Mangement
#### [`unique_ptr`](https://github.com/WillBrennan/learn_stl/blob/master/docs/memory.md#unique_ptr)
`unique_ptr` is pretty simple, but its always good to understand what `std::default_deleter` does and how dangerous aggregate initialisation can be

#### [`addressof`](https://github.com/WillBrennan/learn_stl/blob/master/docs/memory.md#addressof)
`addressof` might seem like a verbose way of calling `&T`; but why does it exist? And why is it constexpr in C++17 and what does a constexpr pointer mean? 

#### [`allocator`](https://github.com/WillBrennan/learn_stl/blob/master/docs/memory.md#allocator)
`allocator` is relatively simple, but why does it define `is_always_equal` and `propagate_on_container_move_assignment`.

