#pragma once

#include <cstdlib>

#include <functional>
#include <iterator>

namespace learn {
// ------------------------------------------------------------------------------------
// non-modifying algorithms

template <typename Iterator, typename UnaryPredicate>
inline bool all_of(Iterator first, Iterator last, UnaryPredicate fn) {
    for (; first != last; ++first) {
        if (!fn(*first)) {
            return false;
        }
    }

    return true;
}

template <typename Iterator, typename UnaryPredicate>
inline bool any_of(Iterator first, Iterator last, UnaryPredicate fn) {
    for (; first != last; ++first) {
        if (fn(*first)) {
            return true;
        }
    }

    return false;
}

template <typename Iterator, typename UnaryPredicate>
inline bool none_of(Iterator first, Iterator last, UnaryPredicate fn) {
    for (; first != last; ++first) {
        if (fn(*first)) {
            return false;
        }
    }

    return true;
}

template <typename Iterator, typename UnaryPredicate>
inline UnaryPredicate for_each(Iterator first, Iterator last, UnaryPredicate fn) {
    for (; first != last; ++first) {
        fn(*first);
    }

    return fn;
}

template <typename Iterator, class Size, typename UnaryPredicate>
inline UnaryPredicate for_each_n(Iterator first, Size count, UnaryPredicate fn) {
    for (Size i = 0; i < count; ++i, ++first) {
        fn(*first);
    }

    return fn;
}

template <typename Iterator, class Value>
inline typename std::iterator_traits<Iterator>::difference_type count(Iterator first, Iterator last,
                                                                      const Value& value) {
    using Index = typename std::iterator_traits<Iterator>::difference_type;
    Index num_elems = 0;

    for (; first != last; ++first) {
        if (*first == value) {
            num_elems += 1;
        }
    }

    return num_elems;
}

template <typename Iterator, typename UnaryPredicate>
inline typename std::iterator_traits<Iterator>::difference_type count_if(Iterator first,
                                                                         Iterator last,
                                                                         UnaryPredicate fn) {
    using Index = typename std::iterator_traits<Iterator>::difference_type;
    Index num_elems = 0;

    for (; first != last; ++first) {
        if (fn(*first)) {
            num_elems += 1;
        }
    }

    return num_elems;
}

template <typename IteratorA, typename IteratorB, typename BinaryPredicate>
inline std::pair<IteratorA, IteratorB> mismatch(IteratorA firstA, IteratorA lastA, IteratorB firstB,
                                                BinaryPredicate fn) {
    while (firstA != lastA && fn(*firstA, *firstB)) {
        ++firstA;
        ++firstB;
    }

    return std::make_pair(firstA, firstB);
}

template <typename IteratorA, typename IteratorB, typename BinaryPredicate>
inline std::pair<IteratorA, IteratorB> mismatch(IteratorA firstA, IteratorA lastA, IteratorB firstB,
                                                IteratorB lastB, BinaryPredicate fn) {
    while (firstA != lastA && firstB != lastB && fn(*firstA, *firstB)) {
        ++firstA;
        ++firstB;
    }

    return std::make_pair(firstA, firstB);
}

template <typename IteratorA, typename IteratorB>
inline std::pair<IteratorA, IteratorB> mismatch(IteratorA firstA, IteratorA lastA,
                                                IteratorB firstB) {
    using Value = typename std::iterator_traits<IteratorA>::value_type;
    return mismatch(firstA, lastA, firstB, std::equal_to<Value>());
}

template <typename IteratorA, typename IteratorB>
inline std::pair<IteratorA, IteratorB> mismatch(IteratorA firstA, IteratorA lastA, IteratorB firstB,
                                                IteratorB lastB) {
    using Value = typename std::iterator_traits<IteratorA>::value_type;
    return mismatch(firstA, lastA, firstB, lastB, std::equal_to<Value>());
}

template <typename IteratorA, typename IteratorB, typename BinaryPredicate>
inline bool equal(IteratorA firstA, IteratorB lastA, IteratorB firstB, BinaryPredicate fn) {
    for (; firstA != lastA; ++firstA, ++firstB) {
        if (!fn(*firstA, *firstB)) {
            return false;
        }
    }

    return true;
}

template <typename IteratorA, typename IteratorB>
inline bool equal(IteratorA firstA, IteratorB lastA, IteratorB firstB) {
    using Value = typename std::iterator_traits<IteratorA>::value_type;
    return equal(firstA, lastA, firstB, std::equal_to<Value>());
}

template <typename Iterator, class Value>
inline Iterator find(Iterator first, Iterator last, const Value& value) {
    for (; first != last; ++first) {
        if (*first == value) {
            return first;
        }
    }

    return first;
}

template <typename Iterator, class UnaryPredicate>
inline Iterator find_if(Iterator first, Iterator last, UnaryPredicate fn) {
    for (; first != last; ++first) {
        if (fn(*first)) {
            return first;
        }
    }

    return first;
}

template <typename Iterator, class UnaryPredicate>
inline Iterator find_if_not(Iterator first, Iterator last, UnaryPredicate fn) {
    return find_if(first, last, [&fn](const auto& value) { return !fn(value); });
}

template <typename FwdIteratorA, typename FwdIteratorB, class BinaryPredicate>
inline FwdIteratorA find_end(FwdIteratorA firstA, FwdIteratorA lastA, FwdIteratorB firstB,
                             FwdIteratorB lastB, BinaryPredicate fn, std::forward_iterator_tag) {
    if (firstB == lastB) {
        return lastA;
    }

    FwdIteratorA result = lastA;

    for (; firstA != lastA; ++firstA) {
        const auto new_result = search(firstA, lastA, firstB, lastB, fn);

        if (new_result == lastA) {
            return result;
        }

        result = new_result;
        firstA = result;
    }

    return result;
}

template <typename FwdIteratorA, typename FwdIteratorB>
inline FwdIteratorA find_end(FwdIteratorA firstA, FwdIteratorA lastA, FwdIteratorB firstB,
                             FwdIteratorB lastB, std::forward_iterator_tag) {
    using Value = typename std::iterator_traits<FwdIteratorA>::value_type;
    return find_end(firstA, lastA, firstB, lastB, std::equal_to<Value>());
}

template <typename IteratorA, typename FwdIteratorB, class BinaryPredicate>
inline IteratorA find_first_of(IteratorA firstA, IteratorA lastA, FwdIteratorB firstB,
                               FwdIteratorB lastB, BinaryPredicate fn) {
    for (; firstA != lastA; ++firstA) {
        FwdIteratorB iterB = firstB;
        for (; iterB != lastB; ++iterB) {
            if (fn(*iterB, *firstA)) {
                return firstA;
            }
        }
    }

    return lastA;
}

template <typename IteratorA, typename FwdIteratorB>
inline IteratorA find_first_of(IteratorA firstA, IteratorA lastA, FwdIteratorB firstB,
                               FwdIteratorB lastB) {
    using Value = typename std::iterator_traits<IteratorA>::value_type;
    return find_first_of(firstA, lastA, firstB, lastB, std::equal_to<Value>());
}

template <typename IteratorA, typename IteratorB, class BinaryPredicate>
inline IteratorA search(IteratorA firstA, IteratorA lastA, IteratorB subfirstB, IteratorB sublastB,
                        BinaryPredicate fn) {
    for (; firstA != lastA; ++firstA) {
        IteratorA it = firstA;

        if (equal(subfirstB, sublastB, it, fn)) {
            return true;
        }
    }

    return false;
}

template <typename IteratorA, typename IteratorB>
inline IteratorA search(IteratorA firstA, IteratorA lastA, IteratorB subfirstB,
                        IteratorB sublastB) {
    using Value = typename std::iterator_traits<IteratorA>::value_type;
    return search(firstA, lastA, subfirstB, sublastB, std::equal_to<Value>());
}

template <typename Iterator, class Size, class T, class BinaryPredicate>
inline Iterator search_n(Iterator first, Iterator last, Size count, const T& value,
                         BinaryPredicate fn) {
    if (count <= 0) {
        return first;
    }

    while (true) {
        if (first == last) {
            return last;
        }

        Iterator iter = first;
        Size i = 1;

        while (i < count) {
            if (fn(*iter, value)) {
                i += 1;
            } else {
                break;
            }

            ++iter;
        }

        if (i == count) {
            return first;
        }

        ++first;
    }

    return last;
}

template <typename Iterator, class Size, class T, class BinaryPredicate>
inline Iterator search_n(Iterator first, Iterator last, Size count, const T& value) {
    using Value = typename std::iterator_traits<Iterator>::value_type;
    return search_n(first, last, count, std::equal_to<Value>());
}

// ------------------------------------------------------------------------------------
// modifying

template <class T>
constexpr void swap(T& a, T& b) {
    T c = std::move(a);
    a = std::move(b);
    b = std::move(c);
}

template <typename Iterator>
inline void reverse(Iterator first, Iterator last) {
    while (first != last && first != --last) {
        swap(*first++, *last);
    }
}

template <class ForwardItA, class ForwardItB>
constexpr void iter_swap(ForwardItA a, ForwardItB b) {
    swap(*a, *b);
}

template <class Iterator>
Iterator rotate(Iterator first, Iterator new_first, Iterator last) {
    if (first == new_first) {
        return last;
    }

    if (new_first == last) {
        return first;
    }

    for (Iterator next = new_first; next != last;) {
        iter_swap(first++, next++);
        if (first == new_first) {
            new_first = next;
        }
    }

    Iterator result = first;

    for (Iterator next = new_first; next != last;) {
        iter_swap(first++, next++);
        if (first == new_first) {
            new_first = next;
        } else if (next == last) {
            next = new_first;
        }
    }

    return result;
}
}  // namespace learn