#include "learn_stl/optional.h"

#include <gmock/gmock.h>
#include <gtest/gtest.h>

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

    ASSERT_NO_THROW({ Optional optional = TypeParam{}; });
    ASSERT_NO_THROW({ Optional optional(TypeParam{}); });
}

TYPED_TEST(OptionalTest, hasValue) {
    using Optional = learn::optional<TypeParam>;

    Optional optional;
    ASSERT_FALSE(optional.has_value());
    ASSERT_FALSE(bool(optional));

    optional = TypeParam{};
    ASSERT_TRUE(optional.has_value());
    ASSERT_TRUE(bool(optional));
}

TYPED_TEST(OptionalTest, value) {
    using Optional = learn::optional<TypeParam>;

    Optional optional;
    ASSERT_THROW({ optional.value(); }, std::bad_cast);
}

TYPED_TEST(OptionalTest, constValue) {}
