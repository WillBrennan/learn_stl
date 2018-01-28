#include "learn_stl/vector.h"

#include <gmock/gmock.h>
#include <gtest/gtest.h>

TEST(Vector, DefaultConstruct) {
    using Vector = learn::vector<double>;
    Vector vector;
}

TEST(Vector, Size) {
    using Vector = learn::vector<double>;
    Vector vector;

    ASSERT_EQ(vector.size(), 0);
    vector.emplace_back(1.0);
    ASSERT_EQ(vector.size(), 1);
}

TEST(Vector, EmplaceBack) {
    using Vector = learn::vector<double>;
    Vector vector;

    vector.emplace_back(0.23);
    EXPECT_NEAR(vector.back(), 0.23, 1E-8);
}