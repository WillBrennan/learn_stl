#include "learn_stl/optional.h"

#include <gmock/gmock.h>
#include <gtest/gtest.h>

#include "learn_stl/array.h"

#include "helpers.h"

template <typename TypeT>
class OptionalTest : public ::testing::Test {
  public:
};

using Types = testing::Types<double, float, int, unsigned char, learn::array<double, 3>>;
TYPED_TEST_CASE(OptionalTest, Types);

TYPED_TEST(OptionalTest, createEmpty) {
    using Optional = learn::optional<TypeParam>;

    ASSERT_NO_THROW({ Optional optional; });
}

TYPED_TEST(OptionalTest, createValue) {
    using Optional = learn::optional<TypeParam>;

    ASSERT_NO_THROW({
        Optional optional = helpers::generate<TypeParam>();
        EXPECT_EQ(optional.value(), helpers::generate<TypeParam>());
    });

    ASSERT_NO_THROW({
        Optional optional(helpers::generate<TypeParam>());
        EXPECT_EQ(optional.value(), helpers::generate<TypeParam>());
    });
}

TYPED_TEST(OptionalTest, hasValue) {
    using Optional = learn::optional<TypeParam>;

    Optional optional;
    ASSERT_FALSE(optional.has_value());
    ASSERT_FALSE(bool(optional));

    optional = helpers::generate<TypeParam>();
    ASSERT_TRUE(optional.has_value());
    ASSERT_TRUE(bool(optional));
}

TYPED_TEST(OptionalTest, value) {
    using Optional = learn::optional<TypeParam>;

    Optional optional;
    ASSERT_THROW({ optional.value(); }, std::bad_cast);

    optional = helpers::generate<TypeParam>();
    EXPECT_EQ(optional.value(), helpers::generate<TypeParam>());
}

TYPED_TEST(OptionalTest, copyAssign) {
    using Optional = learn::optional<TypeParam>;

    Optional optional_a = helpers::generate<TypeParam>();
    Optional optional_b = optional_a;

    EXPECT_TRUE(helpers::equal(optional_a.value(), helpers::generate<TypeParam>()));
    EXPECT_EQ(optional_a.value(), optional_b.value());
}

TYPED_TEST(OptionalTest, constValue) {
    using Optional = learn::optional<TypeParam>;

    Optional optional;
    ASSERT_THROW({ optional.value(); }, std::bad_cast);

    optional = helpers::generate<TypeParam>();
    const auto const_optional = optional;
    EXPECT_EQ(const_optional.value(), helpers::generate<TypeParam>());
}

TYPED_TEST(OptionalTest, swap) {}

TYPED_TEST(OptionalTest, reset) {
    using Optional = learn::optional<TypeParam>;

    Optional optional = helpers::generate<TypeParam>();
    ASSERT_TRUE(optional.has_value());

    ASSERT_NO_THROW({ optional.reset(); });
    ASSERT_FALSE(optional.has_value());
}

TYPED_TEST(OptionalTest, emplace) {
    using Optional = learn::optional<TypeParam>;

    Optional optional;
    optional.emplace(helpers::generate<TypeParam>());

    EXPECT_TRUE(helpers::equal(optional.value(), helpers::generate<TypeParam>()));
}