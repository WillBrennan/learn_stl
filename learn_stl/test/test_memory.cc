#include "learn_stl/memory.h"

#include <gmock/gmock.h>
#include <gtest/gtest.h>

#include "helpers.h"
#include "learn_stl/array.h"

template <typename TypeT>
class AddressOfTest : public ::testing::Test {};

using Types = testing::Types<double, float, int, unsigned char, learn::array<double, 3>>;
TYPED_TEST_SUITE(AddressOfTest, Types);

TYPED_TEST(AddressOfTest, NoOverload) {
    using learn::addressof;

    const auto some_var = helpers::generate<TypeParam>();

    EXPECT_EQ(&some_var, addressof(some_var));
}

template <typename TypeT>
class UniquePtrTest : public ::testing::Test {};

TYPED_TEST_SUITE(UniquePtrTest, Types);

TYPED_TEST(UniquePtrTest, CtorsEmpty) {
    using learn::unique_ptr;
    const auto some_value = helpers::generate<TypeParam>();

    EXPECT_NO_THROW({
        unique_ptr<TypeParam> some_ptr;
        EXPECT_FALSE(some_ptr);
    });
}

TYPED_TEST(UniquePtrTest, Ctors) {
    using learn::unique_ptr;
    const auto some_value = helpers::generate<TypeParam>();

    EXPECT_NO_THROW({
        unique_ptr<TypeParam> some_ptr(new TypeParam(some_value));
        EXPECT_TRUE(some_ptr);
    });
}

TYPED_TEST(UniquePtrTest, Reset) {
    using learn::unique_ptr;
    auto some_value = helpers::generate<TypeParam>();
    unique_ptr<TypeParam> some_ptr(new TypeParam(some_value));

    ASSERT_TRUE(some_ptr);

    some_ptr.reset();

    ASSERT_FALSE(some_ptr);
}

TYPED_TEST(UniquePtrTest, HasValue) {
    using unique_ptr = learn::unique_ptr<TypeParam>;

    auto some_ptr = unique_ptr{};

    ASSERT_FALSE(some_ptr);

    some_ptr = unique_ptr(new TypeParam(helpers::generate<TypeParam>()));

    ASSERT_TRUE(some_ptr);
}

template <typename TypeT>
class MakeUniqueTest : public ::testing::Test {};

TYPED_TEST_SUITE(MakeUniqueTest, Types);

TYPED_TEST(MakeUniqueTest, Construct) {
    using learn::make_unique;
    using learn::unique_ptr;
    const auto some_value = helpers::generate<TypeParam>();
    const auto some_ptr = make_unique<TypeParam>(some_value);

    ::testing::StaticAssertTypeEq<decltype(some_ptr), const unique_ptr<TypeParam>>();

    EXPECT_EQ(*some_ptr, some_value);
    EXPECT_EQ(*(some_ptr.operator->()), some_value);
    EXPECT_TRUE(some_ptr);
}
