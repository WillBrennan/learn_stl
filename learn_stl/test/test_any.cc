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

TYPED_TEST(AnyTest, reset) {
    using learn::any;

    any any_value = generate<TypeParam>();
    EXPECT_TRUE(any_value.has_value());

    any_value.reset();
    EXPECT_FALSE(any_value.has_value());
}

TYPED_TEST(AnyTest, swap) {
    using learn::any;

    any any_value0 = generate<TypeParam>();
    any any_value1;

    any_value0.swap(any_value1);

    EXPECT_FALSE(any_value0.has_value());
    EXPECT_TRUE(any_value1.has_value());
}

TYPED_TEST(AnyTest, type) {
    using learn::any;

    any value;
    EXPECT_EQ(value.type(), typeid(void));

    value.emplace<TypeParam>(generate<TypeParam>());
    EXPECT_EQ(value.type(), typeid(TypeParam));
}

TYPED_TEST(AnyTest, any_cast) {
    using learn::any;

    any value = generate<TypeParam>();
    const auto recovered_value = learn::any_cast<TypeParam>(value);

    EXPECT_EQ(recovered_value, generate<TypeParam>());
}