#include "learn_stl/vector.h"

#include <stdexcept>

#include <gmock/gmock.h>
#include <gtest/gtest.h>

#include "learn_stl/memory.h"

TEST(Vector, DefaultConstruct) {
    using Vector = learn::vector<double>;
    Vector vector;
}

TEST(Vector, CountValueConstructor) {
    using Vector = learn::vector<int>;

    Vector vector(30, 2321);

    ASSERT_THAT(vector, testing::SizeIs(30));
    EXPECT_THAT(vector, testing::Contains(2321));
}

TEST(Vector, Size) {
    using Vector = learn::vector<double>;

    Vector vector;
    ASSERT_EQ(vector.size(), 0);

    vector.emplace_back(1.0);
    ASSERT_EQ(vector.size(), 1);

    vector.emplace_back(1.0);
    ASSERT_EQ(vector.size(), 2);

    vector.emplace_back(1.0);
    ASSERT_EQ(vector.size(), 3);
}

TEST(Vector, EmplaceAndAccess) {
    using Vector = learn::vector<double>;
    Vector vector;

    vector.emplace_back(0.23);
    vector.emplace_back(2.34);
    vector.emplace_back(3.45);

    ASSERT_EQ(vector[0], 0.23);
    ASSERT_EQ(vector[1], 2.34);
    ASSERT_EQ(vector[2], 3.45);
}

TEST(Vector, At) {
    using Vector = learn::vector<double>;
    Vector vector;

    vector.emplace_back(0.23);
    vector.emplace_back(2.34);
    vector.emplace_back(3.45);

    ASSERT_EQ(vector.at(0), 0.23);
    ASSERT_EQ(vector.at(1), 2.34);
    ASSERT_EQ(vector.at(2), 3.45);
}

TEST(Vector, AtOutOfBounds) {
    using Vector = learn::vector<double>;

    Vector vector;
    ASSERT_THROW({ vector.at(0); }, std::out_of_range);

    vector.emplace_back(0.1);
    ASSERT_EQ(vector.at(0), 0.1);
    ASSERT_THROW({ vector.at(1); }, std::out_of_range);
}

TEST(Vector, Front) {
    using Vector = learn::vector<double>;

    Vector vector;
    vector.emplace_back(0.23);

    ASSERT_EQ(vector.front(), 0.23);

    vector.emplace_back(12.11);

    ASSERT_EQ(vector.front(), 0.23);
}

TEST(Vector, Back) {
    using Vector = learn::vector<double>;

    Vector vector;
    vector.emplace_back(0.23);

    ASSERT_EQ(vector.back(), 0.23);

    vector.emplace_back(12.11);

    ASSERT_EQ(vector.back(), 12.11);
}

TEST(Vector, Data) {
    using Vector = learn::vector<double>;
    using learn::addressof;

    Vector vector;
    vector.emplace_back(-0.1);
    vector.emplace_back(0.23);
    vector.emplace_back(2329);

    ASSERT_EQ(vector.data(), addressof(vector.at(0)));
}

TEST(Vector, Reserve) {
    using Vector = learn::vector<double>;

    Vector vector;
    ASSERT_EQ(vector.capacity(), 0);

    vector.emplace_back(1.02);
    ASSERT_GE(vector.capacity(), 1);

    vector.reserve(100);
    ASSERT_EQ(vector.capacity(), 100);
}

TEST(Vector, Erase) {
    using Vector = learn::vector<double>;

    Vector vector;
    vector.emplace_back(1.0);
    vector.emplace_back(1.5);
    vector.emplace_back(2.0);
    vector.emplace_back(3.0);
    vector.emplace_back(3.2);

    ASSERT_EQ(vector.size(), 5);

    vector.erase(vector.begin() + 2, vector.begin() + 4);

    ASSERT_EQ(vector.size(), 3);

    ASSERT_THAT(vector, testing::ElementsAre(1.0, 1.5, 3.2));
}

TEST(Vector, EraseSingle) {
    using Vector = learn::vector<double>;

    Vector vector;
    vector.emplace_back(1.0);
    vector.emplace_back(1.5);
    vector.emplace_back(2.0);
    vector.emplace_back(3.0);
    vector.emplace_back(3.2);

    ASSERT_EQ(vector.size(), 5);

    vector.erase(vector.begin() + 0);

    ASSERT_EQ(vector.size(), 4);

    ASSERT_THAT(vector, testing::ElementsAre(1.5, 2.0, 3.0, 3.2));
}