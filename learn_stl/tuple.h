#pragma once

#include <cstdlib>

#include "utility.h"

namespace learn {
namespace detail {
template <typename Sequences, typename... Types>
struct tuple;

template <std::size_t I, typename Type>
struct tuple_leaf {
  public:
    using type = Type;
    explicit constexpr tuple_leaf(const type& value) : value_(value) {}
    explicit constexpr tuple_leaf(Type&& value) : value_(move(value)) {}

    Type value_;
};

template <std::size_t... Indices, typename... Types>
struct tuple<index_sequence<Indices...>, Types...> : tuple_leaf<Indices, Types>... {
    explicit constexpr tuple(const Types&... elements) : tuple_leaf<Indices, Types>(elements)... {}
    explicit constexpr tuple(Types&&... elements)
        : tuple_leaf<Indices, Types>(std::forward<Types>(elements))... {}
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
};

template <std::size_t I, typename... Types>
class tuple_element<I, tuple<Types...>> {
  public:
    using type = detail::type_at_index_t<I, Types...>;
};

template <std::size_t I, typename Tuple>
using tuple_element_t = typename tuple_element<I, Tuple>::type;

template <class Types>
class tuple_size;

template <typename... Types>
class tuple_size<tuple<Types...>> : public std::integral_constant<std::size_t, sizeof...(Types)> {};

template <typename... Types>
class tuple : public detail::tuple<typename make_index_sequence<sizeof...(Types)>::type, Types...> {
  public:
    explicit constexpr tuple(const Types&... elements) : TupleImpl(elements...) {}
    explicit constexpr tuple(Types&&... elements) : TupleImpl(std::forward<Types>(elements)...) {}

  private:
    using TupleImpl = detail::tuple<typename make_index_sequence<sizeof...(Types)>::type, Types...>;
};

template <std::size_t Index, typename... Types>
constexpr auto& get(tuple<Types...>& data) noexcept {
    using Tuple = tuple<Types...>;
    using Type = tuple_element_t<Index, Tuple>;
    using Element = detail::tuple_leaf<Index, Type>;

    Element& base = data;
    return base.value_;
};

template <std::size_t Index, typename... Types>
constexpr const auto& get(const tuple<Types...>& data) noexcept {
    using Tuple = tuple<Types...>;
    using Type = tuple_element_t<Index, Tuple>;
    using Element = detail::tuple_leaf<Index, Type>;

    const Element& base = data;
    return base.value_;
};

}  // namespace learn