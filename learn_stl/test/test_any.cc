#include "learn_stl/any.h"

#include <gmock/gmock.h>
#include <gtest/gtest.h>

#include "helpers.h"

TEST(Any, emptyConstruction) {
    using learn::any;

    ASSERT_NO_THROW({ any any_value; });
}

template <typename TypeT>
class AnyTest : public ::testing::Test {
  public:
};

using Types = testing::Types<double, float, int, unsigned char>;
TYPED_TEST_CASE(AnyTest, Types);

TYPED_TEST(AnyTest, construction) {
    using learn::any;

    ASSERT_NO_THROW({ any any_value(generate<TypeParam>()); });
    ASSERT_NO_THROW({ any any_value = generate<TypeParam>(); });
}

TYPED_TEST(AnyTest, has_value) {
    using learn::any;

    any any_value;
    EXPECT_FALSE(any_value.has_value());

    any_value = generate<TypeParam>();
    EXPECT_TRUE(any_value.has_value());
}

TYPED_TEST(AnyTest, emplace) {
    using learn::any;

    ASSERT_NO_THROW({
        any any_value;

        any_value.emplace<TypeParam>(generate<TypeParam>());
        EXPECT_TRUE(any_value.has_value());
    });
}
