#include "learn_stl/tuple.h"

#include <gmock/gmock.h>
#include <gtest/gtest.h>

TEST(Tuple, Create) {
    using Tuple = learn::tuple<int, float, char>;
    ASSERT_NO_THROW({ const auto tuple = Tuple(2, 0.3f, 'a'); });
    ASSERT_NO_THROW({ Tuple tuple(-123, 483.273f, 'z'); });
};

TEST(Tuple, EmptyTuple) {
    using Tuple = learn::tuple<>;

    // ASSERT_NO_THROW({ Tuple tuple; });
};

TEST(Tuple, tuple_element_t) {
    using Tuple = learn::tuple<int, float, double, char, unsigned int>;

    using Type0 = learn::tuple_element_t<0, Tuple>;
    testing::StaticAssertTypeEq<Type0, int>();

    using Type1 = learn::tuple_element_t<1, Tuple>;
    testing::StaticAssertTypeEq<Type1, float>();

    using Type2 = learn::tuple_element_t<2, Tuple>;
    testing::StaticAssertTypeEq<Type2, double>();

    using Type3 = learn::tuple_element_t<3, Tuple>;
    testing::StaticAssertTypeEq<Type3, char>();

    using Type4 = learn::tuple_element_t<4, Tuple>;
    testing::StaticAssertTypeEq<Type4, unsigned int>();
}

TEST(Tuple, get) {
    using Tuple = learn::tuple<int, float, double, char, unsigned int>;

    Tuple tuple(-3, 1.2f, 2.1, 'a', 3);

    EXPECT_EQ(learn::get<0>(tuple), -3);
    EXPECT_NEAR(learn::get<1>(tuple), 1.2f, 1e-8f);
    EXPECT_NEAR(learn::get<2>(tuple), 2.1, 1e-8);
    EXPECT_EQ(learn::get<3>(tuple), 'a');
    EXPECT_EQ(learn::get<4>(tuple), 3u);
}

