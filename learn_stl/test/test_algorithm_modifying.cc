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