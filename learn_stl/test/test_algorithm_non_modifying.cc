#include "learn_stl/algorithm.h"

#include <initializer_list>

#include <gmock/gmock.h>
#include <gtest/gtest.h>

TEST(AllOf, IsEven) {
    using learn::all_of;

    const auto numbers = {0, 2, 4, 6, 7, 8, 9};

    const auto fn_is_even = [](const int& value) { return value % 2 == 0; };

    ASSERT_TRUE(all_of(numbers.begin(), numbers.begin() + 3, fn_is_even));
    ASSERT_FALSE(all_of(numbers.begin(), numbers.begin() + 6, fn_is_even));
}

TEST(AllOf, IsOdd) {
    using learn::all_of;

    const auto numbers = {0, 2, 4, 6, 7, 8, 9};

    const auto fn_is_odd = [](const int& value) { return value % 2 != 0; };

    ASSERT_FALSE(all_of(numbers.begin(), numbers.begin() + 3, fn_is_odd));
    ASSERT_TRUE(all_of(numbers.begin(), numbers.begin(), fn_is_odd));
}

TEST(AnyOf, AllEven) {
    using learn::any_of;

    const auto numbers = {0, 2, 4, 6, 8};
    const auto fn_is_even = [](const int& value) { return value % 2 == 0; };

    ASSERT_TRUE(any_of(numbers.begin(), numbers.end(), fn_is_even));
}

TEST(AnyOf, OneOdd) {
    using learn::any_of;

    const auto numbers = {0, 2, 4, 6, 7, 8};
    const auto fn_is_odd = [](const int& value) { return value % 2 != 0; };

    ASSERT_TRUE(any_of(numbers.begin(), numbers.end(), fn_is_odd));
}

TEST(AnyOf, AllOdd) {
    using learn::any_of;

    const auto numbers = {1, 3, 5, 7, 9};
    const auto fn_is_even = [](const int& value) { return value % 2 == 0; };

    ASSERT_FALSE(any_of(numbers.begin(), numbers.end(), fn_is_even));
}

TEST(NoneOff, AllEven) {
    using learn::none_of;

    const auto numbers = {0, 2, 4, 6, 8};
    const auto fn_is_odd = [](const int& value) { return value % 2 != 0; };

    ASSERT_TRUE(none_of(numbers.begin(), numbers.end(), fn_is_odd));
}

TEST(NoneOff, OneEven) {
    using learn::none_of;

    const auto numbers = {0, 2, 4, 5, 6, 8};
    const auto fn_is_odd = [](const int& value) { return value % 2 != 0; };

    ASSERT_FALSE(none_of(numbers.begin(), numbers.end(), fn_is_odd));
}

TEST(ForEach, DoubleValues) {
    using learn::for_each;

    const auto fn_double = [](int& value) { value *= 2; };

    int numbers[] = {-1, 0, 3, 5, 4, 3};
    for_each(numbers, numbers + 6, fn_double);

    EXPECT_THAT(numbers, testing::ElementsAre(-2, 0, 6, 10, 8, 6));
}

TEST(ForEachN, DoubleValues) {
    using learn::for_each_n;

    const auto fn_double = [](int& value) { value *= 2; };

    int numbers[] = {-1, 0, 3, 5, 4, 3};
    for_each_n(numbers, 4, fn_double);

    EXPECT_THAT(numbers, testing::ElementsAre(-2, 0, 6, 10, 4, 3));
}

TEST(Count, Integers) {
    using learn::count;

    const auto numbers = {0, 1, 2, 2, 1, 0, 2, 0, 3, 0, -1};
    const auto num_values = count(numbers.begin(), numbers.end(), 2);

    EXPECT_EQ(3, num_values);
}

TEST(CountIf, Integers) {
    using learn::count_if;

    const auto numbers = {0, 1, 2, 2, 1, 0, 2, 0, 3, 0, -1};

    const auto fn_is_odd = [](const int& number) { return number % 2 != 0; };
    const auto num_odd = count_if(numbers.begin(), numbers.end(), fn_is_odd);

    EXPECT_EQ(num_odd, 4);
}

TEST(Mismatch, SameSizeEqual) {
    using learn::mismatch;

    const auto numbers = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10};
    const auto subset = {5, 6, 7, 8};

    /*
    const auto iter = mismatch(numbers.begin(), numbers.end(), subset.begin(), subset.end());

    EXPECT_EQ(iter, numbers.begin() + 6);
    */
}

TEST(Mismatch, SameSizePredicate) {}
TEST(Mismatch, DifferentSizesEqual) {}
TEST(Mismatch, DifferentSizesPredicate) {}

TEST(Equal, SameSizeEqual) {}
TEST(Equal, SameSizePredicate) {}
TEST(Equal, DifferentSizeEqual) {}
TEST(Equal, DifferentSizePredicate) {}

TEST(Find, SameSize) {}
TEST(Find, DifferentSize) {}
TEST(FindIf, SameSize) {}
TEST(FindIf, DifferentSize) {}
TEST(FindIfNot, SameSize) {}
TEST(FindIfNot, DifferentSize) {}

TEST(FindEnd, Equal) {}
TEST(FindEnd, Predicate) {}

TEST(FindFirstOf, Equal) {}
TEST(FindFirstOf, Predicate) {}
TEST(AdjacentFind, Equal) {}
TEST(AdjacentFind, Predicate) {}
TEST(Search, Equal) {}
TEST(Search, Predicate) {}
TEST(SearchN, Equal) {}
TEST(SearchN, Predicate) {}
