#pragma once

#include <cstdlib>
#include <utility>

#include "utility.h"

namespace learn {
template <std::size_t I, typename Type>
class tuple_element;

namespace detail {
template <typename Sequences, typename... Types>
struct tuple;

template <std::size_t... Indices, typename... Types>
struct tuple<index_sequence<Indices...>, Types...> : tuple_element<Indices, Types>... {
    tuple(const Types&... elements) : tuple_element<Indices, Types>(elements)... {}
    tuple(Types&&... elements) : tuple_element<Indices, Types>(std::forward<Types>(elements))... {}
};

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

}  // namespace detail

template <typename... Types>
class tuple;

template <std::size_t I, typename Type>
class tuple_element {
  public:
    using type = Type;
    explicit tuple_element(const type& value) : value_(value) {}
    explicit tuple_element(Type&& value) : value_(std::move(value)) {}

    Type& get() { return value_; }
    const Type& get() const { return value_; }

  private:
    Type value_;
};

template <std::size_t I, typename... Types>
class tuple_element<I, tuple<Types...>> {
  public:
    using type = detail::type_at_index_t<I, Types...>;
};

template <std::size_t I, typename Tuple>
using tuple_element_t = typename tuple_element<I, Tuple>::type;

template <typename... Types>
class tuple : public detail::tuple<typename make_index_sequence<sizeof...(Types)>::type, Types...> {
  public:
    explicit tuple(const Types&... elements) : TupleImpl(elements...) {}
    explicit tuple(Types&&... elements) : TupleImpl(std::forward<Types>(elements)...) {}

  private:
    using TupleImpl = detail::tuple<typename make_index_sequence<sizeof...(Types)>::type, Types...>;
};

template <std::size_t Index, typename... Types>
auto& get(tuple<Types...>& data) noexcept {
    using Tuple = tuple<Types...>;
    using Type = tuple_element_t<Index, Tuple>;
    using Element = tuple_element<Index, Type>;
    return static_cast<Element>(data).get();
};

template <std::size_t Index, typename... Types>
const auto& get(const tuple<Types...>& data) noexcept {
    using Tuple = tuple<Types...>;
    using Type = tuple_element_t<Index, Tuple>;
    using Element = tuple_element<Index, Type>;
    return static_cast<const Element>(data).get();
};

}  // namespace learn