#pragma once

#include <cstdlib>

namespace learn {

template <std::size_t... Indices>
struct index_sequence;

namespace detail {
template <std::size_t I, typename Sequence>
struct cat_index_sequence;

template <std::size_t I, size_t... Indices>
struct cat_index_sequence<I, index_sequence<Indices...> > : index_sequence<Indices..., I> {};

}  // namespace detail

template <std::size_t... Indices>
struct index_sequence {
    using type = index_sequence<Indices...>;
};

template <std::size_t N>
struct make_index_sequence
    : detail::cat_index_sequence<N - 1, typename make_index_sequence<N - 1>::type>::type {};

template <>
struct make_index_sequence<1> : index_sequence<0> {};

template <class Value>
struct remove_reference {
    using type = Value;
};

template <class Value>
struct remove_reference<Value&> {
    using type = Value;
};

template <class Value>
struct remove_reference<Value&&> {
    using type = Value;
};

template <class Value>
using remove_reference_t = typename remove_reference<Value>::type;

template <class Value>
constexpr typename remove_reference<Value>::type&& move(Value&& value) noexcept {
    return static_cast<typename remove_reference<Value>::type&&>(value);
}

template <class Value>
constexpr Value&& forward(typename remove_reference<Value>::type& value) noexcept {
    return static_cast<Value&&>(value);
}

template <class Value>
constexpr Value&& forward(typename remove_reference<Value>::type&& value) noexcept {
    return static_cast<Value&&>(value);
}

}  // namespace learn