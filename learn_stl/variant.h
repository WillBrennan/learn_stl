#pragma once

#include <cstdint>

#include <algorithm>
#include <type_traits>

#include "algorithm.h"
#include "tuple.h"
#include "type_traits.h"

namespace learn {

template <typename... Types>
class variant;

inline constexpr std::size_t variant_npos = -1;

namespace detail {
template <typename... Types>
constexpr std::size_t max_sizeof() {
    return std::max({sizeof(Types)...});
}

template <typename... Types>
constexpr std::size_t max_align() {
    return std::max({alignof(Types)...});
}

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
}  // namespace detail

template <class T, class... Types>
constexpr bool holds_alternative(const variant<Types...>& v) noexcept {
    return std::disjunction<std::is_same<T, Types>...>::value;
}

struct monostate {};

class bad_variant_access : public std::exception {};

template <typename... Types>
class variant {
  public:
    static_assert(0 < sizeof...(Types), "variant must consist of at least one alternative");

    constexpr variant();

    template <class T>
    constexpr variant(T&& t);

    template <class T>
    variant& operator=(T&& t);

    ~variant();

    constexpr std::size_t index() const noexcept { return type_id_; }

    template <std::size_t I, typename... OtherTypes>
    friend const auto& get(const variant<OtherTypes...>& value) {
        static_assert(I < sizeof...(OtherTypes), "index exceeds number of stored types");
        using T = typename detail::type_at_index<I, OtherTypes...>::type;
        if (I != value.index()) {
            throw bad_variant_access{};
        }

        return *reinterpret_cast<const T*>(&value.storage_);
    }

    template <std::size_t I, typename... OtherTypes>
    friend auto& get(variant<OtherTypes...>& value) {
        static_assert(I < sizeof...(OtherTypes), "index exceeds number of stored types");
        using T = typename detail::type_at_index<I, OtherTypes...>::type;
        if (I != value.index()) {
            throw bad_variant_access{};
        }

        return *reinterpret_cast<T*>(&value.storage_);
    }

  private:
    using size_type = std::size_t;
    using storage = aligned_storage<detail::max_sizeof<Types...>(), detail::max_align<Types...>()>;

    size_type type_id_ = 0;
    storage storage_;
};

template <typename... Types>
constexpr variant<Types...>::variant() {
    using T = typename detail::type_at_index<0, Types...>::type;
    static_assert(std::is_default_constructible_v<T>, "default type is not default constructable!");

    T t;
    detail::variant_helper<Types...>::move(0, &t, &storage_);
}

template <typename... Types>
template <class T>
constexpr variant<Types...>::variant(T&& t) {
    static_assert(holds_alternative<T, Types...>, "varient does not contain type T");

    type_id_ = detail::index_of<T, Types...>();

    const auto index = sizeof...(Types) - 1 - type_id_;
    detail::variant_helper<Types...>::move(index, &t, &storage_);
}

template <typename... Types>
template <class T>
variant<Types...>& variant<Types...>::operator=(T&& t) {
    static_assert(holds_alternative<T, Types...>, "varient does not contain type T");

    auto index = sizeof...(Types) - 1 - type_id_;
    detail::variant_helper<Types...>::destroy(index, &storage_);

    type_id_ = detail::index_of<T, Types...>();

    index = sizeof...(Types) - 1 - type_id_;
    detail::variant_helper<Types...>::move(index, &t, &storage_);

    return *this;
}

template <typename... Types>
variant<Types...>::~variant() {
    detail::variant_helper<Types...>::destroy(type_id_, &storage_);
}

template <class T, typename... OtherTypes>
const auto& get(const variant<OtherTypes...>& value) {
    constexpr auto index = detail::index_of<T, OtherTypes...>();
    static_assert(index != variant_npos, "type T not in variant");

    if (index != value.index()) {
        throw bad_variant_access{};
    }

    return get<index>(value);
}

template <class T, typename... OtherTypes>
auto& get(variant<OtherTypes...>& value) {
    constexpr auto index = detail::index_of<T, OtherTypes...>();
    static_assert(index != variant_npos, "type T not in variant");

    if (index != value.index()) {
        throw bad_variant_access{};
    }

    return get<index>(value);
}

}  // namespace learn