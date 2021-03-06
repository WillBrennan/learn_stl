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
TYPED_TEST_SUITE(AnyTest, Types);

TYPED_TEST(AnyTest, construction) {
    using learn::any;

    ASSERT_NO_THROW({ any any_value(helpers::generate<TypeParam>()); });
    ASSERT_NO_THROW({ any any_value = helpers::generate<TypeParam>(); });
}

TYPED_TEST(AnyTest, has_value) {
    using learn::any;

    any any_value;
    EXPECT_FALSE(any_value.has_value());

    any_value = helpers::generate<TypeParam>();
    EXPECT_TRUE(any_value.has_value());
}

TYPED_TEST(AnyTest, emplace) {
    using learn::any;

    ASSERT_NO_THROW({
        any any_value;

        any_value.emplace<TypeParam>(helpers::generate<TypeParam>());
        EXPECT_TRUE(any_value.has_value());
    });
}

TYPED_TEST(AnyTest, reset) {
    using learn::any;

    any any_value = helpers::generate<TypeParam>();
    EXPECT_TRUE(any_value.has_value());

    any_value.reset();
    EXPECT_FALSE(any_value.has_value());
}

TYPED_TEST(AnyTest, swap) {
    using learn::any;
    using learn::swap;

    any any_value0 = helpers::generate<TypeParam>();
    any any_value1;

    any_value0.swap(any_value1);

    EXPECT_FALSE(any_value0.has_value());
    EXPECT_TRUE(any_value1.has_value());

    swap(any_value0, any_value1);
    EXPECT_TRUE(any_value0.has_value());
    EXPECT_FALSE(any_value1.has_value());
}

TYPED_TEST(AnyTest, type) {
    using learn::any;

    any value;
    EXPECT_EQ(value.type(), typeid(void));

    value.emplace<TypeParam>(helpers::generate<TypeParam>());
    EXPECT_EQ(value.type(), typeid(TypeParam));
}

TYPED_TEST(AnyTest, AnyCastPtr) {
    using learn::any;
    using learn::any_cast;

    const auto actual_value = helpers::generate<TypeParam>();
    any value = actual_value;

    const auto value_ptr = any_cast<TypeParam>(&value);

    ASSERT_TRUE(value_ptr);
    ASSERT_EQ(*value_ptr, actual_value);
}

TYPED_TEST(AnyTest, AnyCastPtrBad) {
    using learn::any;
    using learn::any_cast;

    const auto actual_value = helpers::generate<TypeParam>();
    any value = actual_value;

    const auto value_ptr = any_cast<char>(&value);

    ASSERT_FALSE(value_ptr);
}

TYPED_TEST(AnyTest, ConstAnyCastPtr) {
    using learn::any;
    using learn::any_cast;

    const auto actual_value = helpers::generate<TypeParam>();
    const any value = actual_value;

    const auto value_ptr = any_cast<TypeParam>(&value);

    ASSERT_TRUE(value_ptr);
    ASSERT_EQ(*value_ptr, actual_value);
}

TYPED_TEST(AnyTest, ConstAnyCastPtrBad) {
    using learn::any;
    using learn::any_cast;

    const auto actual_value = helpers::generate<TypeParam>();
    any value = actual_value;

    const auto value_ptr = any_cast<char>(&value);

    ASSERT_FALSE(value_ptr);
}

TYPED_TEST(AnyTest, AnyCastRef) {
    using learn::any;

    any value = helpers::generate<TypeParam>();
    const auto recovered_value = learn::any_cast<TypeParam>(value);

    EXPECT_EQ(recovered_value, helpers::generate<TypeParam>());
}

TYPED_TEST(AnyTest, AnyCastRefBad) {
    using learn::any;

    any value = helpers::generate<TypeParam>();

    ASSERT_THROW({ const auto recovered_value = learn::any_cast<char>(value); }, std::bad_cast);
}

TYPED_TEST(AnyTest, ConstAnyCastRef) {
    using learn::any;

    const any value = helpers::generate<TypeParam>();
    const auto recovered_value = learn::any_cast<TypeParam>(value);

    EXPECT_EQ(recovered_value, helpers::generate<TypeParam>());
}

TYPED_TEST(AnyTest, ConstAnyCastRefBad) {
    using learn::any;

    const any value = helpers::generate<TypeParam>();

    ASSERT_THROW({ const auto recovered_value = learn::any_cast<char>(value); }, std::bad_cast);
}

TYPED_TEST(AnyTest, MoveAnyCast) {
    using learn::any;

    const any value = helpers::generate<TypeParam>();
    const auto recovered_value = learn::any_cast<TypeParam>(move(value));

    EXPECT_EQ(recovered_value, helpers::generate<TypeParam>());
}

TYPED_TEST(AnyTest, MoveAnyCastBad) {
    using learn::any;

    const any value = helpers::generate<TypeParam>();

    ASSERT_THROW({ const auto recovered_value = learn::any_cast<char>(move(value)); },
                 std::bad_cast);
}

TYPED_TEST(AnyTest, CopyConstruction) {
    using learn::any;

    any value = helpers::generate<TypeParam>();
    any new_value = value;

    new_value.reset();

    ASSERT_FALSE(new_value.has_value());
    ASSERT_TRUE(value.has_value());

    ASSERT_EQ(learn::any_cast<TypeParam>(value), helpers::generate<TypeParam>());
}

TYPED_TEST(AnyTest, MoveConstruction) {
    using learn::any;

    any value = helpers::generate<TypeParam>();
    any new_value = move(value);

    ASSERT_TRUE(new_value.has_value());
    ASSERT_FALSE(value.has_value());

    ASSERT_EQ(learn::any_cast<TypeParam>(new_value), helpers::generate<TypeParam>());
}