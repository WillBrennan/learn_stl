#include "learn_stl/memory.h"

#include <gmock/gmock.h>
#include <gtest/gtest.h>

#include "helpers.h"
#include "learn_stl/array.h"

template <typename TypeT>
class AddressOfTest : public ::testing::Test {};

using Types = testing::Types<double, float, int, unsigned char, learn::array<double, 3>>;
TYPED_TEST_CASE(AddressOfTest, Types);

TYPED_TEST(AddressOfTest, NoOverload) {
    using learn::addressof;

    const auto some_var = helpers::generate<TypeParam>();

    EXPECT_EQ(&some_var, addressof(some_var));
}