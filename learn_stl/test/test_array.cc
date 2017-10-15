#include "learn_stl/array.h"

#include <gmock/gmock.h>
#include <gtest/gtest.h>

TEST(Array, construction) {
    using Array = learn::array<double, 4>;

    Array array = {0.0, 1.0, 2.0, 3.4};
}

TEST(Array, iterators) {}
TEST(Array, constIterators) {}

TEST(Array, empty) {}
TEST(Array, size) {}
TEST(Array, max_size) {}