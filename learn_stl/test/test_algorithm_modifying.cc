#include "learn_stl/algorithm.h"

#include <gmock/gmock.h>
#include <gtest/gtest.h>

TEST(Reverse, Empty) {
    using learn::reverse;

    int numbers[] = {};

    ASSERT_NO_THROW({ reverse(numbers, numbers); });
}

TEST(Reverse, EvenSize) {
    using learn::reverse;

    int numbers[] = {2, 4, 3, 1};

    ASSERT_NO_THROW({ reverse(std::begin(numbers), std::end(numbers)); });
    EXPECT_THAT(numbers, testing::ElementsAre(1, 3, 4, 2));
}

TEST(Reverse, OddSize) {
    using learn::reverse;

    int numbers[] = {-1, 3, 2, -2, 4};

    ASSERT_NO_THROW({ reverse(std::begin(numbers), std::end(numbers)); });
    EXPECT_THAT(numbers, testing::ElementsAre(4, -2, 2, 3, -1));
}

TEST(Swap, Double) {
    using Value = double;
    using learn::swap;

    auto value_a = Value{1.0};
    auto value_b = Value{1.5};

    swap(value_a, value_b);

    ASSERT_EQ(value_a, Value{1.5});
    ASSERT_EQ(value_b, Value{1.0});
}

TEST(Swap, Float) {
    using Value = float;
    using learn::swap;

    auto value_a = Value{1.0};
    auto value_b = Value{1.5};

    swap(value_a, value_b);

    ASSERT_EQ(value_a, Value{1.5});
    ASSERT_EQ(value_b, Value{1.0});
}

TEST(Swap, Int) {
    using Value = int;
    using learn::swap;

    auto value_a = Value{1};
    auto value_b = Value{2};

    swap(value_a, value_b);

    ASSERT_EQ(value_a, Value{2});
    ASSERT_EQ(value_b, Value{1});
}

TEST(Swap, UnsignedChar) {
    using Value = unsigned char;
    using learn::swap;

    auto value_a = Value{'a'};
    auto value_b = Value{'b'};

    swap(value_a, value_b);

    ASSERT_EQ(value_a, Value{'b'});
    ASSERT_EQ(value_b, Value{'a'});
}

TEST(IterSwap, DoubleArray) {
    using Value = double;
    using learn::iter_swap;

    Value data[5] = {Value{1.0}, Value{2.0}, Value{3.0}, Value{4.0}, Value{5.0}};

    iter_swap(data + 1, data + 3);

    ASSERT_THAT(data, testing::ElementsAre(1.0, 4.0, 3.0, 2.0, 5.0));
}

TEST(IterSwap, FloatArray) {
    using Value = float;
    using learn::iter_swap;

    Value data[5] = {1.0f, 2.0f, 3.0f, 4.0f, 5.0f};

    iter_swap(data + 1, data + 3);

    ASSERT_THAT(data, testing::ElementsAre(1.0f, 4.0f, 3.0f, 2.0f, 5.0f));
}

TEST(IterSwap, IntArray) {
    using Value = int;
    using learn::iter_swap;

    Value data[5] = {1, 2, 3, 4, 5};

    iter_swap(data + 1, data + 3);

    ASSERT_THAT(data, testing::ElementsAre(1, 4, 3, 2, 5));
}

TEST(IterSwap, UnsignedCharArray) {
    using Value = double;
    using learn::iter_swap;

    Value data[5] = {'a', 'b', 'c', 'd', 'e'};

    iter_swap(data + 1, data + 3);

    ASSERT_THAT(data, testing::ElementsAre('a', 'd', 'c', 'b', 'e'));
}

TEST(Rotate, DoubleArray) {
    using Value = double;
    using learn::rotate;

    Value data[5] = {'a', 'b', 'c', 'd', 'e'};

    const auto new_end = rotate(data, data + 2, data + 5);

    Value expected_data[5] = {'c', 'd', 'e', 'a', 'b'};
    ASSERT_THAT(data, testing::ElementsAreArray(expected_data));
}