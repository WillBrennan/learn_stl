#include "learn_stl/array.h"

#include <gmock/gmock.h>
#include <gtest/gtest.h>

TEST(Array, construction) {
    using Array = learn::array<double, 4>;

    Array array = {0.0, 1.0, 2.0, 3.4};
}

TEST(Array, iterators) {}
TEST(Array, constIterators) {}

TEST(Array, empty) {
    learn::array<double, 4> array_a;
    ASSERT_FALSE(array_a.empty());

    learn::array<int, 2> array_b;
    ASSERT_FALSE(array_b.empty());

    learn::array<int, 0> array_c;
    ASSERT_TRUE(array_c.empty());
}

TEST(Array, size) {
    learn::array<double, 4> array_a;
    ASSERT_EQ(4, array_a.size());

    learn::array<int, 2> array_b;
    ASSERT_EQ(2, array_b.size());

    learn::array<int, 0> array_c;
    ASSERT_EQ(0, array_c.size());
}

TEST(Array, max_size) {
    learn::array<double, 4> array_a;
    ASSERT_EQ(4, array_a.size());

    learn::array<int, 2> array_b;
    ASSERT_EQ(2, array_b.size());

    learn::array<int, 0> array_c;
    ASSERT_EQ(0, array_c.size());
}

TEST(Array, fill) {}
TEST(Array, swap) {}

TEST(Array, at) {
    using Array = learn::array<double, 4>;

    Array array = {0.0, 1.0, 2.0, 3.4};
    EXPECT_NEAR(1.0, array.at(1), 1E-8);
    EXPECT_NEAR(3.4, array.at(3), 1E-8);
    EXPECT_NEAR(2.0, array.at(2), 1E-8);
    EXPECT_NEAR(0.0, array.at(0), 1E-8);

    EXPECT_THROW({ array.at(4); }, std::out_of_range);
}

TEST(Array, directAccess) {
    using Array = learn::array<double, 4>;

    Array array = {0.0, 1.0, 2.0, 3.4};
    EXPECT_NEAR(1.0, array[1], 1E-8);
    EXPECT_NEAR(3.4, array[3], 1E-8);
    EXPECT_NEAR(2.0, array[2], 1E-8);
    EXPECT_NEAR(0.0, array[0], 1E-8);

    EXPECT_NO_THROW({ array[4]; });
}

TEST(Array, front) {
    using Array = learn::array<double, 4>;

    Array array = {0.0, 1.0, 2.0, 3.4};
    EXPECT_NEAR(0.0, array.front(), 1E-8);
}

TEST(Array, back) {
    using Array = learn::array<double, 4>;

    Array array = {0.0, 1.0, 2.0, 3.4};
    EXPECT_NEAR(3.4, array.back(), 1E-8);
}

TEST(Array, data) {
    using Array = learn::array<double, 4>;

    Array array = {0.0, 1.0, 2.0, 3.4};

    ASSERT_NEAR(*array.data(), 0.0, 1E-8);
}