#include "learn_stl/optional.h"

#include <gmock/gmock.h>
#include <gtest/gtest.h>

#include "helpers.h"

template <typename TypeT>
class OptionalTest : public ::testing::Test {
  public:
};

using Types = testing::Types<double, float, int, unsigned char>;
TYPED_TEST_CASE(OptionalTest, Types);

TYPED_TEST(OptionalTest, createEmpty) {
    using Optional = learn::optional<TypeParam>;

    ASSERT_NO_THROW({ Optional optional; });
}

TYPED_TEST(OptionalTest, createValue) {
    using Optional = learn::optional<TypeParam>;

    ASSERT_NO_THROW({ Optional optional = generate<TypeParam>(); });
    ASSERT_NO_THROW({ Optional optional(generate<TypeParam>()); });
}

TYPED_TEST(OptionalTest, hasValue) {
    using Optional = learn::optional<TypeParam>;

    Optional optional;
    ASSERT_FALSE(optional.has_value());
    ASSERT_FALSE(bool(optional));

    optional = generate<TypeParam>();
    ASSERT_TRUE(optional.has_value());
    ASSERT_TRUE(bool(optional));
}

TYPED_TEST(OptionalTest, value) {
    using Optional = learn::optional<TypeParam>;

    Optional optional;
    ASSERT_THROW({ optional.value(); }, std::bad_cast);

    optional = generate<TypeParam>();
    EXPECT_EQ(optional.value(), generate<TypeParam>());
}

TYPED_TEST(OptionalTest, DISABLED_constValue) {
    using Optional = learn::optional<TypeParam>;

    Optional optional;
    ASSERT_THROW({ optional.value(); }, std::bad_cast);

    optional = generate<TypeParam>();
    const auto const_optional = optional;
    EXPECT_EQ(const_optional.value(), generate<TypeParam>());
}

TYPED_TEST(OptionalTest, swap) {}

TYPED_TEST(OptionalTest, reset) {
    using Optional = learn::optional<TypeParam>;

    Optional optional = generate<TypeParam>();
    ASSERT_TRUE(optional.has_value());

    ASSERT_NO_THROW({ optional.reset(); });
    ASSERT_FALSE(optional.has_value());
}

TYPED_TEST(OptionalTest, emplace) {
    using Optional = learn::optional<TypeParam>;

    Optional optional;
    optional.emplace(generate<TypeParam>());

    EXPECT_EQ(optional.value(), generate<TypeParam>());
}