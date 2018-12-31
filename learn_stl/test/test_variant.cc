#include "learn_stl/variant.h"

#include <gmock/gmock.h>
#include <gtest/gtest.h>

TEST(Variant, DetailIndexOf) {
    using learn::detail::index_of;

    const auto index = index_of<double, char, double, int>();
    ASSERT_EQ(index, 1);
}

TEST(Variant, DefaultConstruction) {
    using learn::variant;

    variant<int, double, char> our_variant;
}

TEST(Variant, ValueConstructor) {
    using learn::variant;

    variant<int, double, char> value0 = 3;
    ASSERT_EQ(value0.index(), 0);

    variant<int, double, char> value1 = 'a';
    ASSERT_EQ(value1.index(), 2);

    variant<int, double, char> value2 = 2.32911;
    ASSERT_EQ(value2.index(), 1);
}

TEST(Variant, AssignValueOperator) {
    using learn::variant;

    variant<int, double, char> value = 3;
    ASSERT_EQ(value.index(), 0);

    value = 'a';
    ASSERT_EQ(value.index(), 2);

    value = 2.32911;
    ASSERT_EQ(value.index(), 1);
}

TEST(Variant, TypeGet) {
    using learn::bad_variant_access;
    using learn::get;
    using learn::variant;

    struct MyValue {};

    variant<char, double, unsigned int> value = 'v';

    ASSERT_EQ(get<char>(value), 'v');

    ASSERT_THROW(get<double>(value), bad_variant_access);
    ASSERT_THROW(get<unsigned int>(value), bad_variant_access);
}

TEST(Variant, ConstTypeGet) {
    using learn::bad_variant_access;
    using learn::get;
    using learn::variant;

    struct MyValue {};

    const variant<char, double, unsigned int> value = 'v';

    ASSERT_EQ(get<char>(value), 'v');

    ASSERT_THROW(get<double>(value), bad_variant_access);
    ASSERT_THROW(get<unsigned int>(value), bad_variant_access);
}

TEST(Variant, IndexGet) {
    using learn::bad_variant_access;
    using learn::get;
    using learn::variant;

    variant<char, double, unsigned int> value = 'v';

    ASSERT_EQ(get<0>(value), 'v');

    ASSERT_THROW(get<1>(value), bad_variant_access);
    ASSERT_THROW(get<2>(value), bad_variant_access);
}

TEST(Variant, ConstIndexGet) {
    using learn::bad_variant_access;
    using learn::get;
    using learn::variant;

    const variant<char, double, unsigned int> value = 'v';

    ASSERT_EQ(get<0>(value), 'v');

    ASSERT_THROW(get<1>(value), bad_variant_access);
    ASSERT_THROW(get<2>(value), bad_variant_access);
}