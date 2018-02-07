#include "learn_stl/detail/unordered_container.h"

#include <gmock/gmock.h>
#include <gtest/gtest.h>

namespace helpers {
struct BadHash {
    std::size_t operator()(const int value) { return value % 10; }
};

}  // namespace helpers

TEST(UnorderedContainer, Constructor) {
    using Unordered =
        learn::detail::Unordered<int, std::hash<int>, std::equal_to<int>, std::allocator<int>>;

    ASSERT_NO_THROW({ Unordered unordered(20); });
}

TEST(UnorderedContainer, Emplace) {
    using Unordered =
        learn::detail::Unordered<int, helpers::BadHash, std::equal_to<int>, std::allocator<int>>;

    Unordered unordered(10);
}