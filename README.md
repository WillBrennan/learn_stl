# Learn STL
### A simple standard library; to learn how it works by implementation

Ever wondered how std::tuple is implemented? How std::get works? Well I started implementing this to find out just that.

The standard library heavily relies on many [C++ Idioms](https://en.wikibooks.org/wiki/More_C%2B%2B_Idioms) and unusual language features, by approximately implementing the library you can get a bit more familiar with them!

### Vocab Types
#### `any`
Any is the de-facto type-erasure method in C++, it provides type-safe container for single values of any type. The implementation provides an introduction to polymorphism in C++.

#### `optional`
How can you store an object without a default-constructor on the stack? Well with a `union` is how, but what is this weird special type.

#### `variant`


#### `tuple`
Tuple heavily depends on variadic templates and `std::index_sequence`, and isn't obvious how its implemented. It's a refreshing look at 
features which aren't used day-to-day.

### Containers
#### `array`
Array is deceptively simple, but how are its constructors and destructors implicity declared? Why is array constexpr but vector isn't, and what is aggregate-initialization?
Understanding Array requires a strong comprehension of these often over-looked language features.

#### `vector`
Everyone knows `std::vector` right? But why is reserving so important? And what is `std::allocator` and why wrap it in `std::allocator_traits`?

#### `valarray`
`valarray` provides an introduction to expression-templates. It stores elements in a vector, and it provides element-wise unary and binary operations. It won't create any temporaries and will only perform one iteration as it evaluates the expression for each resultant element.

### Smart Pointers
#### `unique_ptr`
`unique_ptr` is pretty simple, but its always good to understand what `std::default_deleter` does.
